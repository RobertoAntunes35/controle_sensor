from main import app

from flask_mqtt import Mqtt
import json 
from pydantic import ValidationError
from src.module.user.dto.dto_mqtt_input import RequestMQTT
from src.module.user.controller.sensor_controller import SensorController
import src.config.constant.status_rabbitmq as const_rabbit

mqtt = Mqtt(app)

@mqtt.on_connect()
def handle_connect(client, userdata, flags, rc):
    mqtt.subscribe('sensores/movimento')
    mqtt.subscribe('sensores/distancia')
    print('Conectado ao broker')
    
@mqtt.on_message()
def handle_message(client, userdata, message):
    try:
        with app.app_context():
            Controller = SensorController('mqtt')
            request = getInfoRequestMQTT(message)
            
            if request is None or request == None:
                raise Exception('O valor para request não pode ser vazio.')
                
            # # topico sensor de distancia
            if message.topic == 'sensores/movimento':
                Controller.insertDataTemperatura(request)
                
            
            elif message.topic == 'sensores/distancia':
                Controller.insertDataDistancia(request)
            
    except Exception as ex:
        msg = f'Erro ao inserir informacoes atraves do protocolo MQTT. Error: {str(ex)}'
    finally:
        pass 

def getInfoRequestMQTT(message: str) -> dict:
    try:
        msg = str(message.payload.decode()).split('/')
        dictInfo = {item.split(",")[0]: item.split(",")[1] for item in msg}
        RequestMQTT(**dictInfo)
        return dictInfo
    except ValidationError as ex:
        print('Erro ao validar objeto de transferecia. Error: ', ex)
    except Exception as ex2:
        print(f'Erro geral: {ex2}')
    
    