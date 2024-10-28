from main import app

from flask_mqtt import Mqtt
import json 

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
            # topico sensor de temperatura
            if message.topic == 'sensores/temperatura':
                
                # pega o dado, da seguinte forma: codigo,1/name,sensor_temperatura/value,10.00
                
                
                #response = Controller.insertDataTemperatura(message.payload.decode())
                msg = str(message.payload.decode()).split('/')
                dicionario = {item.split(",")[0]: item.split(",")[1] for item in msg}
                
                Controller.insertDataTemperatura(dicionario)
                
                
                
                
                
            elif message.topic == 'sensores/distancia':
                pass 
            # topico sensor de distancia
                
            pass 

    except:
        pass 
    