from flask import jsonify, Blueprint, request, json

routes = Blueprint("routes", __name__)

@routes.route('/')
def home():
    return jsonify({
        'Api':"getDataSensore",
        'status':'up'
    })