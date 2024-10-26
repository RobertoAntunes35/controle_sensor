from main import app

from flask_mqtt import Mqtt
import json 


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
        
        print(message.topic)
        # topico sensor de temperatura
        if message.topic == 'sensores/temperatura':
            
            # pega o dado, como um json {"codigo": 1, "name": "sensor_temperatura", "value": 15.10}
            #response = Controller.insertDataTemperatura(message.payload.decode())
            print(message.payload)
            
            
            
            
        elif message.topic == 'sensores/distancia':
            pass 
        # topico sensor de distancia
            
        pass 

    except:
        pass 
    