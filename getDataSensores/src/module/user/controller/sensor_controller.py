from flask import jsonify
from dotenv import load_dotenv
import os
import jwt 
from src.module.user.serializer.response import ResposeApi
from src.module.user.middleware.middleware import Middleware
from src.module.user.repository.sensor_repository import SensorRepository
from src.rabbitmq_server import RabbitMQServer
import src.config.constant.status_rabbitmq as const_rabbit
import json 


load_dotenv()
class SensorController:
    def __init__(self, type: str):        
        self.type = type
        if self.type != 'mqtt':
            self.MiddlewareController = Middleware()
            self.jsonData, self.status = self.MiddlewareController.check_if_user_login()
    
        self.Rabbitmq_server = RabbitMQServer(const_rabbit.URI_RABBIT)
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
                self.Rabbitmq_server.publishInQueueLogs(f'Solicitado a inclusão de dados para o sensor de temperatura através do protocolo: {self.type}', const_rabbit.LOGS, const_rabbit.NA)
                statusResponse = self.Repository.insertDataTemperatura(codigo=data['codigo'], name=data['name'], value=data['value'])
                
                
                message = None 
                if statusResponse.status == '200 OK':
                    message = f'Inclusão realizada com sucesso para o sensor de temperatura através do protocolo: {self.type}'
                    
                else:
                    message = f'Erro ao realizar a inclusão o sensor de temperatura através do protocolo: {self.type}. Error: {statusResponse.message}'
                    
                self.Rabbitmq_server.publishInQueueLogs(message, const_rabbit.LOGS, statusResponse.status)
                
                return statusResponse
            except Exception as ex:
                return ResposeApi(500, ex, self.__class__.__name__).response('error')
            
            
        self.Rabbitmq_server.publishInQueueLogs(f'Solicitado inserção no banco de dados mas, houve erro de autenticação. Error: {self.jsonData.message}', const_rabbit.LOGS, self.jsonData.status)
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
                
                # encaminha log para o rabbitmq de inserção de dados no banco
                self.Rabbitmq_server.publishInQueueLogs(f'Solicitado a inclusão de dados para o sensor de distancia através do protocolo: {self.type}', const_rabbit.LOGS, const_rabbit.NA)
                statusResponse = self.Repository.insertDataDistancia(codigo=data['codigo'], name=data['name'], value=data['value'])
                
                
                message = None 
                if statusResponse.status == '200 OK':
                    message = f'Inclusão realizada com sucesso para o sensor de distancia através do protocolo: {self.type}'
                    
                else:
                    message = f'Erro ao realizar a inclusão o sensor de distancia através do protocolo: {self.type}. Error: {statusResponse.message}'
                    
                self.Rabbitmq_server.publishInQueueLogs(message, const_rabbit.LOGS, statusResponse.status)
                
                return statusResponse
            except Exception as ex:
                return ResposeApi(500, ex, self.__class__.__name__).response('error')
            
        self.Rabbitmq_server.publishInQueueLogs(f'Solicitado inserção no banco de dados mas, houve erro de autenticação. Error: {self.jsonData.message}', const_rabbit.LOGS, self.jsonData.status)        
        return self.jsonData
    
    def getAllDataTemperatura(self):
        if self.status:
            try:
                self.Rabbitmq_server.publishInQueueLogs(f'Solicitado a visualização dos dados para o sensor de temperatura', const_rabbit.LOGS, const_rabbit.NA)
                statusResponse = self.Repository.getAllDataTemperatura()
                
                message = None 
                if statusResponse.status == '200 OK':
                    message = f'Solicitação realizada com sucesso para dados de Temperatura.'
                    
                else:
                    message = f'Erro ao realizar a requisição para os dados de distancia. Error: {statusResponse.message}'
                    
                self.Rabbitmq_server.publishInQueueLogs(message, const_rabbit.LOGS, statusResponse.status)
                
                return statusResponse
            except Exception as ex:
                return ResposeApi(500, ex, self.__class__.__name__).response('error')
        return self.jsonData
    
    def getAllDataDistancia(self):
        if self.status:
            try:
                self.Rabbitmq_server.publishInQueueLogs(f'Solicitado a visualização dos dados para o sensor de temperatura', const_rabbit.LOGS, const_rabbit.NA)
                statusResponse = self.Repository.getAllDataDistancia()
                message = None 
                
                if statusResponse.status == '200 OK':
                    print('entrei aqui')
                    message = f'Solicitação realizada com sucesso para dados de distancia.'
                    
                else:
                    message = f'Erro ao realizar a requisição para os dados de distancia. Error: {statusResponse.message}'
                    
                self.Rabbitmq_server.publishInQueueLogs(message, const_rabbit.LOGS, statusResponse.status)
                return statusResponse
            except Exception as ex:
                return ResposeApi(500, ex, self.__class__.__name__).response('error')
        return self.jsonData



    