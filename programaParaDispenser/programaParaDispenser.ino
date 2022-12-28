#include <HX711.h>
#include <EasyBuzzer.h>
#include <WiFi.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

//Conexion Servidor Web
bool apertura=false;

WiFiServer server(80);
WiFiClient client;

const char* ssid     = "TeleCentro-9155";
const char* password = "LDMHLJMJDN2M";

String pagina = "<!DOCTYPE html>"
  "<html>"
  "<head>"
  "<meta charset='utf-8' />"
  "<title>Servidor Web ESP32</title>"
  "</head>"
  "<body>"
  "<center>"
  "<h1>Servidor Web ESP32</h1>"
  "</center>"
  "</body>"
  "</html>";

//const char* ssid     = "WifiDispenser";
//const char* password = "DispC842";

int contconexion = 0;
String header;


//Pines para MPP
#define IN1 13
#define IN2 12
#define IN3 14
#define IN4 27
int demoraMPP = 30;

//Pin para Buzzer
#define Buzzer 5

//Pines y valores para pantalla OLED SSD1306
#define ANCHO_PANTALLA 128
#define LARGO_PANTALLA 64
Adafruit_SSD1306 display(ANCHO_PANTALLA,LARGO_PANTALLA,&Wire,-1);
int x,y;
char puntero = '-';

//Variables de control
int pulsos=0;
int valor=0;
bool EstadoBuzzer=true;
bool menu2=false;
bool menu3=false;
bool menu4=false;
  

//Pines y valores para KY-040
#define PIN_A 2  
#define PIN_B 4    
#define PIN_BOTON 15 

#define ANTIREBOTE_A 150

static int valorEnc = 0;

volatile bool sentidoReloj = false;
volatile bool sentidoAntiReloj = false;

unsigned long TiempoAntirebote = 0;

bool sentidoAnteriorR = false;
bool sentidoAnteriorAR = false;

//Sensor de peso
HX711 escala;
float factorDeCalibracion = 48100;
float unidades;



/**
 * FUNCIONES DE CONTROL
**/
void leerEncoder() {
  if ((!sentidoReloj)&&(!sentidoAntiReloj)) {
    int pinA = digitalRead(PIN_A);
    delayMicroseconds(1500);
    int pinB = digitalRead(PIN_B);
    if (pinA == pinB){
      if (sentidoAnteriorR) {
        sentidoReloj = true;
      } else {
        sentidoAntiReloj = true;
      }      
    } else {
      if (sentidoAnteriorAR) {
        sentidoAntiReloj = true;
      } else {
        sentidoReloj = true;
      }
    }    
  }
}

int Encoder(){

  //Pulsos
  if (!digitalRead(PIN_BOTON)) {
    pulsos++;
    
    //valorEnc = 0;
    //Serial.print("Reset: ");
    //Serial.println(valorEnc);
    if(EstadoBuzzer==true){
    EasyBuzzer.singleBeep(2637,200);
    }
    while(!digitalRead(PIN_BOTON)){
    delay(200);
    EasyBuzzer.stopBeep();
    }
  }
  
  //Giros
  if (sentidoReloj) {
    valorEnc+=16;
    //Serial.print("Turned CW: ");
    //Serial.println(valorEnc);
    sentidoReloj = false;
    sentidoAnteriorR = true;
    TiempoAntirebote = millis();
  }

  if (sentidoAntiReloj) {
    valorEnc-=16;
    //Serial.print("Turned CCW: ");
    //Serial.println(valorEnc);
    sentidoAntiReloj = false;
    sentidoAnteriorAR = true;
    TiempoAntirebote = millis();
  }

  if(valorEnc>48){
  valorEnc=48;
  }
  if(valorEnc<0){
  valorEnc=0;
  }

  if ((millis() - TiempoAntirebote) > ANTIREBOTE_A) {
    sentidoAnteriorR = false;
    sentidoAnteriorAR = false;
  }
 return valorEnc;
}

float lecturaPeso(bool Pasaje){
  escala.set_scale(factorDeCalibracion);

  unidades = escala.get_units(),10;
  if (unidades < 0)
  {
    unidades = 0.00;
  }

  if(Pasaje==false){
  return unidades;
  }
  if(Pasaje==true){
  return unidades*1000;
  }
}



/**
 * FUNCIONES GRAFICAS
**/

void ventana4(){
  
  x=5,y=Encoder();
  if(y>48){
  y=48;
  }
  if(y<0){
  y=0;
  }

  
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  
  display.setCursor(x,y);
  display.print(puntero);

  display.setCursor(x+12,0);
  display.println("Salir");
  
  display.setCursor(x+12,16); 
  if(apertura==false){
  display.println("Cerrado");
  }else if(apertura==true){
  display.println("Abierto");
  }
  
  display.setCursor(x+12,32);
  if(apertura==false){
  display.println("'IP'");
  }else if(apertura==true){
  display.println(WiFi.localIP());
  }
  

  if(y==16 && pulsos==1){
  pulsos=0;
    if(apertura==false){
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED and contconexion <50) { 
    ++contconexion;
    delay(500);
    
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
  
    display.setCursor(x,y);
    display.print(puntero);

    display.setCursor(x+12,0);
    display.println("Salir");
  
    display.setCursor(x+12,16); 
    display.println("Abriendo");
  
    display.setCursor(x+12,32);
    display.print(".");
    display.println("..");

    display.display();
    }
    if (contconexion <50) {
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);

      display.setCursor(x,y);
      display.print(puntero);

      display.setCursor(x+12,0);
      display.println("Salir");

      display.setCursor(x+12,16); 
      display.println("Abierto");
      
      display.setCursor(x+12,32);
      display.println(WiFi.localIP());
      server.begin();
      apertura=true;
    }else { 
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);

      display.setCursor(x,y);
      display.print(puntero);

      display.setCursor(x+12,0);
      display.println("Salir");

      display.setCursor(x+12,16); 
      display.println("Error");
      
      display.setCursor(x+12,32);
      display.println("X");
      display.display();
      apertura=false;
      delay(500);
    }
    
    
    
    }else if(apertura==true){ 
    apertura=false;
    header = "";
    // Cerramos la conexión
    client.stop(); 
    }
  
  }
  
  if(y==0 && pulsos==1){
  pulsos=0;
  menu4=false;
  }

  client = server.available();
  if (client) {                          
    String currentLine = "";               
     while(client.connected()) {            
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c);                    
        header += c;
        if (c == '\n') {                    
          
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
        
            // Muestra la página web
            client.println(pagina);
            
            // la respuesta HTTP temina con una linea en blanco
            client.println();
          } else { // si tenemos una nueva linea limpiamos currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // si C es distinto al caracter de retorno de carro
          currentLine += c;      // lo agrega al final de currentLine
        }
      }   

       x=5,y=Encoder();
       if(y>48){
        y=48;
        }
       if(y<0){
        y=0;
       }
       
       display.clearDisplay();
       display.setTextSize(2);
       display.setTextColor(WHITE);

       display.setCursor(x,y);
       display.print(puntero);

       display.setCursor(x+12,0);
       display.println("Salir");

       display.setCursor(x+12,16); 
       display.println("Abierto");
      
       display.setCursor(x+12,32);
       display.println(WiFi.localIP());
       display.display();

       if(y==16 && pulsos==1){
       pulsos=0;
       if(apertura==true){
       apertura=false;
       header = "";
       client.stop(); 
       }
      }
     }
    header = "";
    client.stop(); 
  
  }
  display.display();
}


void ventana3(){

  int paso[4][4]={
  {1,0,0,0},
  {0,1,0,0},
  {0,0,1,0},
  {0,0,0,1}
  };
  
  bool servir = false;
  bool servirC=false;
  bool servirA=false;
  
  bool conversion=false;
  
  bool sensadoPeso=false;
  bool actuadoresM = false;
  float peso;
  x=5,y=Encoder();
  if(y>48){
  y=48;
  }
  if(y<0){
  y=0;
  }
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  
  display.setCursor(x,y);
  display.print(puntero);

  display.setCursor(x+12,0);
  display.println("Leer peso");
  
  display.setCursor(x+12,16); 
  display.println("Servir");
  
  display.setCursor(x+12,32);
  display.println("Salir");

  if(y==0 && pulsos==1){
  pulsos=0;
  sensadoPeso=true;
    while(sensadoPeso==true){
    x=5,y=Encoder();
    if(y>48){
    y=48;
    }
    if(y<0){
    y=0;
    }
    
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
  
    display.setCursor(x,y);
    display.print(puntero);

    display.setCursor(x+12,0);
    display.println("Peso:");
  
    display.setCursor(x+12,16); 
    display.println("Convertir");
  
    display.setCursor(x+12,32);
    if(conversion==false){
    peso=lecturaPeso(conversion);
    display.print(peso);
    display.println("Kg");
    }else if(conversion==true){
    peso=lecturaPeso(conversion);
    display.print(peso);
    display.println("g");
    }
    
    display.setCursor(x+12,48);
    display.println("Salir");
    display.display();

    if(y==16 && pulsos==1){
    pulsos=0;
      if(conversion==false){
        conversion=true;
      }else if(conversion==true){
        conversion=false;
      }
    }

    if(y==32 && pulsos==1){
    pulsos=0;
      
    }
    
    if(y==48 && pulsos==1){
    pulsos=0;
    sensadoPeso=false; 
    }
    
    }
  }

  if(y==16 && pulsos==1){
  pulsos=0;
  actuadoresM = true;
  
    while(actuadoresM == true){
      display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
      x=5,y=Encoder();
      if(y>48){
       y=48;
      }
      if(y<0){
       y=0;
      }

      display.setCursor(x,y);
      display.print(puntero);

      display.setCursor(x+12,0);
      if(servirA==false && servirC==false){
      display.println("'Elejir'");
      }
      if(servirA==true){
      display.println("Agua");
      }
      if(servirC==true){
      display.println("Comida");
      }
      
      display.setCursor(x+12,16); 
      if(servir==true){
      display.println("Servir");
      }else if(servir==false){
      display.println("No Servir");
      }

      if(servir==true && servirC==true){
      for(int b=0;b<4;b++){
      digitalWrite(IN1,paso[b][0]);
      digitalWrite(IN2,paso[b][1]);
      digitalWrite(IN3,paso[b][2]);
      digitalWrite(IN4,paso[b][3]);
      delay(demoraMPP);
      }
      
      }else if(servir==false && servirC==true){
      digitalWrite(IN1, LOW); 
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      }
      
      
      display.setCursor(x+12,32);
      display.println("Salir");

      display.display();
      
      if(y==0 && pulsos==1){
      pulsos=0;
      if(servirC==false && servirA==true){
        servirC=true;
        servirA=false;
      }else if(servirC==true && servirA==false){
        servirC=false;
        servirA=true;
      }else if(servirC==false && servirA==false){
        servirC=false;
        servirA=true;
      }
      
      }
      if(y==16 && pulsos==1){
      pulsos=0;
      if(servir==false){
        servir=true;
      }else if(servir==true){
        servir=false;
      }
    }

      if(y==32 && pulsos==1){
      pulsos=0;
      actuadoresM = false;
     }
      
    }
  
  }

  //Salida del menu de entorno
  if(y==32 && pulsos==1){
  pulsos=0;
  menu3=false;
  }
  display.display();
}


void ventana2(){
  x=5,y=Encoder();
  if(y>48){
  y=48;
  }
  if(y<0){
  y=0;
  }
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  
  display.setCursor(x,y);
  display.print(puntero);

  display.setCursor(x+12,0);
  display.println("Ajustes");
  if(EstadoBuzzer==true){
  display.setCursor(x+12,16); 
  display.println("Sonido/Si");
  }else if(EstadoBuzzer==false){
  display.setCursor(x+12,16); 
  display.println("Sonido/No");
  }
  display.setCursor(x+12,32);
  display.println("Salir");

  if(y==0 && pulsos==1){
  pulsos=0;
  }

  if(y==16 && pulsos==1){
  pulsos=0;
  if(EstadoBuzzer==true){
    EstadoBuzzer=false;
    }else if(EstadoBuzzer==false){
    EstadoBuzzer=true; 
    }
  }

  //Salida del menu de ajustes
  if(y==32 && pulsos==1){
  pulsos=0;
  menu2=false;
  }
  display.display();
}




void ventanas(){
  x=5,y=Encoder();
  
  //Limites de desplazamiento
  if(y>48){
  y=48;
  }
  if(y<0){
  y=0;
  }

  
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);

  //Menu principal
  
  display.setCursor(x,y);
  display.print(puntero);

  display.setCursor(x+12,0);
  display.println("Ajustes");
  display.setCursor(x+12,16); 
  display.println("Conexion");
  display.setCursor(x+12,32);
  display.println("Entorno");
  
  //display.setCursor(x+12,48);
  //display.println("Reducir");



  
  //Menu de ajustes
  if(y==0 && pulsos==1){
  pulsos=0;
  menu2=true;
  while(menu2==true){
  ventana2();
  }
}




  //Menu de conexion
  if(y==16 && pulsos==1){
  pulsos=0;
  menu4=true;
  while(menu4==true){
  ventana4();
  pulsos=0;
  }
  }

  //Menu de entorno
  if(y==32 && pulsos==1){
  pulsos=0;
  menu3=true;
  while(menu3==true){
  ventana3();
  }
  }


  
  if(y==48 && pulsos==1){

  
  pulsos=0;
  }
  
  display.display(); 
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  server.begin();
  escala.begin(16,17);
  escala.set_scale();
  escala.tare();

  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC,0x3c)){
    Serial.println("No se encontro la pantalla");
    for(;;);
  }
  display.clearDisplay();
  
  EasyBuzzer.setPin(Buzzer);
  pinMode(PIN_A, INPUT_PULLUP);
  pinMode(PIN_B, INPUT_PULLUP);
  pinMode(PIN_BOTON, INPUT_PULLUP);
  attachInterrupt(PIN_B, leerEncoder, CHANGE);
  Serial.println("Encoder detectado: ");
  
}

void loop() {
  EasyBuzzer.update();
 
  ventanas();
}
