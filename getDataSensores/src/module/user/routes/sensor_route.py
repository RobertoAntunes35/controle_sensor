from flask import jsonify, Blueprint, request, json

from src.module.user.controller.sensor_controller import SensorController
from src.module.user.serializer.response import ResposeApi
routes = Blueprint("routes", __name__)

@routes.route('/')
def home():
    Controller = SensorController()
    return Controller.insertDataTemperatura()

@routes.route('/insert-data/temperatura', methods=['POST'])
def insert_data_temperatura() -> json:
    try:
        Controller = SensorController('http')
        return Controller.insertDataTemperatura(request)        
    except Exception as ex:
        return ResposeApi(500, ex, 'sensor_route.py')
    
@routes.route('/insert-data/distancia', methods=['POST'])
def insert_data_unidade() -> json:
    try:
        Controller = SensorController('http')
        return Controller.insertDataDistancia(request)        
    except Exception as ex:
        return ResposeApi(500, ex, "sensor_route.py")
    
@routes.route('/get-all-data/temperatura', methods=['GET'])
def get_all_data_temperatura() -> json:
    try:
        Controller = SensorController('http')
        return Controller.getAllDataTemperatura()        
    except Exception as ex:
        return ResposeApi(500, ex, "sensor_route.py")
    
@routes.route('/get-all-data/distancia', methods=['GET'])
def get_all_data_distancia() -> json:
    try:
        Controller = SensorController('http')
        return Controller.getAllDataDistancia()        
    except Exception as ex:
        return ResposeApi(500, ex, "sensor_route.py")
    
    
    