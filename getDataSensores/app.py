from paho.mqtt import client

#criando uma função de callback para quando
#conectarmos ao broker
def conectar(client, userdata, flags, rc):
    print("Conectado ao broker!")

#criando uma função de callback para quando recebermos 
#mensagens do broker
def recebimento_msg(client, userdata, msg):
    mensagem = msg.payload.decode()
    print(mensagem)

    if mensagem == 's':
        #cancelando a assinatura do tópico temperatura
        client.unsubscribe("temperatura")

#essa função é de callback para um tópico específico
def recebimento_umidade(client, userdata, msg):
    mensagem = msg.payload.decode()
    topico = msg.topic
    print(mensagem)

#criamos o cliente MQTT
cliente_sub = client.Client(client_id="client_id")

cliente_sub.on_connect = conectar

cliente_sub.message_callback_add("primeiro_sensor", recebimento_umidade)
cliente_sub.message_callback_add("segundo_sensor", recebimento_msg)

cliente_sub.connect("192.168.0.111", 1883)
cliente_sub.subscribe("log")
cliente_sub.subscribe("primeiro_sensor")
cliente_sub.subscribe("segundo_sensor")

cliente_sub.loop_forever()