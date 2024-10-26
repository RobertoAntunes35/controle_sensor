import json
import paho.mqtt.client as mqtt

def publish_message(broker_address, topic, message):
    """
    Publica uma mensagem JSON em um tópico MQTT.

    :param broker_address: Endereço do broker MQTT.
    :param topic: Tópico onde a mensagem será publicada.
    :param message: Dicionário a ser convertido em JSON e publicado.
    """
    # Converte o dicionário para uma string JSON
    json_message = json.dumps(message)

    # Cria um cliente MQTT
    client = mqtt.Client()

    # Conecta ao broker MQTT
    try:
        client.connect(broker_address)
        print(f"Conectado ao broker {broker_address}")

        # Publica a mensagem no tópico especificado
        client.publish(topic, json_message)
        print(f"Mensagem publicada no tópico '{topic}': {json_message}")

    except Exception as e:
        print(f"Erro ao publicar mensagem: {e}")

    finally:
        # Desconecta do broker
        client.disconnect()

# Exemplo de uso
if __name__ == "__main__":
    broker = "192.168.0.111"
    topic = "sensores/temperatura"
    message_data = {
        "codigo": 1,
        "name": "sensor_temperatura",
        "value": 15.10
    }

    publish_message(broker, topic, message_data)