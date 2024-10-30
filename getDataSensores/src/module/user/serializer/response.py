from flask import jsonify
import json
import src.config.constant.status_rabbitmq as const_rabbit
from src.rabbitmq_server import RabbitMQServer


class ResposeApi:
    def __init__(self, status: int, message: str, classe = None):
        self.status = status
        self.message = str(message)
        self.classe = classe
        
        self.Rabbitmq_server = RabbitMQServer(const_rabbit.URI_RABBIT)
        
        
    def response(self, message):
        messageReturn = {
            message:self.status,
            'message':self.message,
            'class': self.classe
        }
        
        self.Rabbitmq_server.publishInQueueLogs(f'Informação gerada pela classe ResponseApi.response(). Message: {self.message}', const_rabbit.LOGS, self.status)
        print(messageReturn)
        return jsonify(messageReturn), self.status

    def response_data(self, data):
        response_body = {
            'class': self.classe,
            'status': self.status,
            'data': data,
        }
        self.Rabbitmq_server.publishInQueueLogs(f'Informação gerada pela classe ResponseApi.response_data(). Data: {type(data)}', const_rabbit.LOGS, self.status)
        
        print(response_body)
        return jsonify(response_body)