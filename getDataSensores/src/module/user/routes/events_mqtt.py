from main import app

from flask_mqtt import Mqtt
import json 
from pydantic import ValidationError
from src.module.user.dto.dto_mqtt_input import RequestMQTT
from src.module.user.controller.sensor_controller import SensorController

mqtt = Mqtt(app)
mqtt.init_app(app)

@mqtt.on_connect()
def handle_connect(client, userdata, flags, rc):
    mqtt.subscribe('sensores/temperatura')
    mqtt.subscribe('sensores/distancia')
    print('Conectado ao broker')
    
@mqtt.on_message()
def handle_message(client, userdata, message):
    try:
        with app.app_context():
            
            Controller = SensorController('mqtt')
            request = getInfoRequestMQTT(message)
            
            # topico sensor de temperatura
            if message.topic == 'sensores/temperatura':
                Controller.insertDataTemperatura(request)
            
            elif message.topic == 'sensores/distancia':
                Controller.insertDataDistancia(request)
            # topico sensor de distancia
            
    except Exception as ex:
        print('Error: ', ex)


def getInfoRequestMQTT(message: str) -> dict:
    try:
        msg = str(message.payload.decode()).split('/')
        dictInfo = {item.split(",")[0]: item.split(",")[1] for item in msg}
        RequestMQTT(**dictInfo)
        return dictInfo
    except ValidationError as ex:
        print('Erro ao validar objeto de transferecia.\nError: ', ex)
        pass 
    
    