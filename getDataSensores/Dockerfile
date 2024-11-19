FROM python:3.11-bullseye

# Instalar mysqlclient
RUN pip install --no-cache-dir mysqlclient

# Definir o diretório de trabalho
WORKDIR /app

# Copiar o arquivo requirements.txt
COPY requirements.txt requirements.txt

# Instalar as dependências Python
RUN pip install -r requirements.txt

# Copiar o restante do código da aplicação
COPY . .

# Comando para iniciar o aplicativo
CMD ["python", "main.py"]
