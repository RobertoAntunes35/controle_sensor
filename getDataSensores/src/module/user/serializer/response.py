from flask import jsonify
import json
import src.config.constant.status_rabbitmq as const_rabbit
from src.rabbitmq import RabbitMQServer


class ResposeApi:
    def __init__(self, status: int, message: str, classe = None):
        self.status = status
        self.message = str(message)
        self.classe = classe
        self.rabbitmq = RabbitMQServer()
        
    def response(self, message):
        messageReturn = {
            message:self.status,
            'message':self.message,
            'class': self.classe
        }
        if message == 'status':
            routing_key = 'info'
        else:
            routing_key = 'error'
                
        self.rabbitmq.publish_message(routing_key, self.message, self.status)
        print(messageReturn)
        return messageReturn, self.status

    def response_data(self, data):
        response_body = {
            'class': self.classe,
            'status': self.status,
            'data': data,
        }
        
        print(response_body)
        return response_body, self.status