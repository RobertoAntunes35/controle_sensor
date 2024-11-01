from flask import jsonify
from dotenv import load_dotenv
import os
import jwt 
from src.module.user.serializer.response import ResposeApi
from src.module.user.middleware.middleware import Middleware
from src.module.user.repository.sensor_repository import SensorRepository
import src.config.constant.status_rabbitmq as const_rabbit
from src.rabbitmq import RabbitMQServer
import json 


load_dotenv()
class SensorController:
    def __init__(self, type: str):        
        self.type = type
        if self.type != 'mqtt':
            self.MiddlewareController = Middleware()
            self.jsonData, self.status = self.MiddlewareController.check_if_user_login()
        
        self.rabbitmq = RabbitMQServer()
        self.Repository = SensorRepository(type)
            
    def insertDataTemperatura(self, req):
        if self.type == 'mqtt':
            self.status = True

        if self.status:
            data = None
            try:
                # para requisição através do mqtt
                if self.type == 'mqtt':
                    data = req
                    
                # para requisição http
                elif self.type == 'http':
                    data = json.loads(req.data)

                # encaminha log para o rabbitmq de inserção de dados no banco
                self.rabbitmq.publish_message('info', f'Solicitado insercao de dados para TEMPERATURA pelo protocolo {self.type}', const_rabbit.NA)
                statusResponse, _ = self.Repository.insertDataTemperatura(codigo=data['codigo'], name=data['name'], value=data['value'])
                
                if isinstance(statusResponse, dict):
                    checkResponse = statusResponse.get('status', [])
                    checkMessage = statusResponse.get('message', [])
                
                else:
                    checkResponse = statusResponse.status
                    checkMessage = statusResponse.message
                    
                
                if checkResponse == 200:
                    message = f'Inclusao realizada com sucesso para o sensor de temperatura atraves do protocolo: {self.type}'
                    
                else:
                    message = f'Erro ao realizar a inclusao o sensor de temperatura atraves do protocolo: {self.type}. Error: {checkMessage}'
                
                self.rabbitmq.publish_message('info', message, checkResponse)
                return statusResponse
            except Exception as ex:
                return ResposeApi(500, ex, self.__class__.__name__).response('error')
            
        return self.jsonData
    
    def insertDataDistancia(self, req):
        if self.type == 'mqtt':
            self.status = True

        if self.status:
            data = None
            try:
                # para requisição através do mqtt
                if self.type == 'mqtt':
                    data = req
                    
                # para requisição http
                elif self.type == 'http':
                    data = json.loads(req.data)

                self.rabbitmq.publish_message('info', f'Solicitado insercao de dados para DISTANCIA pelo protocolo {self.type}', const_rabbit.NA)        
                statusResponse, _ = self.Repository.insertDataDistancia(codigo=data['codigo'], name=data['name'], value=data['value'])
                
                if isinstance(statusResponse, dict):
                    checkResponse = statusResponse.get('status', [])
                    checkMessage = statusResponse.get('message', [])
                
                else:
                    checkResponse = statusResponse.status
                    checkMessage = statusResponse.message
                    
                
                if checkResponse == 200:
                    message = f'Inclusao realizada com sucesso para o sensor de distancia atraves do protocolo: {self.type}'
                    
                else:
                    message = f'Erro ao realizar a inclusao o sensor de distancia através do protocolo: {self.type}. Error: {checkMessage}'
                    
                self.rabbitmq.publish_message('info', message, checkResponse)
                
                return statusResponse
            except Exception as ex:
                return ResposeApi(500, ex, self.__class__.__name__).response('error')
            
            
        return self.jsonData
    
    def getAllDataTemperatura(self):
        if self.status:
            try:
                
                self.rabbitmq.publish_message('info', 'Solicitado leitura dos dados de temperatura', const_rabbit.NA)        
                statusResponse, _ = self.Repository.getAllDataTemperatura()
                
                message = None 
                if statusResponse.get('status', []) == 200:
                    message = f'Solicitacao realizada com sucesso para dados de Temperatura.'
                    
                else:
                    message = f'Erro ao realizar a requisicao para os dados de distancia. Error: {statusResponse.get('message', [])}'
                
                self.rabbitmq.publish_message('info', message, statusResponse.get('status', []))        
                
                return statusResponse
            except Exception as ex:
                return ResposeApi(500, ex, self.__class__.__name__).response('error')
        return self.jsonData
    
    def getAllDataDistancia(self):
        if self.status:
            try:
                self.rabbitmq.publish_message('info', 'Solicitado leitura dos dados de distancia', const_rabbit.NA)        
                
                statusResponse, _ = self.Repository.getAllDataDistancia()
                
                if statusResponse.get('status', []) == 200:
                    message = f'Solicitacao realizada com sucesso para dados de distancia.'
                    
                else:
                    message = f'Erro ao realizar a requisicao para os dados de distancia. Error: {statusResponse.get('message', [])}'
                    
                self.rabbitmq.publish_message('info', message, statusResponse.get('status', []))        
                                
                return statusResponse
            except Exception as ex:
                return ResposeApi(500, ex, self.__class__.__name__).response('error')
        return self.jsonData



    