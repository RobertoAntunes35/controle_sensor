#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Keypad_I2C.h>
#include <Keypad.h>
#include <Wire.h>  // Biblioteca utilizada para fazer a comunicação com o I2C
#include <LiquidCrystal_I2C.h>
#include <stdio.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <UltrasonicSensor.h>
#include <stdio.h>

UltrasonicSensor ultrasonic(13, 14);

// botão referencia
// =================== BOTAO REFERENCIA ===================
const int botaoPin = 2;
int estadoBotao = 0;
int ultimoEstadoBotao = 0;

unsigned long tempoInicio = 0;  // Variável para armazenar o tempo de início da pressão do botão
bool botaoPressionado = false;  // Variável para armazenar o tempo total pressionado
bool estadoConfiguracao = false;
// =================== FIM ===================


// =================== BOTAO SENSOR 01 ===================
const int botaoSensor01 = 12;
int estadoBotaoSensor01 = 0;
// =================== FIM ===================

// =================== BUZZER ===================
#define pinoBuzzer 25

// =================== BOTAO SENSOR 02 ===================
const int botaoSensor02 = 13;
int estadoBotaoSensor02 = 0;
// =================== FIM ===================


// =================== BOTAO SELECT ===================
const int botaoSelect = 11;
int estadoBotaoSelect = 0;
// =================== FIM ===================


// =================== BOTAO VOLTAR, RETIRAR ? ===================
const int botaoBack = 10;
// =================== FIM ===================


// =================== POTENCIOMETRO GERAL ===================
#define potenciometro 34
int valorMedidoPotenciomento = 0;
int valorConvertidoPotenciometro = 0;
// =================== FIM ===================

// =================== POTENCIOMETRO X ===================
#define potenciometro1 36
int valorMedidoPotenciomento1 = 0;
int valorConvertidoPotenciometro1 = 0;
// =================== FIM ===================


// =================== ESTADOS ===================
// sensorSelecionado
// 0 SENSOR 1
// 1 SENSOR 2
bool sensores[2] = { false, false };
bool passos[10] = { false, false, false, false, false, false, false, false, false, false };
int quantidadeDados;

int currentPass = 0;
long int quantidadeDadosFinal;
int sensorSelecionadoFinal;


// =================== FIM ===================

// =================== UNIDADE DE MEDIDAS ===================
const char* listaUnidades[] = { "mm", "cm", "m", "C", "K", "F" };
const int indexLista[] = { 0, 1, 2, 3, 4, 5 };
// =================== FIM ===================


// =================== LISTA DE SENSORES ===================
const char* listaSensorSelecionado[] = { "SENSOR 1", "SENSOR 2" };
// =================== FIM ===================

// =================== VALORES PARA APLICAR VALOR CORRECAOREGRESSAO LINEAR ===================
int sensorSelecionado;

float erroSensor1 = 0;
float mAngularSensor1 = 1;
float bLinearSensor1 = 0;

float erroSensor2 = 0;
float mAngularSensor2 = 1;
float bLinearSensor2 = 0;

float aplicaRegressaoLinear(float dadoSensor, float mAngular, float bLinear) {
  return (dadoSensor * mAngular) + bLinear;
}
// =================== FIM ===================


// =================== DISPLAY LCD 16X2 ===================

#define col 16     // Serve para definir o numero de colunas do display utilizado
#define lin 2      // Serve para definir o numero de linhas do display utilizado
#define ende 0x27  // Serve para definir o endereço do display.
LiquidCrystal_I2C lcd(ende, col, lin);
// =================== FIM ===================

// =================== WIFI ===================
// Configurações WiFi
const char* ssid = "Login";  // Nome da sua rede WiFi
const char* password = "123";     // Senha da rede WiFi

// Configurações MQTT
const char* mqtt_server = "192.168.0.1";                     // endereçõ de ip do mosquitto
const char* topic_log_primeiro_sensor = "sensores/distancia";  // topico primeiro sensor
const char* topic_log_segundo_sensor = "sensores/movimento";   // topico segundo sensor
const char* topic_log = "log";                                 // topico para log de conexão

WiFiClient espClient;
PubSubClient client(espClient);
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

// realiza a conexão com o mqtt
void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    Serial.println("Conectando MQTT");
    if (client.connect("client_esp32")) {
      Serial.println("Conectado Ao Mosquitto!");
      client.publish(topic_log, "A conexão do esp32 com o mosquitto foi estabelecida com sucesso !");  // Publica no tópico
    } else {
      Serial.print("Falhou, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

//

Adafruit_MPU6050 mpu;




// =================== LEDS ===================
const int ledPin1 = 3;
const int ledPin2 = 4;
// =================== FIM ===================


// =================== TECLADO ===================
// número de linhas no teclado
const byte linhas = 4;
// número de colunas no teclado
const byte colunas = 4;

// array que armazena as teclas (Mapeação de teclas)
char keys[linhas][colunas] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

#define enderecoTeclado 0x26

// Definição dos pinos do PCF8574
byte rowPins[linhas] = { 0, 1, 2, 3 };   // conecta-se aos pinos de linha do teclado
byte colPins[colunas] = { 4, 5, 6, 7 };  // conecta-se aos pinos de coluna do teclado

Keypad_I2C teclado(makeKeymap(keys), rowPins, colPins, linhas, colunas, enderecoTeclado, PCF8574);
// =================== FIM ===================


// =================== TELAS ===================
void montaTelaConfiguracaoSelecionada(int v) {
  lcd.setCursor(0, 0);
  lcd.print("Selecione U.N:");
  lcd.setCursor(0, 1);
  lcd.print(listaUnidades[v]);
}
void montaTelaUnidadeSelecionada(int v) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("U.N. Selecionada");
  lcd.setCursor(0, 1);
  lcd.print(listaUnidades[v]);
  delay(500);
}
void montaTelaConfiguracaoSelecionada() {
  Serial.println("Configuracao Iniciada");
  limpaTela();
  lcd.setCursor(0, 0);
  lcd.print("Configuracao");
  lcd.setCursor(0, 1);
  lcd.print("Iniciada");
}
void montaTelaInicial() {
  valorConvertidoPotenciometro = map(valorMedidoPotenciomento, 0, 1023, 0, 5);
  lcd.setCursor(0, 0);
  lcd.print("A-SEN1 | B-SENS2");
  lcd.setCursor(0, 1);
  lcd.print("C-CONF  SENSORES");
}
void montaTelaBlank() {
  Serial.println("called montaTelaClear()");
  lcd.clear();
  delay(1000);
}
void limpaTela() {
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
}
int montaTelaSelecaoSensor() {
  Serial.println("Selecao Sensor");
  limpaTela();
  lcd.setCursor(0, 0);
  lcd.print("Selecione um ");
  lcd.setCursor(0, 1);
  lcd.print("sensor: 01 | 02");
  while (1) {
    valorConvertidoPotenciometro = map(valorMedidoPotenciomento, 0, 1023, 0, 5);
    char tecla = teclado.getKey();

    if (tecla) {
      feedbackSomClique();
      if (tecla == 'A') {
        feedbackConfirmacao();
        return 0;
      } else if (tecla == 'B') {
        feedbackConfirmacao();
        return 1;
      }
    }
  }
}
void montaTelaSensorSelecionado(int indexListaSensor) {
  limpaTela();
  lcd.setCursor(0, 0);
  lcd.print("SELECIONADO");
  lcd.setCursor(0, 1);
  lcd.print(listaSensorSelecionado[indexListaSensor]);
  delay(2000);
}
void montaTelaSensor01Selecionado() {
  Serial.println("Sensor 01 selecionado");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sensor");
  lcd.setCursor(0, 1);
  lcd.print("selecionado: 01");
}
void montaTelaoValorSensor(float valorSensor, int sensorSelecionado, int valorUnidade) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(listaSensorSelecionado[sensorSelecionado]);
  lcd.setCursor(0, 1);
  lcd.print(valorSensor);
  lcd.setCursor(7, 1);
  lcd.print(listaUnidades[valorUnidade]);
  delay(200);
}
void montaTelaValorSelecionado(float value, int unidade) {
  Serial.println("Sensor 01 selecionado");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Valor: ");
  lcd.setCursor(6, 0);
  lcd.print(value);
  lcd.setCursor(0, 1);
  lcd.print("Unidade: ");
  lcd.setCursor(8, 1);
  lcd.print(listaUnidades[unidade]);
  delay(500);
}
long int montaTelaQuantidadeDadosASeremPegos(int value) {
  limpaTela();
  String quantidadeDados = "";

  if (value > 0) {
    Serial.print("Entrou AQUI - Qnt: ");
    Serial.println(value);
    lcd.setCursor(0, 0);
    lcd.print("Quantidade de");
    lcd.setCursor(0, 1);
    lcd.print("dados: ");
    lcd.setCursor(6, 1);
    lcd.print(value);
    return 0;
  } else {

    lcd.setCursor(0, 0);
    lcd.print("Quantidade de");
    lcd.setCursor(0, 1);
    lcd.print("dados: ");
    lcd.setCursor(6, 1);
    while (true) {
      char tecla = teclado.getKey();

      if (tecla) {
        feedbackSomClique();

        if (tecla == '1' || tecla == '2' || tecla == '3' || tecla == 'A') {
          // se alguma tecla da linha 1 do teclado for apertada, temos que fazer debounce, pois se não o microcontrolador irá contabilizar vários cliques. Isso só ocorre na linha 1.
          delay(250);
        }


        if (tecla == '*') {
          quantidadeDados.remove(quantidadeDados.length() - 1);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Quantidade de");
          lcd.setCursor(0, 1);
          lcd.print("dados: ");
          lcd.setCursor(6, 1);
          lcd.print(quantidadeDados);
        }

        else if (tecla == '#') {
          Serial.print("Quantidade Dados 1: ");
          Serial.println(quantidadeDados);
          feedbackConfirmacao();
          return quantidadeDados.toInt();
        }

        else if (!isAlpha(tecla)) {
          quantidadeDados += tecla;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Quantidade de");
          lcd.setCursor(0, 1);
          lcd.print("dados: ");
          lcd.setCursor(6, 1);
          lcd.print(quantidadeDados);
        }
      }
    }
  }
}
// =================== FIM ===================



// =================== CONVERSÃO ===================
float realizaConversao(float valorSelecionado, int valueSelect) {

  if (valueSelect == 0) {
    return conversaoMilimitroParaMetro(valorSelecionado);
  } else if (valueSelect == 1) {
    return conversaoCmParaMetro(valorSelecionado);
  } else if (valueSelect == 2) {
    return valorSelecionado;
  } else if (valueSelect == 3) {
    return conversaoCelsiusParaKelvin(valorSelecionado);
  } else if (valueSelect == 4) {
    return valorSelecionado;
  } else if (valueSelect == 5) {
    return conversaoFahrenheitParaKelvin(valorSelecionado);
  }
}

float conversaoMilimitroParaMetro(float value) {
  return value / 1000;
}

float conversaoCmParaMetro(float value) {
  return value / 100;
}

float conversaoCelsiusParaKelvin(float value) {

  const float vKelvin = 273.13;
  return value + vKelvin;
}

float conversaoFahrenheitParaKelvin(float value) {
  float first = ((value - 32) * 5) / 9;
  return first + 273.13;
}

int realizaConversaoUnidade(int unidade) {
  if (unidade == 0 || unidade == 1 || unidade == 2) {
    return 2;
  } else {
    return 4;
  }
}

float realizaConversaoCmParaDemais(int unidade, float value) {
  // aqui, temos que converter de cm para m, para mm e proprio cm
  if (unidade == 0) {
    return value * 10;
  } else if (unidade == 1) {
    return value;
  } else if (unidade == 2) {
    return value / 100;
  }
}
// =================== FIM ===================


// =================== ESTRUTURA DE DADOS PARA CRIAÇÃO DE UMA LISTA ===================
struct Node {
  float data;         // Dado do nó (valor float agora)
  struct Node* next;  // Ponteiro para o próximo nó
};

// Função para adicionar um novo nó no início da lista
void push(struct Node** head_ref, float new_data) {

  // Alocar memória para o novo nó
  struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));

  // Atribuir o dado (valor float)
  new_node->data = new_data;

  // Conectar o novo nó no início da lista
  new_node->next = (*head_ref);

  // Atualizar a cabeça da lista para o novo nó
  (*head_ref) = new_node;
}

// Função para imprimir a lista ligada
void printList(struct Node* node) {
  while (node != NULL) {
    Serial.println(node->data);  // Agora imprimimos floats
    node = node->next;
  }
}

// Função para calcular o tamanho da lista ligada
int size(struct Node* node) {
  int count = 0;
  while (node != NULL) {
    count++;
    node = node->next;
  }
  return count;
}

void deleteList(struct Node** head_ref) {
  struct Node* current = *head_ref;  // Aponta para o início da lista
  struct Node* next;                 // Usado para armazenar o próximo nó

  // Itera pela lista
  while (current != NULL) {
    next = current->next;  // Salva o próximo nó
    free(current);         // Libera o nó atual
    current = next;        // Move para o próximo nó
  }

  // Garante que a lista esteja vazia
  *head_ref = NULL;
}
// =================== FIM ===================


// =================== FUNÇÕES PARA REALIZAR CALCULOS DA REGRESSAO LINEAR ===================
float somatoria(struct Node* node) {
  float valueTotal = 0.00;
  while (node != NULL) {
    valueTotal = valueTotal + node->data;
    node = node->next;
  }
  return valueTotal;
}

float somatoriaYmedYref(struct Node* node1, struct Node* node2) {
  float valorTotal = 0.00;
  while (node1 != NULL) {
    valorTotal = valorTotal + (node1->data * node2->data);
    node1 = node1->next;
    node2 = node2->next;
  }
  return valorTotal;
}

float yMedQuadrado(struct Node* node) {
  float valorTotal = 0.00;
  while (node != NULL) {
    valorTotal = valorTotal + (node->data * node->data);
    node = node->next;
  }
  return valorTotal;
}
// =================== FIM ===================
int valorUnidade = -1;
void configuraUnidadeMedida() {
  limpaTela();
  while (true) {
    valorConvertidoPotenciometro = map(valorMedidoPotenciomento, 0, 1023, 0, 5);
    char tecla = teclado.getKey();

    Serial.print("Valor potenciomento: ");
    Serial.print(valorMedidoPotenciomento);

    Serial.print(" Valor Convertido: ");
    Serial.print(valorConvertidoPotenciometro);
    Serial.println();
    valorConvertidoPotenciometro = 1;
    montaTelaConfiguracaoSelecionada(valorConvertidoPotenciometro);
    if (tecla) {
      feedbackSomClique();

      if (tecla == '1' || tecla == '2' || tecla == '3' || tecla == 'A') {
        delay(250);
      }
      if (tecla == '#') {
        limpaTela();
        montaTelaUnidadeSelecionada(valorConvertidoPotenciometro);
        // valorUnidade = valorConvertidoPotenciometro;
        valorUnidade = 1;
        feedbackConfirmacao();
        return;
      }
    }
  }
}



// =================== LISTA DE INFORMAÇÕES PARA PEGAR DADOS DE ENTRADA E MEDIDO ===================
struct Node* listaYRef = NULL;
struct Node* listaSensor1 = NULL;


struct Node* listaAxRef = NULL;
struct Node* listaAxMed = NULL;

struct Node* listaAyRef = NULL;
struct Node* listaAyMed = NULL;

struct Node* listaAzRef = NULL;
struct Node* listaAzMed = NULL;


void montaTelaInsercaoValorSensor(int sensorSelecionado, int valorUnidade, int quantidadeDadosTotal) {
  limpaTela();
  Serial.println("Entrou aqui monta insercao valor sensor");
  int sizeItems = 0;
  float aX, aY, aZ;

  if (sensorSelecionado == 1) {
    lcd.setCursor(14, 0);
    lcd.print("S2");
    lcd.setCursor(10, 0);
    lcd.print(sizeItems);
    lcd.setCursor(12, 0);
    lcd.print("/");
    lcd.setCursor(13, 0);
    lcd.print(quantidadeDadosTotal);
  }

  while (true) {
    char tecla = teclado.getKey();
    float valorSelecionado;
    float valorSensor = realizaConversaoCmParaDemais(valorUnidade, ultrasonic.read_data_sensor());
    Serial.println(tecla);



    if (tecla) {
      feedbackSomClique();
      Serial.print("SENSOR SELECIONADO: ");
      Serial.println(sensorSelecionado);
      if (tecla == '#') {
        if (sensorSelecionado == 0) {
          valorSelecionado = realizaConversao(valorSensor, valorUnidade);
          push(&listaSensor1, valorSelecionado);
          sizeItems = size(listaSensor1);
        }

        // else if (sensorSelecionado == 1) {
        //   push(&listaAxMed, aX);
        //   push(&listaAyMed, aY);
        //   push(&listaAzMed, aZ);
        //   sizeItems = size(listaAxMed);
        //   lcd.setCursor(10, 0);
        //   lcd.print(sizeItems);
        // }

        //push(&listaSensor1, valorSelecionado);
        //montaTelaInsercaoValorSensor(realizaConversaoCmParaDemais(valorUnidade, valorSensorUltrassonico), sensorSelecionado, valorUnidade, size(listaSensor1), quantidadeDados);

        //int sizeItems = size(listaSensor1);

        Serial.print("sizeItems: ");
        Serial.print(sizeItems);
        Serial.print(" Quantidade Itens: ");
        Serial.print(quantidadeDadosTotal);
        Serial.println();
        feedbackConfirmacao();
      }
    }

    if (sensorSelecionado == 0) {
      lcd.setCursor(0, 0);
      lcd.print(listaSensorSelecionado[sensorSelecionado]);
      lcd.setCursor(10, 0);
      lcd.print(sizeItems);
      lcd.setCursor(12, 0);
      lcd.print("/");
      lcd.setCursor(13, 0);
      lcd.print(quantidadeDadosTotal);
      lcd.setCursor(0, 1);
      lcd.print(valorSensor);
      lcd.setCursor(7, 1);
      lcd.print(listaUnidades[valorUnidade]);
    }
    // else if (sensorSelecionado == 1) {
    //   if (mpu.getMotionInterruptStatus()) {
    //     /* Get new sensor events with the readings */


    //     Serial.print("SIZE ITENS: ");
    //     Serial.println(sizeItems);
    //     sensors_event_t a, g, temp;
    //     mpu.getEvent(&a, &g, &temp);
    //     aX = a.acceleration.x;
    //     aY = a.acceleration.y;
    //     aZ = a.acceleration.z;

    //     /* Print out the values */
    //     Serial.print("AccelX:");
    //     Serial.print(a.acceleration.x);
    //     Serial.print(",");
    //     Serial.print("AccelY:");
    //     Serial.print(a.acceleration.y);
    //     Serial.print(",");
    //     Serial.print("AccelZ:");
    //     Serial.print(a.acceleration.z);
    //     Serial.print(", ");
    //     Serial.print("GyroX:");
    //     Serial.print(g.gyro.x);
    //     Serial.print(",");
    //     Serial.print("GyroY:");
    //     Serial.print(g.gyro.y);
    //     Serial.print(",");
    //     Serial.print("GyroZ:");
    //     Serial.print(g.gyro.z);
    //     Serial.println("");

    //     lcd.setCursor(0, 0);
    //     lcd.print("Ax:");
    //     lcd.setCursor(3, 0);
    //     lcd.print(aX);

    //     lcd.setCursor(0, 1);
    //     lcd.print("aY: ");
    //     lcd.setCursor(3, 1);
    //     lcd.print(aY);

    //     lcd.setCursor(8, 1);
    //     lcd.print("aZ:");
    //     lcd.setCursor(11, 1);
    //     lcd.print(aZ);
    //   }
    // }

    if (sizeItems == quantidadeDadosTotal) {
      return;
    }
  }
}

// ================================================================================================ PROBLEMA AQUI
void montaTelaInsercaoValorConhecido(int unidadeSelecionada, int quantidadeInclusa, int quantidadeFinal) {


  limpaTela();
  String valorDado = "";
  if (sensorSelecionado == 0) {
    int sizeItems = 0;
    lcd.setCursor(0, 0);
    lcd.print("Valor Conhecido");

    lcd.setCursor(0, 1);
    lcd.print("Val: ");
    lcd.setCursor(4, 1);
    lcd.print(valorDado);
    lcd.setCursor(7, 1);
    lcd.print(listaUnidades[unidadeSelecionada]);
    lcd.setCursor(11, 1);
    lcd.print(sizeItems);
    lcd.setCursor(12, 1);
    lcd.print("/");
    lcd.setCursor(13, 1);
    lcd.print(quantidadeFinal);
    while (true) {

      char tecla = teclado.getKey();
      if (tecla) {
        feedbackSomClique();
        if (tecla == '*') {
          valorDado.remove(valorDado.length() - 1);
          lcd.clear();
          lcd.print(valorDado);
        }

        else if (tecla == '#') {
          Serial.println("ENTROU AQUI");
          long int valorDadoConvertido = valorDado.toInt();
          float valorSelecionado = realizaConversao(valorDadoConvertido, valorUnidade);

          push(&listaYRef, valorSelecionado);
          sizeItems = size(listaYRef);
          Serial.print("sizeItems: ");
          Serial.print(sizeItems);
          valorDado.remove(0, valorDado.length());
          limpaTela();
          lcd.setCursor(0, 0);
          lcd.print("Valor Conhecido");
          feedbackConfirmacao();
        }

        else if (!isAlpha(tecla)) {
          valorDado += tecla;
        }

        lcd.setCursor(0, 1);
        lcd.print("Val: ");
        lcd.setCursor(4, 1);
        lcd.print(valorDado);
        lcd.setCursor(7, 1);
        lcd.print(listaUnidades[unidadeSelecionada]);
        lcd.setCursor(11, 1);
        Serial.print("SIZE ");
        Serial.println(sizeItems);
        lcd.print(sizeItems);
        lcd.setCursor(12, 1);
        lcd.print("/");
        lcd.setCursor(13, 1);
        lcd.print(quantidadeFinal);
      }



      if (sizeItems == quantidadeFinal) {
        printList(listaYRef);

        return;
      }
    }
  }
  // else if (sensorSelecionado == 1) {
  //   int sizeItems = 0;
  //   int quantidadeDados = 0;
  //   int passo = 0;
  //   lcd.setCursor(0, 0);
  //   lcd.print("Valor Conhecido");
  //   lcd.setCursor(0, 1);
  //   if (passo == 0) {
  //     lcd.print("Ax: ");
  //   } else if (passo == 1) {
  //     lcd.print("Ay: ");
  //   } else if (passo == 2) {
  //     lcd.print("Az: ");
  //   }
  //   lcd.setCursor(3, 1);
  //   lcd.print(valorDado);
  //   lcd.setCursor(7, 1);
  //   lcd.print(listaUnidades[unidadeSelecionada]);
  //   lcd.setCursor(11, 1);
  //   Serial.print("SIZE ");
  //   Serial.println(sizeItems);
  //   lcd.print(sizeItems);
  //   lcd.setCursor(12, 1);
  //   lcd.print("/");
  //   lcd.setCursor(13, 1);
  //   lcd.print(quantidadeFinal);



  //   while (true) {

  //     char tecla = teclado.getKey();
  //     if (tecla) {
  //       feedbackSomClique();
  //       if (tecla == '*') {
  //         valorDado.remove(valorDado.length() - 1);
  //         lcd.clear();
  //         lcd.print(valorDado);
  //       }

  //       else if (tecla == '#') {
  //         Serial.println("ENTROU AQUI");
  //         long int valorDadoConvertido = valorDado.toInt();
  //         float valorSelecionado = realizaConversao(valorDadoConvertido, valorUnidade);

  //         if (passo == 0) {
  //           // Ax
  //           push(&listaAxRef, valorSelecionado);
  //           passo = 1;
  //         } else if (passo == 1) {
  //           // Ay
  //           push(&listaAyRef, valorSelecionado);
  //           passo = 2;
  //         } else if (passo == 2) {
  //           // Az
  //           push(&listaAzRef, valorSelecionado);
  //           passo = 3;
  //         } else if (passo == 3) {
  //           quantidadeDados = quantidadeDados + 1;
  //           passo = 0;
  //         }
  //         sizeItems = size(listaAxRef);
  //         Serial.print("sizeItems: ");
  //         Serial.print(quantidadeDados);
  //         valorDado.remove(0, valorDado.length());
  //         limpaTela();
  //         lcd.setCursor(0, 0);
  //         lcd.print("Valor Conhecido");
  //         feedbackConfirmacao();
  //       }

  //       else if (!isAlpha(tecla)) {
  //         valorDado += tecla;
  //       }

  //       lcd.setCursor(0, 1);
  //       if (passo == 0) {
  //         lcd.print("Ax: ");
  //       } else if (passo == 1) {
  //         lcd.print("Ay: ");
  //       } else if (passo == 2) {
  //         lcd.print("Az: ");
  //       }


  //       lcd.setCursor(3, 1);
  //       lcd.print(valorDado);
  //       lcd.setCursor(7, 1);
  //       lcd.print(listaUnidades[unidadeSelecionada]);
  //       lcd.setCursor(11, 1);
  //       Serial.print("SIZE ");
  //       Serial.println(sizeItems);
  //       lcd.print(quantidadeDados);
  //       lcd.setCursor(12, 1);
  //       lcd.print("/");
  //       lcd.setCursor(13, 1);
  //       lcd.print(quantidadeFinal);
  //     }
  //     if (quantidadeDados == quantidadeFinal) {
  //       return;
  //     }
  //   }
  // }
}


// =================== FIM ===================



void setup() {
  // PINOS
  pinMode(potenciometro, INPUT);
  pinMode(potenciometro1, INPUT);
  pinMode(pinoBuzzer, OUTPUT);
  digitalWrite(pinoBuzzer, LOW);

  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);

  lcd.begin(21, 22);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Iniciando...");
  delay(3000);
  lcd.clear();


  Serial.println("MPU 6050 TEST");
  lcd.setCursor(0, 0);
  lcd.print("MPU 6050 TEST");

  lcd.setCursor(0, 1);
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    lcd.print("MPU FAILED FOUND");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU 6050 FOUND!");
  lcd.print("MPU 6050 FOUND!");
  delay(1000);

  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu.setMotionDetectionThreshold(1);
  mpu.setMotionDetectionDuration(20);
  mpu.setInterruptPinLatch(true);  // Keep it latched.  Will turn off when reinitialized.
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);

  Serial.println("");
  Wire.begin();
  teclado.begin(makeKeymap(keys));

  // =================== CONFIGURAÇÃO E PRIMEIRAS INFORMAÇÕES DO DISPLAY LCD ===================


  Serial.println("Project Display LCD setup");



  // lcd.setCursor(0, 0);
  // lcd.print("Sensor 01 - OK");
  // delay(1000);
  // lcd.setCursor(0, 1);
  // lcd.print("Sensor 02 - OK");
  // delay(2000);
  // lcd.clear();

  // for (int i = 0; i <= 15; i++) {
  //   lcd.setCursor(0, 0);
  //   lcd.print("Carregando...");
  //   lcd.setCursor(i, 1);
  //   lcd.print("*");
  //   delay(200);
  //   lcd.clear();
  // }

  limpaTela();
  // =================== FIM ===================
}


void loop() {
  valorMedidoPotenciomento = analogRead(potenciometro);
  char tecla = teclado.getKey();
  /*
    * Tecla A - Sensor 1 e Sensor 2
    * Tecla B - Transmissão de dados 
    * Tecla D - Iniciar configuração - passo 1 
    * Tecla # - Confirmar valores 
    * Tecla * - Voltar para tela inicial

*/

  if (tecla) {
    feedbackSomClique();
    Serial.print("Teclada apertada: ");
    Serial.println(tecla);

    if (tecla == 'A') {
      feedbackConfirmacao();
    }

    else if (tecla == 'B') {
      feedbackConfirmacao();
      currentPass = 99;
    }

    else if (tecla == 'C') {
      feedbackConfirmacao();
      currentPass = 1;
    }

    else if (tecla == 'D') {
      currentPass = 0;
      feedbackConfirmacao();
    }
  }

  if (currentPass == 0) {
    deleteList(&listaYRef);
    deleteList(&listaSensor1);
    montaTelaInicial();

    quantidadeDadosFinal = 0;
  } else if (currentPass == 1) {
    montaTelaConfiguracaoSelecionada();
    currentPass = 2;
  } else if (currentPass == 2) {
    sensorSelecionado = montaTelaSelecaoSensor();
    Serial.print("Sensor selecionado: ");
    Serial.println(sensorSelecionado);
    currentPass = 3;
  } else if (currentPass == 3) {
    montaTelaSensorSelecionado(sensorSelecionado);
    currentPass = 4;
  } else if (currentPass == 4) {
    configuraUnidadeMedida();
    currentPass = 5;
  } else if (currentPass == 5) {
    quantidadeDadosFinal = montaTelaQuantidadeDadosASeremPegos(0);
    Serial.print("Quantidade Dados Before: ");
    Serial.println(quantidadeDadosFinal);
    currentPass = 6;
  } else if (currentPass == 6) {
    Serial.print("Quantidade Dados 1: ");
    Serial.println(quantidadeDadosFinal);
    montaTelaQuantidadeDadosASeremPegos(quantidadeDadosFinal);
    currentPass = 7;
  } else if (currentPass == 7) {

    if (sensorSelecionado == 0) {
      montaTelaInsercaoValorConhecido(valorUnidade, size(listaYRef), quantidadeDadosFinal);
    }
    // else if (sensorSelecionado == 1) {
    //   montaTelaInsercaoValorConhecido(valorUnidade, size(listaAxRef), quantidadeDadosFinal);
    // }

    currentPass = 8;
  } else if (currentPass == 8) {
    if (sensorSelecionado == 0) {
      montaTelaInsercaoValorSensor(sensorSelecionado, valorUnidade, quantidadeDadosFinal);
    }

    // else if (sensorSelecionado == 1) {
    //   montaTelaInsercaoValorSensor(sensorSelecionado, valorUnidade, quantidadeDadosFinal);
    // }
    currentPass = 9;
  } else if (currentPass == 9) {
    //Regressão Linear
    //1) Calcular somatoria de yReferencia e yMedido = somatoria(yRef) e somatoria(yMed)
    //2) Soma do produto dos valores medidos (yReferencia . yMedido) = somatoriaXY
    //3) Somatoria ao quadrado do valor Medido (yMedido²) = yMedQuadrado()
    if (sensorSelecionado == 0) {
      // 1) Calcular a somatoria de yRef e yMed
      float yReferencia = somatoria(listaYRef);
      float yMedido = somatoria(listaSensor1);

      // 2) somatoria dos produtos yMed . yRef
      float yMedidoXyReferencia = somatoriaYmedYref(listaYRef, listaSensor1);

      // 3) yMedQuadrado
      float yMedidoQuadrado = yMedQuadrado(listaSensor1);

      // 4) somatoria(yMed) * somatoria(yMed)
      float somatoriaYQuadrado = yMedido * yMedido;


      // valores
      Serial.print("yReferencia: ");
      Serial.print(yReferencia);
      Serial.println();

      Serial.print("yMedido: ");
      Serial.print(yMedido);
      Serial.println();

      Serial.print("yMedidoXyReferencia: ");
      Serial.print(yMedidoXyReferencia);
      Serial.println();

      Serial.print("yMedidoQuadrado: ");
      Serial.print(yMedidoQuadrado);
      Serial.println();

      // 4) [somatoriaXY - (somatoria(yRef).somatoria(yMed))] / (yMedQuadrado() - somatoria(yMed)²)
      // 4.1 coeficiente angular
      float mAngular = (yMedidoXyReferencia - (yReferencia * yMedido)) / (yMedidoQuadrado - somatoriaYQuadrado);


      // 4.2 coeficiente linear
      float mXSomatoriaYmedido = mAngular * yMedido;
      int quantidadeDados = size(listaYRef);
      float bLinear = (yReferencia - mXSomatoriaYmedido) / quantidadeDados;

      Serial.print("mAngular: ");
      Serial.print(mAngular);
      Serial.println();

      Serial.print("bLinear: ");
      Serial.print(bLinear, 4);
      Serial.println();

      mAngularSensor1 = mAngular;
      bLinearSensor1 = bLinear;
    }
    // else if (sensorSelecionado == 1) {

    //   // Coleta de dados
    //   float axRef = somatoria(listaAxRef);
    //   float axMed = somatoria(listaAxMed);

    //   float ayRef = somatoria(listaAyRef);
    //   float ayMed = somatoria(listaAyMed);

    //   float azRef = somatoria(listaAzRef);
    //   float azMed = somatoria(listaAzMed);

    //   float somatoriaAxRefAxMed = somatoriaYmedYref(listaAxRef, listaAxMed);
    //   float axMedQuadrado = yMedQuadrado(listaAxMed);
    //   float somatoriaAxMedQuad = axMed * axMed;

    //   float mAngularAx = (somatoriaAxRefAxMed - (axRef * axMed)) / (axMedQuadrado - somatoriaAxMedQuad);
    //   float bLinearAx = (axRef - (mAngularAx * axMed)) / size(listaAxRef);

    //   float somatoriaAyRefAyMed = somatoriaYmedYref(listaAyRef, listaAyMed);
    //   float ayMedQuadrado = yMedQuadrado(listaAyMed);
    //   float somatoriaAyMedQuad = ayMed * ayMed;

    //   float mAngularAy = (somatoriaAyRefAyMed - (ayRef * ayMed)) / (ayMedQuadrado - somatoriaAyMedQuad);
    //   float bLinearAy = (ayRef - (mAngularAy * ayMed)) / size(listaAyRef);

    //   float somatoriaAzRefAzMed = somatoriaYmedYref(listaAzRef, listaAzMed);
    //   float azMedQuadrado = yMedQuadrado(listaAzMed);
    //   float somatoriaAzMedQuad = azMed * azMed;

    //   float mAngularAz = (somatoriaAzRefAzMed - (azRef * azMed)) / (azMedQuadrado - somatoriaAzMedQuad);
    //   float bLinearAz = (azRef - (mAngularAz * azMed)) / size(listaAzRef);

    //   Serial.print("mAngularAx: ");
    //   Serial.print(mAngularAx);
    //   Serial.println();

    //   Serial.print("bLinearAx: ");
    //   Serial.print(bLinearAx);
    //   Serial.println();

    //   Serial.print("mAngularAy: ");
    //   Serial.print(mAngularAy);
    //   Serial.println();

    //   Serial.print("bLinearAy: ");
    //   Serial.print(bLinearAy);
    //   Serial.println();

    //   Serial.print("mAngularAz: ");
    //   Serial.print(mAngularAz);
    //   Serial.println();

    //   Serial.print("bLinearAz: ");
    //   Serial.print(bLinearAz);
    //   Serial.println();
    // }

    currentPass = 99;
  } else if (currentPass == 10) {
  } else if (currentPass == 99) {


    limpaTela();
    lcd.setCursor(0, 0);
    lcd.print("INFO SENSORES");
    int opcaoSelecionada = 0;
    while (1) {
      //TENTA REALIZAR CONEXÃO AO MQTT
      if (!client.connected()) {
        reconnect();
      }

      char tecla = teclado.getKey();
      float floatValorSensor = ultrasonic.read_data_sensor();
      float valorSensorComRegressao = aplicaRegressaoLinear(floatValorSensor, mAngularSensor1, bLinearSensor1);


      // DETECTA O MOVIMENTO DO SENSOR
      int objetoEmMovimento = verificaMovimentoObjeto();

      if (tecla) {
        feedbackSomClique();

        if (tecla == '1') {
          opcaoSelecionada = 1;
        } else if (tecla == '2') {
          opcaoSelecionada = 2;
        } else if (tecla == 'D') {
          currentPass = 0;
          break;
        }
      }

      if (opcaoSelecionada == 1) {
        Serial.print("VALOR SENSOR: ");
        Serial.println(floatValorSensor);
        limpaTela();
        lcd.setCursor(14, 0);
        lcd.print("S1");

        lcd.setCursor(0, 0);
        lcd.print("S/R: ");
        lcd.setCursor(4, 0);
        lcd.print(floatValorSensor);

        lcd.setCursor(0, 1);
        lcd.print("C/R: ");
        lcd.setCursor(4, 1);
        lcd.print(valorSensorComRegressao);
        lcd.setCursor(14, 1);
        lcd.print(listaUnidades[valorUnidade]);
      }
      // else if (opcaoSelecionada == 2) {
      //   Serial.print("VALOR SENSOR: ");
      //   Serial.println(floatValorSensor);
      //   limpaTela();
      //   lcd.setCursor(14, 0);
      //   lcd.print("S2");

      //   lcd.setCursor(0, 0);
      //   lcd.print("S/R: ");
      //   lcd.setCursor(4, 0);
      //   lcd.print(floatValorSensor);

      //   lcd.setCursor(0, 1);
      //   lcd.print("C/R: ");
      //   lcd.setCursor(4, 1);
      //   lcd.print(valorSensorComRegressao);
      //   lcd.setCursor(14, 1);
      //   lcd.print(listaUnidades[valorUnidade]);
      // }

      char messageDistancia[100];
      sprintf(messageDistancia, "codigo,1/name,sensor_distancia/value,%.2f", valorSensorComRegressao);

      char messageMovimento[100];
      sprintf(messageMovimento, "codigo,1/name,sensor_movimento/value,%d", objetoEmMovimento);

      Serial.print("MESSAGE DISTANCIA CM: ");
      Serial.print(floatValorSensor);
      Serial.print(" | MESSAGE OBJETO MOV: ");
      Serial.print(objetoEmMovimento);
      Serial.println();

      client.publish(topic_log_primeiro_sensor, messageDistancia);
      client.publish(topic_log_segundo_sensor, messageMovimento);

      Serial.print("Valor Sem Regressao: ");
      Serial.print(floatValorSensor);
      Serial.print(" | Valor com regressao: ");
      Serial.println(valorSensorComRegressao);

      feedbackSensorAtivado(valorSensorComRegressao, objetoEmMovimento);

      client.loop();
      delay(50);
    }
  }
}

void feedbackSensorAtivado(float valorSensor, int objetoMov) {

  if (valorSensor <= 10 && objetoMov == 1) {
    while (1) {
      char tecla = teclado.getKey();

      digitalWrite(pinoBuzzer, HIGH);
      if (efetuaLogin(valorSensor, objetoMov)) {
        digitalWrite(pinoBuzzer, LOW);
        limpaTela();
        lcd.setCursor(0, 0);
        lcd.print("INFO SENSORES");
        break;
      }
    }
  }
}


int verificaMovimentoObjeto() {
  if (mpu.getMotionInterruptStatus()) {
    /* Get new sensor events with the readings */
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    /* Print out the values */
    Serial.print("AccelX:");
    Serial.print(a.acceleration.x);
    Serial.print(",");
    Serial.print("AccelY:");
    Serial.print(a.acceleration.y);
    Serial.print(",");
    Serial.print("AccelZ:");
    Serial.print(a.acceleration.z);
    Serial.print(", ");
    Serial.print("GyroX:");
    Serial.print(g.gyro.x);
    Serial.print(",");
    Serial.print("GyroY:");
    Serial.print(g.gyro.y);
    Serial.print(",");
    Serial.print("GyroZ:");
    Serial.print(g.gyro.z);
    Serial.println("");
    return 1;
  } else {
    return 0;
  }
}


// Variáveis globais para armazenar os dados do callback
bool mensagemRecebida = false;
String senhaRecebida;

// Função callback global para processar mensagens recebidas
void callback(char* topic, byte* payload, unsigned int length) {
  if (String(topic) == "password") {
    char mensagem[length + 1];
    memcpy(mensagem, payload, length);
    mensagem[length] = '\0';           // Adiciona terminador de string
    senhaRecebida = String(mensagem);  // Salva a mensagem recebida
    mensagemRecebida = true;           // Indica que uma mensagem foi recebida
  }
}

bool efetuaLogin(float valorSensor, int objetoMov) {

  lcd.setCursor(0, 0);
  lcd.print("AGUARDANDO:   ");
  lcd.setCursor(0, 1);
  lcd.print("SENHA...");
  String senha = "456";
  String senhaEscondidaExibicao;
  int numeroTentativas = 0;
  int numeroMaximoTentativas = 3;

  client.setCallback(callback);
  client.subscribe("password");

  while (true) {

    client.loop();
    // int objetoEmMovimento = verificaMovimentoObjeto();
    // float valorSensorDistancia = ultrasonic.read_data_sensor();

    // char messageDistancia[100];
    // sprintf(messageDistancia, "codigo,1/name,sensor_distancia/value,%.2f", valorSensorDistancia);

    // char messageMovimento[100];
    // sprintf(messageMovimento, "codigo,1/name,sensor_movimento/value,1", objetoEmMovimento);

    // client.publish(topic_log_primeiro_sensor, messageDistancia);
    // client.publish(topic_log_segundo_sensor, messageMovimento);
    Serial.println(senhaRecebida);

    if (mensagemRecebida) {
      mensagemRecebida = false;
      if (senhaRecebida == senha) {
        feedbackSomLoginBemSucedido();
        return true;
      } else {
        lcd.noBlink();
        numeroTentativas++;
        senhaRecebida = "";
        senhaEscondidaExibicao = "";
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Senha Incorreta ");
        lcd.setCursor(0, 1);
        lcd.print("Tentativa ");
        lcd.print(numeroTentativas);
        lcd.print(" de ");
        lcd.print(numeroMaximoTentativas);
        feedbackSomFalhaLogin();  // efetua o som de feedback de login falhado
        delay(4000);
        lcd.clear();
        if (numeroTentativas >= numeroMaximoTentativas) {  // testa se o número de tentativas for igual ou superior ao número máximo de tentativas
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Tentat. Zeradas ");
          lcd.setCursor(0, 1);
          lcd.print("Entre novamente ");
          feedbackSomTentativasZeradas();
          lcd.clear();
          return false;  // caso o usuário chegue ou ultrapasse o número máximo de tentativas, retorna false para a chamada da função
        }
      }
    }
  }

  // lcd.blink();
  // char tecla = teclado.getKey();



  // if (tecla) {
  //   feedbackSomClique();
  //   if (tecla == '#') {
  //     if (senhaInserida == senha) {
  //       lcd.noBlink();
  //       feedbackSomLoginBemSucedido();
  //       return true;
  //     } else {
  //       lcd.noBlink();
  //       numeroTentativas++;
  //       senhaInserida = "";
  //       senhaEscondidaExibicao = "";
  //       lcd.clear();
  //       lcd.setCursor(0, 0);
  //       lcd.print("Senha Incorreta ");
  //       lcd.setCursor(0, 1);
  //       lcd.print("Tentativa ");
  //       lcd.print(numeroTentativas);
  //       lcd.print(" de ");
  //       lcd.print(numeroMaximoTentativas);
  //       feedbackSomFalhaLogin();  // efetua o som de feedback de login falhado
  //       lcd.clear();
  //       if (numeroTentativas >= numeroMaximoTentativas) {  // testa se o número de tentativas for igual ou superior ao número máximo de tentativas
  //         lcd.clear();
  //         lcd.setCursor(0, 0);
  //         lcd.print("Tentat. Zeradas ");
  //         lcd.setCursor(0, 1);
  //         lcd.print("Entre novamente ");
  //         feedbackSomTentativasZeradas();
  //         lcd.clear();
  //         return false;  // caso o usuário chegue ou ultrapasse o número máximo de tentativas, retorna false para a chamada da função
  //       }
  //     }
  //   } else if (tecla == '*') {
  //     senhaInserida.remove(senhaInserida.length() - 1);
  //     senhaEscondidaExibicao.remove(senhaEscondidaExibicao.length() - 1);  // também remove o último caractere da váriavel senhaEscondidaExibicao
  //     lcd.clear();
  //     lcd.print(senhaEscondidaExibicao);
  //   } else if (tecla == 'B') {            // se a tecla pressionada foi a B (SAIR), ...
  //     return false;                       // retorna false para a chamada da função
  //   } else if (!isAlpha(tecla)) {         // se a tecla pressionada não for nenhuma letra, ...
  //     senhaInserida += tecla;             // adiciona a tecla pressionada para a váriavel senhaInserida
  //     senhaEscondidaExibicao += '*';      // adiciona o caractere '*' para a váriavel senhaEscondidaExibicao
  //     lcd.clear();                        // limpa o display
  //     lcd.print(senhaEscondidaExibicao);  // mostra a váriavel senhaEscondidaExibicao
  //   }
  // }
}


// função de toque do buzzer para feedback do aperto da tecla
void feedbackSomClique() {
  digitalWrite(pinoBuzzer, HIGH);
  delay(25);
  digitalWrite(pinoBuzzer, LOW);
}
// função de toque do buzzer para feedback de Falha de Login
void feedbackSomFalhaLogin() {
  digitalWrite(pinoBuzzer, HIGH);
  delay(50);
  digitalWrite(pinoBuzzer, LOW);
  delay(50);
  digitalWrite(pinoBuzzer, HIGH);
  delay(50);
  digitalWrite(pinoBuzzer, LOW);
  delay(50);
  digitalWrite(pinoBuzzer, HIGH);
  delay(50);
  digitalWrite(pinoBuzzer, LOW);
  delay(50);
  digitalWrite(pinoBuzzer, HIGH);
  delay(50);
  digitalWrite(pinoBuzzer, LOW);
  delay(50);
  digitalWrite(pinoBuzzer, HIGH);
  delay(50);
  digitalWrite(pinoBuzzer, LOW);
  delay(50);
  digitalWrite(pinoBuzzer, HIGH);
  delay(50);
  digitalWrite(pinoBuzzer, LOW);
}
// função de toque do buzzer para feedback de Tentativas Zeradas
void feedbackSomTentativasZeradas() {
  digitalWrite(pinoBuzzer, HIGH);
  delay(500);
  digitalWrite(pinoBuzzer, LOW);
  delay(500);
  digitalWrite(pinoBuzzer, HIGH);
  delay(500);
  digitalWrite(pinoBuzzer, LOW);
  delay(500);
  digitalWrite(pinoBuzzer, HIGH);
  delay(500);
  digitalWrite(pinoBuzzer, LOW);
}
// função de toque do buzzer para feedback de Login bem Sucedido
void feedbackSomLoginBemSucedido() {
  digitalWrite(pinoBuzzer, HIGH);
  delay(100);
  digitalWrite(pinoBuzzer, LOW);
  delay(50);
  digitalWrite(pinoBuzzer, HIGH);
  delay(100);
  digitalWrite(pinoBuzzer, LOW);
}
void feedbackConfirmacao() {
  delay(500);
  digitalWrite(pinoBuzzer, HIGH);
  delay(70);
  digitalWrite(pinoBuzzer, LOW);
  delay(70);
  digitalWrite(pinoBuzzer, HIGH);
  delay(70);
  digitalWrite(pinoBuzzer, LOW);
  delay(70);
  digitalWrite(pinoBuzzer, HIGH);
  delay(70);
  digitalWrite(pinoBuzzer, LOW);
  delay(70);
}
