
from flask import Flask
from flask_mqtt import Mqtt
from flask_socketio import SocketIO
from src.module.user.model.sensores_model import db
from src.module.user.routes.events_mqtt import *
from flask_cors import CORS
app = Flask(__name__)
CORS(app)
app.config.from_object('config.Config')

db.init_app(app)

with app.app_context():
    db.create_all()
    
from src.module.user.routes.sensor_route import routes
app.register_blueprint(routes)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)