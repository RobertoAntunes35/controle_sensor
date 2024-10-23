from main import app

from flask_mqtt import Mqtt

mqtt = Mqtt(app)
mqtt.init_app(app)


@mqtt.on_connect()
def handle_connect(client, userdata, flags, rc):
    mqtt.subscribe('sensores/temperatura')
    mqtt.subscribe('sensores/distancia')
    print('Conectado ao broker')
    
@mqtt.on_message()
def handle_message(client, userdata, message):
    print('Message: ', message.payload.decode())