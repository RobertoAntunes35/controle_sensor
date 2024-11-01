import logging
import pika
from pika import DeliveryMode
from pika.exchange_type import ExchangeType
import os
import uuid
from datetime import datetime
import json 

class RabbitMQServer:
    def __init__(self, host='localhost', username='guest', password='guest'):
        self.setup_logging()
        
        credentials = pika.PlainCredentials(username, password)
        self.parameters = pika.ConnectionParameters(host, credentials=credentials)
        self.connection = None
        self.channel = None
        
        self.exchange = 'logs'
        
        self.declare_exchange_and_queues()
        

    def setup_logging(self):
        log_directory = 'logs'
        os.makedirs(log_directory, exist_ok=True)

        self.logger = logging.getLogger('SensorController')
        self.logger.setLevel(logging.DEBUG)

        info_handler = logging.FileHandler(os.path.join(log_directory, 'info.log'))
        info_handler.setLevel(logging.INFO)
        info_format = logging.Formatter('%(asctime)s - INFO - %(message)s')
        info_handler.setFormatter(info_format)

        error_handler = logging.FileHandler(os.path.join(log_directory, 'error.log'))
        error_handler.setLevel(logging.ERROR)
        error_format = logging.Formatter('%(asctime)s - ERROR - %(message)s')
        error_handler.setFormatter(error_format)

        self.logger.addHandler(info_handler)
        self.logger.addHandler(error_handler)

    def connect(self):
        try:
            self.connection = pika.BlockingConnection(self.parameters)
            self.channel = self.connection.channel()
            
            self.logger.info("Conexão com RabbitMQ estabelecida com sucesso.")
        except Exception as e:
            self.logger.error(f"Erro ao conectar ao RabbitMQ: {e}")
            print(f"Erro ao conectar ao RabbitMQ: {e}")

    def declare_exchange_and_queues(self):        
        if self.connection is None or not self.connection:
            self.connect()
        try:
            self.channel.exchange_declare(
                exchange='logs',
                exchange_type=ExchangeType.direct,
                auto_delete=False,
                durable=True
            )
            
            self.logger.info(f"Exchange '{self.exchange}' declarada com sucesso.")
        except Exception as e:
            print(f"Erro ao declarar o exchange logs: {e}")
            self.logger.error(f"Erro ao declarar o exchange logs: {e}")
        
        try:
            self.channel.queue_declare(queue='info_logs', durable=True)
            self.channel.queue_bind(exchange=self.exchange, queue='info_logs', routing_key='info')
            self.logger.info(f'Fila "info_logs" declarada e vinculada ao exchange "logs" com chave de roteamento "info"')

            self.channel.queue_declare(queue='error_logs', durable=True)
            self.channel.queue_bind(exchange=self.exchange, queue='error_logs', routing_key='error')
            self.logger.info(f'Fila "error_logs" declarada e vinculada ao exchange "logs" com chave de roteamento "error"')
        except Exception as e:
            print(f"Erro ao declarar e vincular as filas.")             
            self.logger.error(f"Erro ao declarar e vincular as filas.")             

    def publish_message(self, routing_key, message, status, persistent=True):
        if self.connection is None or not self.connection:
            self.connect()
        try:
            dict_message = {
                'id': str(uuid.uuid4()),
                'api': 'getDataSensores',
                'message': message,
                'status': status,
                'time': datetime.now().isoformat()
            }
            
            dict_message_json = json.dumps(dict_message)
        except:
            self.logger.error(f"Erro ao criar a mensagem: {e}")
            print(f"Erro ao criar a mensagem: {e}")
        
        try:
            delivery_mode = DeliveryMode.Persistent if persistent else DeliveryMode.Transient
            self.channel.basic_publish(
                exchange=self.exchange,
                routing_key=routing_key,
                body=dict_message_json,
                properties=pika.BasicProperties(content_type='text/plain', delivery_mode=delivery_mode)
            )
            self.logger.info(f"Mensagem enviada para o exchange '{self.exchange}' com a chave de roteamento '{routing_key}': {message}")
        except Exception as e:
            self.logger.error(f"Erro ao enviar mensagem: {e}")
            print(f"Erro ao enviar mensagem: {e}")

    def close_connection(self):
        if self.connection:
            self.connection.close()
            self.logger.info("Conexão com RabbitMQ encerrada com sucesso.")

if __name__ == "__main__":
    rabbit_logger = RabbitMQServer()
    rabbit_logger.publish_message('info', 'Mensagem de informacao 1', 200)
    rabbit_logger.publish_message('error', 'Mensagem de error 3', 500)
    rabbit_logger.close_connection()
