import os 
from dotenv import load_dotenv

load_dotenv()

db_user = os.getenv('DB_USER')
db_password = os.getenv('DB_PASSWORD')
db_database = os.getenv('DB_DATABASE')
db_host = os.getenv('DB_HOST')


URL_CONNECTION_MYSQL = f'mysql://{db_user}:{db_password}@{db_host}/{db_database}'

class Config:
    MQTT_BROKER_URL = '192.168.0.111'
    MQTT_BROKER_PORT = 1883
    MQTT_USERNAME = ''
    MQTT_PASSWORD = ''
    MQTT_TLS_ENABLED = False
    SQLALCHEMY_DATABASE_URI = URL_CONNECTION_MYSQL