from src.module.user.model.sensores_model import db, Distancia, Temperatura
import json 
import datetime

from src.module.user.DTO.api_response import ResposeApi

from flask import jsonify

class SensorRepository:
    def __init__(self):
        self.time = datetime.datetime.now() 
    
    async def insertDataTemperatura(self, name: str, value: float):
        try:
            new_temperatura = Temperatura(name=name, value=value, timestamp=self.time)
            db.session.add(new_temperatura)
            await db.session.commit()
            
            return ResposeApi(200, "insert successfully in Temperatura").response()
        except:
            pass
        
    async def insertDataDistancia(self, name: str, value: float):
        try:
            new_distancia = Distancia(name=name, value=value, timestamp=self.time)
            db.session.add(new_distancia)
            await db.session.commit()
            
            return ResposeApi(200, "insert successfully in Distancia").response()
        except:
            pass
        
      