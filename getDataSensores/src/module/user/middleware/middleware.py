from flask import request, jsonify
from dotenv import load_dotenv
import os 
import jwt 
from src.module.user.serializer.response import ResposeApi
import json 


load_dotenv()
class Middleware:
    SECRET_KEY = os.getenv('SECRET_KEY') 
    
    def __init__(self):
        self.request = request
            
    def constructor(self) -> json:
        try:
            self.authorization = self.request.headers.get('Authorization')
            self.token = self.authorization.split(" ")[1]
            
            decoded_token = jwt.decode(self.token, self.SECRET_KEY, algorithms=["HS256"]) 
            print(decoded_token)
            return jsonify(decoded_token), True
        except jwt.ExpiredSignatureError:
            return ResposeApi(401, "Token expirado").response('error'), False
        except jwt.InvalidTokenError:
            return ResposeApi(401, "Token invalido").response('error'), False
        except Exception:
            return ResposeApi(500, "Token não encontrado ou invalido.").response('error'), False
    
    def check_if_user_login(self) -> bool:
        """Verifica se os dados do usuário esta no rabbitmq"""
        # consulta ao broker
        return self.constructor()
 