#include <Arduino.h>
#include <HardwareSerial.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <WiFiManager.h>
#include <WiFiClient.h>
#include <WiFi.h>  // Certifique-se de incluir a biblioteca correta
#include "driver/adc.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <ESP32Firebase.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"


#define Valvula 14 // Pinos de ligaçao de output digital '
#define Bomba 2    // Pinos de ligaçao de output analogico  '

#define sensorUmidade1 34 // Pinos de ligaçao de input analogica
#define sensorUmidade2 35
#define sensorUmidade3 32
#define ACS712_PIN  33
#define DS18B20_Pin 25 
#define DHTPIN 26  

#define sensorVazao 4  // Pinos de ligaçao de input digital 

OneWire oneWire(DS18B20_Pin);           // Configuração do OneWire :
DallasTemperature sensors(&oneWire);

#define DHTTYPE DHT22  // Tipo de sensor (DHT11, DHT21, ou DHT22)
DHT dht(DHTPIN, DHTTYPE); //Crie um objeto do tipo DHT:

#define Dwin Serial2  // Assuming Serial for display
unsigned char Buffer[9];

#define consumo_id 0x60
#define corrente_id 0x61
#define umidade_id 0x62
#define temp_id 0x63
#define umidade_Ar_id 0x64
#define consumoTotal_id 0x65

unsigned char Consumo[8] = {0x5a, 0xa5, 0x05, 0x82,consumo_id, 0x00, 0x00, 0x00};
unsigned char Corrente[8] = {0x5a, 0xa5, 0x05, 0x82, corrente_id, 0x00, 0x00, 0x00};
unsigned char Umidade[8] = {0x5a, 0xa5, 0x05, 0x82, umidade_id, 0x00, 0x00, 0x00};
unsigned char Temperatura[8] = {0x5a, 0xa5, 0x05, 0x82, temp_id, 0x00, 0x00, 0x00};
unsigned char Umidade_AMB[8] = {0x5a, 0xa5, 0x05, 0x82, umidade_Ar_id, 0x00, 0x00, 0x00};
unsigned char ConsumoTotal[8] = {0x5a, 0xa5, 0x05, 0x82,consumoTotal_id, 0x00, 0x00, 0x00};

#define bt_valvula_id 0x52
#define bt_bomba_id 0x53

unsigned char Bt_valvula[8] = {0x5a, 0xa5, 0x05, 0x82,bt_valvula_id, 0x00, 0x00, 0x00};
unsigned char Bt_bomba[8] = {0x5a, 0xa5, 0x05, 0x82,bt_bomba_id, 0x00, 0x00, 0x00};


unsigned long previousMillis = 0;  // Armazena o último tempo que a função foi executada
const long interval =  5000 ;          // 30 * 60 * 1000;        //2000 Intervalo desejado em milissegundos (2 segundos)
unsigned long currentMillis = 0;   // Variável global para armazenar o tempo atual

unsigned long previousMillis1 = 0;
const long interval1 = 5000;
unsigned long currentMillis1 = 0; 

//Modulo Leitor Gravador Micro Sd Esp32 Arduino Nodemcu
const int chipSelect = 5;

// Calibração do sensor (ajuste conforme necessário)
const float sensibilidade = 66.0;  // mV/A (valor padrão para o ACS712)
int offset = 2325;       // Valor de offset (ajuste conforme necessário)

const String tabela = "ControleIrrigacao";
const String colunas = "EstufaID, StatusBomba, StatusValvula, CorrenteBomba, UmidadeSolo, UmidadeAmbiente, TemperaturaAmbiente";

unsigned long previousMillisVazao = 0;
int contador = 0;
int consumo = 0 ;
float vazao =0 ;
float vazaoTotal = 0 ;

int motorSpeedmanual = 0;
int velocidadeMotor = 200;

float cont_val = 0;
float cont_bomb = 0;
int controleValvula_anterior = -1; // atualizaçao ihm controle manual on/off
int controleBomba_anterior = -1;  // atualizaçao ihm controle manual on/off
int controleBomba=0; 
int controleValvula=0;
int atualiza_ihm = 0;

int umidadeMinima = 0;
int umidadeMaxima = 0;

int temp_adc_val = 10;
bool statusValvula = false;
bool statusBomba = false;

// Configurações de Wi-Fi

const char *ssid = "DESKTOP-AT0N6UN 7487";
const char *password = "cesar123";

// Configurações de banco de dados 

IPAddress server_ip(85, 10, 205, 173);  // Substitua pelos valores do seu servidor MySQL
char user[] = "rootarduino";
char password_mysql[] = "quemsoueu";
char db[] = "tccarduino";

// Objeto de conexão MySQL
WiFiClient client;
MySQL_Connection conn((Client*)&client);

 int t = 0;


enum Estado {
  AUTOMATICO,
  MANUAL
};

Estado estadoAtual = AUTOMATICO; 

// Funções declaradas
void conectarWiFi();
void inserirNoBanco(const String &tabela, const String &colunas, int estufaID, bool statusBomba, bool statusValvula, float correnteBomba, float umidadeSolo, float umidadeAmbiente, float temperaturaAmbiente);
void Data_Arduino_to_Display(int sensor, unsigned char endereco[]);
void IHM_Controle_();
void controlarBombaUmidade( int umidadePercentual);
int  leituraporcentagem();
void desligarSistema(); 
void ligarSistema(int velocidadeMotor, const char* mensagem);
float vazaoAgua();
float readACS712();
void Bomba_LD();
void Valvula_LD();
void mudarParaAutomatico();
void mudarParaManual();
void verificarAlarmes(float correnteBomba, int umidadeSolo, float umidadeAmbiente, float temperaturaAmbiente);
void acionarAlarme(const char* mensagem) ;


void setup(){
  
  pinMode(Valvula, OUTPUT);
  digitalWrite(Valvula, LOW);
  pinMode(Bomba, OUTPUT); // Define o pino como saída
  digitalWrite(Bomba, LOW);
  pinMode(sensorVazao, INPUT);
  pinMode(ACS712_PIN, INPUT_PULLUP);
  pinMode(sensorUmidade1, INPUT_PULLUP);
  pinMode(sensorUmidade2, INPUT_PULLUP);
  pinMode(sensorUmidade3, INPUT_PULLUP);
 
  attachInterrupt(digitalPinToInterrupt(sensorVazao), contarPulsos, RISING); // Anexa a interrupção

  ledcSetup(0, 1000, 10);  // Canal 0, 1000 Hz de frequência, resolução de 10 bits
  ledcAttachPin(Bomba, 0);

  Dwin.begin(115200);
  sensors.begin();  // Inicia o sensor DS18B20
  dht.begin();  // Inicia o sensor DHTPIN

  Serial.begin(115200);

   if(!SD.begin(chipSelect)){
        Serial.println("Card Mount Failed");
   delay(1000);
    }
   Serial.println("Cartão inicializado corretamente!!!");

  conectarWiFi();
    
  // escreverArquivo(caminho, mensagem);
/*
  WiFi.begin(ssid,password);
    while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.println("erro");
  }
 / Serial.println("conectado");
 */ 
  //  Conectar ao banco de dados MySQL
  /*
   if (conn.connect(server_ip, 3306, user, password_mysql, db)) {
       Serial.write("Connected to MySQL server");
   } else {
     Serial.write("Connection failed.");
   }
 */
  
}

void loop(){ 

  const char *caminho = "/teste.txt";
  const char *mensagem = "Teste de escrita no arquivo.";
  escreverArquivo(caminho, mensagem);
  

    IHM_Controle_();
    reconectarWiFi();
    reconectarBanco();


    int    estufaID = 2;
    vazao = vazaoAgua();
    float  correnteBomba = readACS712();
    float  umidadeSolo =  leituraporcentagem();
    float  umidadeAmbiente = dht.readHumidity();
    sensors.requestTemperatures();
    float  temperaturaAmbiente = dht.readTemperature();                //sensors.getTempCByIndex(0);
   //  Serial.println(temperaturaAmbiente);

     if ( estadoAtual == AUTOMATICO) {     // Exemplo: Verifica se há uma transição para o modo automático
      controlarBombaUmidade(umidadeSolo);
    } else if (estadoAtual == MANUAL  ) {
       Bomba_LD();
       Valvula_LD(); 
      }

    currentMillis = millis();
     if (currentMillis - previousMillis >= interval) {
    // Salva o último tempo que a função foi executada
     previousMillis = currentMillis;

      Serial.println("comando banco executado");
     inserirNoBanco(tabela, colunas, estufaID, statusBomba, statusValvula,
     correnteBomba, umidadeSolo, umidadeAmbiente, temperaturaAmbiente);
     }

     
  currentMillis1 = millis();
  if (currentMillis1 - previousMillis1 >= interval1) {
  previousMillis1 = currentMillis1; 
  
  switch (atualiza_ihm) {
    case 0:
      Data_Arduino_to_Display(correnteBomba, Corrente); 
      atualiza_ihm++;
      break;
      
    case 1:
      Data_Arduino_to_Display(umidadeSolo, Umidade);
      atualiza_ihm++;
      break;
      
    case 2:
      Data_Arduino_to_Display(vazao, Consumo);
      atualiza_ihm++;
      break;
      
    case 3:
      Data_Arduino_to_Display(temperaturaAmbiente, Temperatura);
      atualiza_ihm++;
      break;
      
    case 4:
      Data_Arduino_to_Display(umidadeAmbiente, Umidade_AMB);
      atualiza_ihm ++ ;
      break;
    case 5:
      Data_Arduino_to_Display(vazaoTotal, ConsumoTotal);
      atualiza_ihm = 0;
      break;
      
    default:
      break;
  }
 }
}
//=============================================================================

/*void Data_Arduino_to_Display(int sensor, unsigned char endereco[])
{
  delay(5);
  endereco[6] = highByte(sensor);
  endereco[7] = lowByte(sensor);
  Dwin.write(endereco, 8);
 // Serial.println(endereco[4]);
  delay(5);
}*/

void Data_Arduino_to_Display(float sensorValue, unsigned char endereco[]) {
  // Converte o valor float para um array de bytes
 
 delay(5);

  byte hex [4];
  FloatToHex(sensorValue,hex);
  Dwin.write(endereco, 6 );
  Dwin.write(hex[3]);
  Dwin.write(hex[2]);
  Dwin.write(hex[1]);
  Dwin.write(hex[0]);

 delay(5);

}

void FloatToHex(float sensorValue, byte hex[4]) {
    // Cria um ponteiro para o valor float
    byte *bytePointer = reinterpret_cast<byte*>(&sensorValue);

    // Copia os bytes do float para o array hex
    for (int i = 0; i < 4; ++i) {
        hex[i] = *bytePointer;
        ++bytePointer;
    }
}

 void IHM_Controle_() {
  if (Dwin.available() ) {
    for (int i = 0; i < 9; i++) {
      Buffer[i] = Dwin.read();
    }
    
    if (Buffer[0] == 0x5A) {
      switch (Buffer[4]) {
        case 0x59:
          handleCase59(Buffer[8]);
          break;
        case 0x58:
          handleCase58(Buffer[8]);
          break;
        case 0x57:
          handleCase57(Buffer[8]);
          break;
        case 0x51:
          handleCase51(Buffer[8]);
          break;
        case 0x50:
          handleCase50(Buffer[8]);
          break;
         case 0x55:
          handleCase55(Buffer[8]);
          break;
        default:
          handleDefaultCase(Buffer[4]);
          break;
      }
    } else {
      Serial.println("Valor inesperado em Buffer[0]");
    }
  } else {
    Serial.println("Ainda não há dados suficientes disponíveis em Dwin");
  }
}

void handleCase59(byte value) {
  if (value == 0) {
    mudarParaAutomatico();
  } else if (value == 1) {
    mudarParaManual();
  } else {
    Serial.println("Valor inesperado em Buffer[8] para o caso 0x59");
  }
}

void handleCase58(byte value) {
  // Implemente o comportamento para o caso 0x58
  umidadeMaxima = value;
  Serial.println("Umidade Máxima atualizada para: " + String(umidadeMaxima));
}

void handleCase57(byte value) {
  // Implemente o comportamento para o caso 0x57
  umidadeMinima = value;
  Serial.println("Umidade Mínima atualizada para: " + String(umidadeMinima));
}

void handleCase51(byte value) {
  // Implemente o comportamento para o caso 0x51
  controleBomba = value;
  cont_val = value ;
  Serial.println("Controle da Bomba atualizado para: " + String(controleBomba)); 
}
void handleCase50(byte value) {
  // Implemente o comportamento para o caso 0x50
  controleValvula = value;
  cont_bomb = value ;
  Serial.println("Controle da Válvula atualizado para: " + String(controleValvula));
}

void handleCase55(byte value) {
  // Implemente o comportamento para o caso 0x55
  offset =value*100;      
  Serial.println("Valor de offset: " + String( offset));
}

void handleDefaultCase(byte value) {
  // Implemente o comportamento para casos não tratados
  Serial.println("Comando não reconhecido: " + String(value));
}



float readACS712() {
       uint32_t soma_adc = 0;
       for (int i = 0; i < 300; i++) {
       soma_adc +=  analogRead(ACS712_PIN) ;  // Leitura do valor analógico
      vTaskDelay(pdMS_TO_TICKS(1));  // Pequeno atraso para estabilizar as leituras
    }
    float media_adc = (float)soma_adc / 300.0;
    float tensao = (media_adc / 4095.0) * 3300.0;    // Conversão para mV
    float corrente = (tensao - offset) / sensibilidade;
    //Serial.println(ACS712_PIN);
      //Serial.println(corrente)
   return corrente;
}

float vazaoAgua() {
  // Verificar se passou um minuto
    float consumoAgua = contador / 425.0; // 1 litro a cada 325 pulsos
    return consumoAgua;
    // Se não passou um minuto, retornar -1 para indicar que não houve leitura ainda 
}

void contarPulsos() {
   Serial.println(contador);
  contador++;
}


void controlarBombaUmidade(float umidadePercentual) {
  if (umidadePercentual < umidadeMinima) {
    ligarSistema(velocidadeMotor, "ligado sistema");
  } else if (umidadePercentual > umidadeMaxima) {
    desligarSistema();
  } else if (statusBomba && statusValvula) {
    ligarSistema(velocidadeMotor, "ligado sistema");
  } else if (!statusBomba && !statusValvula ) {
    desligarSistema();
  }
}
void Bomba_LD(){

   if(controleBomba != controleBomba_anterior){
    controleBomba_anterior = controleBomba;
   if (controleBomba == 1){
   statusBomba = true ;
   analogWrite(Bomba, velocidadeMotor);
   Serial.println("liga Bomba");
   delay(10); 
    } else if (controleBomba == 0) {
   statusBomba = false ;
   analogWrite(Bomba, 0);
   Serial.println("desliga Bomba");
   vazaoTotal = vazaoTotal +vazao ;
   contador = 0;
   delay(10);  
  }
 }
}

void  Valvula_LD(){

  if(controleValvula != controleValvula_anterior){
    controleValvula_anterior = controleValvula ;
   if (controleValvula == 1){
    digitalWrite(Valvula, HIGH);
    Serial.println("liga valvula");
    statusValvula = true;
    delay(10); 
   } else if (controleValvula == 0){ 
    digitalWrite(Valvula, LOW);
    Serial.println("desliga valvula");
    statusValvula = false;
    delay(10); 

   }
  }
 }

void ligarSistema(int velocidadeMotor, const char* mensagem) {
  controleBomba = 1 ;
  controleValvula =1 ;
  statusValvula = true;
  statusBomba = true;
 // Serial.println(mensagem);
  Valvula_LD();
  delay(50);  // Aguarde um curto período para perceber a mudança de velocidade
  Bomba_LD();
 
}

void desligarSistema() {
  controleBomba = 0 ;
  controleValvula = 0 ;
   Valvula_LD();
   Bomba_LD();
   statusValvula = false;
   statusBomba = false;// Serial.println("desligado sistema");
}

int leituraporcentagem() {
  // Leitura do primeiro sensor
  int valorSensor1 = analogRead(sensorUmidade1);

  // Leitura do segundo sensor
  int valorSensor2 = analogRead(sensorUmidade2);

  // Leitura do terceiro sensor
  int valorSensor3 = analogRead(sensorUmidade3);

  // Mapeie os valores para a faixa de 0 a 100
  float umidadePercentual1 = map(valorSensor1, 4200,0, 100, 0);
  float umidadePercentual2 = map(valorSensor2, 3000,0, 100, 0);
  float umidadePercentual3 = map(valorSensor3, 3000,0, 100, 0);

  // Calcule a média dos valores
  float mediaUmidadePercentual = (umidadePercentual1 + umidadePercentual2 + umidadePercentual3) / 3.0;

  // Limite o valor entre 0 e 100
  mediaUmidadePercentual = constrain(mediaUmidadePercentual, 0, 100);

  return static_cast<int>(mediaUmidadePercentual);  // Converte para inteiro antes de retornar
}

void inserirNoBanco(const String& tabela, const String& colunas, int estufaID, bool statusBomba,
 bool statusValvula, float correnteBomba, float umidadeSolo, float umidadeAmbiente, float temperaturaAmbiente) {
  // Verifica se o estufaID existe na tabela Estufas
   String INSERT_SQL = "INSERT INTO " + tabela + " (" + colunas + ") VALUES ('" + String(estufaID) + "', '"
            + String(statusBomba) + "', '" + String(statusValvula) + "', '" + String(correnteBomba) + "', '" + String(umidadeSolo)
             + "', '" + String(umidadeAmbiente) + "', '" + String(temperaturaAmbiente) + "')";

  // Monta a consulta SQL dinamicamente
 /*String INSERT_SQL = "INSERT INTO ControleIrrigacao (EstufaID, StatusBomba, StatusValvula, CorrenteBomba,
  UmidadeSolo, UmidadeAmbiente, TemperaturaAmbiente) VALUES ('2', '1', '0', '2.50', '45.00', '60.00', '25.00')";
*/
  // Cria um cursor para executar a consulta
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  // Tenta executar a consulta
  if (cur_mem->execute(INSERT_SQL.c_str())) {
    // Sucesso
    Serial.write("Inserção no banco de dados bem-sucedida.");
  } else {
    // Falha
    Serial.write("Erro ao inserir no banco de dados.");
    // Imprime a mensagem de erro do cursor  
   
  }

  // Libera a memória do cursor
  delete cur_mem;
} 
// exemplo de consulta necessario adptaçao 
/*
void buscarNoBanco(const String& tabela, const String& colunas, const String& condicao) {
  String SELECT_SQL = "SELECT " + colunas + " FROM " + tabela + " WHERE " + condicao;

  // Cria um cursor para executar a consulta
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  // Tenta executar a consulta
  if (cur_mem->execute(SELECT_SQL.c_str())) {
    // Sucesso
    Serial.write("Consulta ao banco de dados bem-sucedida.\n");
    // Obtém e exibe os resultados
    MySQL_Row *row = NULL;
    do {
      row = cur_mem->get_next_row();
      if (row != NULL) {
        for (int i = 0; i < row->get_field_count(); i++) {
          Serial.print(row->get_field(i));
          Serial.print("\t");
        }
        Serial.println();
      }
    } while (row != NULL);
  } else {
    // Falha
    Serial.write("Erro ao consultar o banco de dados.\n");
    // Imprime a mensagem de erro do cursor
  }

  // Libera a memória do cursor
  delete cur_mem;
} 
 */

void mudarParaAutomatico() {
  estadoAtual = AUTOMATICO;
  Serial.println("Mudou para o modo AUTOMÁTICO");
}

void mudarParaManual() { 
  estadoAtual = MANUAL;
  Serial.println("Mudou para o modo MANUAL");
}


void conectarWiFi(){
  // Conectar-se ao WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Conectado ao WiFi!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}


void verificarAlarmes(float correnteBomba, int umidadeSolo, float umidadeAmbiente, float temperaturaAmbiente) {
    // Verificar condições de alarme e acionar o alarme conforme necessário
    if (correnteBomba > 5.0) {
        // Corrente da bomba muito alta, acione o alarme
        acionarAlarme("Corrente da bomba muito alta!");
    }

    if (umidadeSolo < umidadeMinima) {
        // Umidade do solo muito baixa, acione o alarme
        acionarAlarme("Umidade do solo muito baixa!");
    }

    if (umidadeAmbiente > 80) {
        // Umidade ambiente muito alta, acione o alarme
        acionarAlarme("Umidade ambiente muito alta!");
    }

    if (temperaturaAmbiente > 35) {
        // Temperatura ambiente muito alta, acione o alarme
        acionarAlarme("Temperatura ambiente muito alta!");
    }

     if (WiFi.status() != WL_CONNECTED) {
        // A conexão WiFi caiu, acione o alarme
        acionarAlarme("Conexão WiFi perdida!");
    }




    
}

void acionarAlarme(const char* mensagem) {
    // Aqui você pode implementar a lógica para acionar o alarme,
    // como enviar uma notificação por email, SMS ou acionar um alarme sonoro.
    // Por exemplo:
    Serial.println("ALERTA: " + String(mensagem));
    // Código para acionar o alarme, por exemplo, acender um LED ou tocar um buzzer.

}
void reconectarWiFi() {
    // Tentar reconectar ao WiFi
    if (WiFi.status() != WL_CONNECTED) {
        Serial.print("Tentando reconectar ao WiFi...");
        WiFi.begin(ssid, password);
        delay(5000);

        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("");
            Serial.println("Reconectado ao WiFi!");
            Serial.print("Endereço IP: ");
            Serial.println(WiFi.localIP());
        } else {
            Serial.println("");
            Serial.println("Falha na reconexão ao WiFi.");
        }
    }
}

bool reconectarBanco() {
    // Tenta conectar ao servidor MySQL
    if (!conn.connected()) {
    if (conn.connect(server_ip, 3306, user, password_mysql, db)) {
       Serial.write("Connected to MySQL server");
       return true;
    } else {
     Serial.write("Connection failed.");
     return false;
    }
  }    
}

void lerArquivo(const char *caminho) {
  Serial.printf("Lendo arquivo: %s\n", caminho);

  File arquivo = SD.open(caminho);
  if (!arquivo) {
    Serial.println("Falha ao abrir o arquivo para leitura.");
    return;
  }

  Serial.println("Conteúdo do arquivo:");
  while (arquivo.available()) {
    Serial.write(arquivo.read());
  }
  arquivo.close();
}

void escreverArquivo(const char *caminho, const char *mensagem) {
  Serial.printf("Escrevendo no arquivo: %s\n", caminho);

  File arquivo = SD.open(caminho, FILE_WRITE);
  if (!arquivo) {
    Serial.println("Falha ao abrir o arquivo para escrita.");
    return;
  }

  if (arquivo.print(mensagem)) {
    Serial.println("Escrita bem-sucedida.");
  } else {
    Serial.println("Falha ao escrever no arquivo.");
  }
  arquivo.close();
}

void excluirArquivo(const char *caminho) {
  Serial.printf("Excluindo arquivo: %s\n", caminho);

  if (SD.remove(caminho)) {
    Serial.println("Arquivo excluído com sucesso.");
  } else {
    Serial.println("Falha ao excluir o arquivo.");
  }
}