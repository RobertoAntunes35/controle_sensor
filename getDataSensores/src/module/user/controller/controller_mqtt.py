from flask import jsonify
from dotenv import load_dotenv
import os 
import jwt 
from src.module.user.serializer.response import ResposeApi
from src.module.user.middleware.middleware import Middleware
from src.module.user.repository.sensor_repository import SensorRepository
import json 


load_dotenv()
class SensorController:
    SECRET_KEY = os.getenv('SECRET_KEY') 
    
    def __init__(self):
        self.Repository = SensorRepository()
        
        
    def insertDataTemperatura(self, req, type = None):
        if type == 'mqtt':
            self.status = True

        if self.status:
            try:
                data = json.loads(req.data)
                # valida a informação com um serializer
                return self.Repository.insertDataTemperatura(codigo=data['codigo'], name=data['name'], value=data['value'])
            except Exception as ex:
                return ResposeApi(500, ex, self.__class__.__name__).response('error')
        return self.jsonData
    

    def insertDataDistancia(self, req, type = None):
        if type == 'mqtt':
            self.status = True
        
        if self.status:
            try:
                data = json.loads(req.data)
                # valida a informação com um serializer
                return self.Repository.insertDataDistancia(codigo=data['codigo'], name=data['name'], value=data['value'])
            except Exception as ex:
                return ResposeApi(500, ex, self.__class__.__name__).response('error')
        return self.jsonData
    
    
    
    def getAllDataTemperatura(self):
        if self.status:
            try:
                return self.Repository.getAllDataTemperatura()
            except Exception as ex:
                return ResposeApi(500, ex, self.__class__.__name__).response('error')
        return self.jsonData
    
    def getAllDataDistancia(self):
        if self.status:
            try:
                return self.Repository.getAllDataDistancia()
            except Exception as ex:
                return ResposeApi(500, ex, self.__class__.__name__).response('error')
        return self.jsonData



    
        
