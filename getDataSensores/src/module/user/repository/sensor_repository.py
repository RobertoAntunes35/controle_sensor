from src.module.user.model.sensores_model import db, Distancia, Temperatura
import datetime

from src.module.user.serializer.response import ResposeApi

class SensorRepository:
    def __init__(self, type):
        self.time = datetime.datetime.now() 
        self.type = type 
    
    def insertDataTemperatura(self, codigo: str, name: str, value: float):
        try:
            new_temperatura = Temperatura(codigo=codigo, name=name, value=value, timestamp=self.time)
            db.session.add(new_temperatura)
            db.session.commit()
            if self.type == 'mqtt':
                
                response = {
                    'status':200,
                    'message':f'insert successfully in Temperatura by protocol MQTT with ESP32',
                    'class': self.__class__.__name__
                }, 200
                
                print(response)
                return response

            return ResposeApi(200, "insert successfully in Temperatura by protocol HTTP").response('status')

        except Exception as ex:
            if self.type == 'mqtt':
                response =  {
                    'status':200,
                    'message':f'erro trying to insert data in database TEMPERATURA by protocol MQTT with ESP32 {ex}',
                    'class': self.__class__.__name__
                }, 200
                
                print(response)
                
                return response
                
            return ResposeApi(500, ex, self.__class__.__name__).response('error')
        finally:
            db.session.close()
            
    
    def insertDataDistancia(self, codigo: str, name: str, value: float):
        try:
            new_temperatura = Distancia(codigo=codigo, name=name, value=value, timestamp=self.time)
            db.session.add(new_temperatura)
            db.session.commit()
            
            if self.type == 'mqtt':
                response =  {
                    'status':200,
                    'message':'insert successfully in Distancia by protocol MQTT with ESP32',
                    'class': self.__class__.__name__
                }, 200
                
                print(response)
                return response
            return ResposeApi(200, "insert successfully in Distancia by protocol HTTP").response('status')
        except Exception as ex:
            if type == 'mqtt':
                response = {
                    'error': 500,
                    'message': f'erro trying to insert data in database TEMPERATURA by protocol MQTT with ESP32 {ex}',
                    'class': self.__class__.__name__
                }, 500
                
                print(response)
                
                return response
                
            return ResposeApi(500, ex, self.__class__.__name__).response('error')
        finally:
            db.session.close()
    
        
    def getAllDataTemperatura(self):
        try:
            all_data_temperatura = db.session.query(Temperatura).all()
            listTemperatura = [value.to_dict() for value in all_data_temperatura]

            return ResposeApi(200, None, self.__class__.__name__).response_data(listTemperatura)
        except Exception as ex:
            return ResposeApi(500, ex).response('error')
        finally:
            db.session.close()
        
        
    def getAllDataDistancia(self):
        try:
            all_data_distancia = db.session.query(Distancia).all()
            listDistancia = [value.to_dict() for value in all_data_distancia]
            return ResposeApi(200, None, self.__class__.__name__).response_data(listDistancia)
        except Exception as ex:
            return ResposeApi(500, ex, self.__class__.__name__).response('error')
        finally:
            db.session.close()
            
        
      