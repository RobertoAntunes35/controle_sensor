from main import app

from flask_mqtt import Mqtt
import json 
from pydantic import ValidationError
from src.module.user.dto.dto_mqtt_input import RequestMQTT
from src.module.user.controller.sensor_controller import SensorController
from src.rabbitmq_server import RabbitMQServer
import src.config.constant.status_rabbitmq as const_rabbit
mqtt = Mqtt(app)

@mqtt.on_connect()
def handle_connect(client, userdata, flags, rc):
    mqtt.subscribe('sensores/temperatura')
    mqtt.subscribe('sensores/distancia')
    print('Conectado ao broker')
    
@mqtt.on_message()
def handle_message(client, userdata, message):
    Rabbitmq_server = RabbitMQServer(const_rabbit.URI_RABBIT)
    try:
        with app.app_context():
            
            Controller = SensorController('mqtt')
            request = getInfoRequestMQTT(message)
            
            
            if request is None or request == None:
                raise Exception('O valor para request não pode ser vazio.')
                
            # topico sensor de temperatura
            if message.topic == 'sensores/temperatura':
                Controller.insertDataTemperatura(request)
                
            
            elif message.topic == 'sensores/distancia':
                Controller.insertDataDistancia(request)
            
    except Exception as ex:
        msg = f'Erro ao inserir informacoes atraves do protocolo MQTT. Error: {str(ex)}'
        Rabbitmq_server.publishInQueueLogs(msg, const_rabbit.LOGS, 500)
    finally:
        Rabbitmq_server.close()


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
    
    