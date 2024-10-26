from flask import jsonify
from src.module.user.repository.sensor_repository import SensorRepository
class Service:
    def __init__(self):
        self.Repository = SensorRepository()
    
    def insertTemperaturaData(self, codigo, value):
        try:
            pass 
        except:
            pass 
        finally:
            pass 
        
        return jsonify({
            'status':200,
            'message': 'Successfully insert data'
        }) 