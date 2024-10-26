from flask_sqlalchemy import SQLAlchemy

db = SQLAlchemy()

class Temperatura(db.Model):
    __tablename__ = 'sensor_temperatura'
    id = db.Column(db.Integer, primary_key = True, autoincrement = True)
    codigo = db.Column(db.Integer, nullable = False)
    name = db.Column(db.String(300), nullable = False)
    value = db.Column(db.Double, nullable = False)
    timestamp = db.Column(db.DateTime, default=db.func.current_timestamp())
    
    
    def __str__(self):
        return f'<-- Sensor de Temperatura: {self.name} -->'
    
    def to_dict(self):
        return {
            'id': self.id,
            'name': self.name,
            'value': self.value,
            'timestamp': self.timestamp
        }
    
class Distancia(db.Model):
    __tablename__ = 'sensor_distancia'
    id = db.Column(db.Integer, primary_key = True, autoincrement = True)
    codigo = db.Column(db.Integer, nullable = False)
    name = db.Column(db.String(300), nullable = False)
    value = db.Column(db.Double, nullable = False)
    timestamp = db.Column(db.DateTime, default=db.func.current_timestamp())


    def __str__(self):
        return f'<-- Sensor de distancia: {self.name} -->'
    
    def to_dict(self):
        return {
            'id': self.id,
            'name': self.name,
            'value': self.value,
            'timestamp': self.timestamp
        }
    
    