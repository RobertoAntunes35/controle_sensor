from flask import jsonify
class ResposeApi:
    def __init__(self, status: int, message: str):
        self.status = status
        self.message = message
        
    def response(self):
        return jsonify({
            'status':self.status,
            'message':self.message
        })