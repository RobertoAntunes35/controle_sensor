from main import app

from flask_mqtt import Mqtt
from src.module.user.service.sensor_service import Service
mqtt = Mqtt(app)
mqtt.init_app(app)


@mqtt.on_connect()
def handle_connect(client, userdata, flags, rc):
    mqtt.subscribe('sensores/temperatura')
    mqtt.subscribe('sensores/distancia')
    print('Conectado ao broker')
    
@mqtt.on_message()
def handle_message(client, userdata, message):
    service = Service()
    try:
        # topico sensor de temperatura
        if message.topic == 'sensores/temperatura':
            service.insertTemperaturaData(message.payload)
        elif message.topic == 'sensores/distancia':
            pass 
        # topico sensor de distancia
            
        pass 

    except:
        pass 
    
    
    print('Message: ', message.topic)