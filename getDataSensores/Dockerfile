# Usamos uma imagem base oficial do Python
FROM python:3.12-slim


# Instalamos o mysqlclient
RUN pip install mysqlclient

COPY requirements.txt requirements.txt

RUN pip install -r requirements.txt

# Copiamos o código da aplicação para o container
COPY . /app

# Definimos o diretório de trabalho
WORKDIR /app

# Exemplo de comando para executar a aplicação
CMD ["python", "main.py"]
