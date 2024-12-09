# CONTROLE DE SENSORES COM MICROSERVIÇOS

- 1) Para rodar a aplicação, primero iremos precisar subir o serviço do mosquitto manualmente.
    - Para tal, primeiro configure seu endereço de IP:
        Altere o arquivo  getDataSensores/mos2/mosquitto.conf na linha 666 'bind_address 192.168.0.1' para seu respectivo endereço de IP
    - Logo em seguida, altere o docker-compose.yaml na linha 67, a variável de ambiente MQTT_BROKER_URL para o IP incluso no arquivo de configuração. 
    - Feito isso, precisaremos subir o serviço com os seguintes passo:
        - Rode no terminal o seguinte comando, dentro da pasta mos2: mosquitto.exe -v -c mosquitto.conf


- 2) Com o serviço do mosquitto rodando, basta apenas subir o docker compose. 
    - Na raiz do projeto, rode o seguinte codigo: docker-compose up --build
 

Infos sobre as requisições:

Serviço de autenticação (Node.js):
    Porta: 8080
    Solicitar token: '/api/v1/auth', passando como parametro no body email e password
    Consultar via email: '/api/v1/user/email/:email' passando o email na requisição
    Consultar via nome: '/api/v1/user/name/:name' passando o nome na requisição

Serviço de exibição de logs (React.js)
    Porta: 3000

Serviço de visualização e inserção de dados (Flask)
    Porta: 5000
    Inserir dados via HTTP
        Temperatura: 'api/v1/insert-data/temperatura' passando como parametro no body codigo, name, value
        Distancia: 'api/v1/insert-data/distancia' passando como parametro no body codigo, name, value
    Visualizar dados
        Temperatura: '/api/v1/get-all-data/temperatura'
        Distancia: '/api/v1/get-all-data/distancia'
