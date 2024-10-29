from pydantic import BaseModel

class RequestMQTT(BaseModel):
    codigo: int
    name: str 
    value: float