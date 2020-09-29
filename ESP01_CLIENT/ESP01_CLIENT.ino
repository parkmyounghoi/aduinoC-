int pin5 = 5; int pin6 = 6; int pin9=9; int pin10=10; int pin11 = 11;
long time = millis();
int sunbun = 1;
#include "WiFiEsp.h"

#ifndef HAVE_HWEspSerial
#include "SoftwareSerial.h"
SoftwareSerial EspSerial(2, 3); // RX, TX
#endif

char ssid[] = "iptime";            // your network SSID (name)
char pass[] = "";                 // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

int inputpinD13 = 13;

char server[] = "dron512.dothome.co.kr";

// Initialize the Ethernet client object
WiFiEspClient client; //iptime 연결 하기 위한 클래스

void setup()
{
  pinMode(pin5,OUTPUT); pinMode(pin6,OUTPUT); pinMode(pin9,OUTPUT); pinMode(pin10,OUTPUT); pinMode(pin11,OUTPUT);
  pinMode(inputpinD13,INPUT_PULLUP);
  // initialize serial for debugging
  Serial.begin(9600);
  // initialize serial for ESP module
  EspSerial.begin(9600);
  // initialize ESP module
  WiFi.init(&EspSerial);  //iptime 접속 초기화

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  // you're connected now, so print out the data
  Serial.println("You're connected to the network");
  
  printWifiStatus();

  Serial.println();
  Serial.println("Starting connection to server...");
  // if you get a connection, report back via serial

  digitalWrite(pin5,HIGH);
  digitalWrite(pin6,HIGH);
  digitalWrite(pin9,HIGH);
  digitalWrite(pin10,HIGH);
  digitalWrite(pin11,HIGH);
  if (client.connect(server, 80)) {
    Serial.println("Connected to server");
    // Make a HTTP request
    client.println("GET /a.php?a=33&b=44 HTTP/1.1");
    client.println("Host: dron512.dothome.co.kr");
    client.println("Connection: close");
    client.println("aa"); // println 서버로 보내는거 send
    client.flush();
  }
  while (client.available()) {
    char c = client.read(); //client.read 서버로부터 받는거
    Serial.write(c);
  }
  if (!client.connected()) {
    Serial.println();
    Serial.println("Disconnecting from server...");
    client.stop();   
  }
}

void loop()
{
  while(true);
  int read = digitalRead(inputpinD13);
  Serial.println("read = "+String(read));
  if(read == 0 ){
    
  }
  else{
    if( time+1000 < millis() && sunbun == 1 ){
      time = millis();
      sunbun = 2;
      digitalWrite(pin5,HIGH);
      digitalWrite(pin6,LOW);
      digitalWrite(pin9,LOW);
      digitalWrite(pin10,LOW);
      digitalWrite(pin11,LOW);
    }
    if( time+1000 < millis() && sunbun == 2 ){
      time = millis();
      sunbun = 3;
      digitalWrite(pin5,LOW);
      digitalWrite(pin6,HIGH);
      digitalWrite(pin9,LOW);
      digitalWrite(pin10,LOW);
      digitalWrite(pin11,LOW);
    }
    if( time+1000 < millis() && sunbun == 3 ){
      time = millis();
      sunbun = 4;
      digitalWrite(pin5,LOW);
      digitalWrite(pin6,LOW);
      digitalWrite(pin9,HIGH);
      digitalWrite(pin10,LOW);
      digitalWrite(pin11,LOW);
    }
    if( time+1000 < millis() && sunbun == 4 ){
      time = millis();
      sunbun = 5;
      digitalWrite(pin5,LOW);
      digitalWrite(pin6,LOW);
      digitalWrite(pin9,LOW);
      digitalWrite(pin10,HIGH);
      digitalWrite(pin11,LOW);
    }
    if( time+1000 < millis() && sunbun == 5 ){
      time = millis();
      sunbun = 1;
      digitalWrite(pin5,LOW);
      digitalWrite(pin6,LOW);
      digitalWrite(pin9,LOW);
      digitalWrite(pin10,LOW);
      digitalWrite(pin11,HIGH);
    }
  }
  delay(200);
}


void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
