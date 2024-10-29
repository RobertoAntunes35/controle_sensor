from flask import jsonify
import json


class ResposeApi:
    def __init__(self, status: int, message: str, classe = None):
        self.status = status
        self.message = str(message)
        self.classe = classe
        
    def response(self, message):
        messageReturn = {
            message:self.status,
            'message':self.message,
            'class': self.classe
        }
        
        print(messageReturn)
        return jsonify(messageReturn), self.status

    def response_data(self, data):
        response_body = {
            'class': self.classe,
            'status': self.status,
            'data': data,

        }
        print(response_body)
        return jsonify(response_body)