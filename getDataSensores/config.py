import os 
from dotenv import load_dotenv

load_dotenv()

db_user = os.getenv('DB_USER')
db_password = os.getenv('DB_PASSWORD')
db_database = os.getenv('DB_DATABASE')
db_host = os.getenv('DB_HOST')

mqtt_broker = os.getenv('MQTT_BROKER_URL')
print(f'mqtt_broker: {mqtt_broker}')
print(f'db_user: {mqtt_broker}')
print(f'db_password: {mqtt_broker}')
print(f'db_host: {mqtt_broker}')

URL_CONNECTION_MYSQL = f'mysql://{db_user}:{db_password}@{db_host}/{db_database}'

class Config:
    MQTT_BROKER_URL = mqtt_broker
    MQTT_BROKER_PORT = 1883
    MQTT_USERNAME = ''
    MQTT_PASSWORD = ''
    MQTT_TLS_ENABLED = False
    SQLALCHEMY_DATABASE_URI = URL_CONNECTION_MYSQL