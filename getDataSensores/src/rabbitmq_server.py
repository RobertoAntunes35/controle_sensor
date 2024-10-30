
import pika 
import json 
from datetime import datetime

class RabbitMQServer:
    def __init__(self, host='amqp://guest:guest@localhost:5672/%2F'):
        self.host = host
        self.connection = None
        self.channel = None 
    
    def connect(self):
        self.connection = pika.BlockingConnection(pika.URLParameters(self.host))
        self.channel = self.connection.channel()
    
    def publishInQueueLogs(self, message: str, queue: str, status):
        try:
            dict_message = {
            'api': 'getDataSensores',
            'message': message,
            'status':status,
            'time': datetime.now().isoformat()
            }
        
            message_input = json.dumps(dict_message)
            print(message_input)

            if not self.channel:
                self.connect()
            self.channel.basic_publish(exchange='', routing_key=queue, body=message_input)
            print(f' [x] Enviou: {message}')
        except Exception as ex:
            print('ERROR: ', ex)
        
        
    def consume(self, callback, queue: str):
        if not self.channel:
            self.connect()
        def on_message(ch, method, properties, body):
            print(f" [x] Recebeu '{body.decode()}'")
            callback(body.decode())
        
        self.channel.basic_consume(queue=queue, on_message_callback=on_message, auto_ack=True)
        print(' [*] Aguardando mensagens. Pressione CTRL+C para sair.')
        self.channel.start_consuming()
    
    def close(self):
        if self.connection:
            self.connection.close()