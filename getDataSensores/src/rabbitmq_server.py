
import pika 
import json 
from datetime import datetime
import uuid
class RabbitMQServer:
    def __init__(self, host='amqp://guest:guest@localhost:5672/%2F'):
        self.host = host
        self.connection = None
        self.channel = None
        self.connect()

    def connect(self):
        if not self.connection or self.connection.is_closed:
            self.connection = pika.BlockingConnection(pika.URLParameters(self.host))
            self.channel = self.connection.channel()

    
    def publishInQueueLogs(self, message: str, queue: str, status):
        try:
            self.channel.tx_select()
            dict_message = {
                'id': str(uuid.uuid4()),
                'api': 'getDataSensores',
                'message': message,
                'status': status,
                'time': datetime.now().isoformat()
            }
            message_input = json.dumps(dict_message)

            properties = pika.BasicProperties(message_id=dict_message['id'])
            self.channel.basic_publish(exchange='', routing_key=queue, body=message_input, properties=properties)
            
            self.channel.tx_commit()
            print(f' [x] MENSAGEM PUBLICADA: {message} [x]')
        
        except Exception as ex:
            self.channel.tx_rollback()
            print('Error publishQueueLogs: ', ex)

        
        
    def consume(self, callback, queue: str):
        def on_message(ch, method, properties, body):
            print(f" [x] Recebeu '{body.decode()}'")
            try:
                callback(body.decode())
                ch.basic_ack(delivery_tag=method.delivery_tag)
            except Exception as e:
                print(f"Erro ao processar mensagem: {e}")
                ch.basic_nack(delivery_tag=method.delivery_tag, requeue=True)
        
        self.channel.basic_consume(queue=queue, on_message_callback=on_message)
    
    def close(self):
        if self.connection:
            self.connection.close()