from flask import jsonify
import json


class ResposeApi:
    def __init__(self, status: int, message: str, classe = None):
        self.status = status
        self.message = str(message)
        self.classe = classe
    def response(self, message):
        return jsonify({
            message:self.status,
            'message':self.message,
            'class': self.classe
        }), self.status
        
    def response_data(self, data):
        print(type(self.message))
        response_body = {
            'class': self.classe,
            'status': self.status,
            'data': data,

        }
        
        return jsonify(response_body)