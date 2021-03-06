#include <DHT.h>
#include <ESP8266WiFi.h>
 
// Anstelle der ??? den ThingSpeak Write API Key und die WLAN Netzwerk SSID und das Passwort eintragen
String apiKey = "???";
const char* ssid = "???";
const char* password = "???";

// Die ThingSpeak Server Adresse
const char* server = "api.thingspeak.com";

// Der Datenpin des Sensors
#define DHTPIN D4
// Der Sensortyp
#define DHTTYPE DHT22 


// Initialisierung
DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;


//Initialiserung beim Programmstart
void setup() 
{

// Baudrate der seriellen Debugschnittstelle
Serial.begin(115200);
delay(10);
dht.begin();

 
Serial.println();
Serial.println();
Serial.print("Verbindungsaufnahe zu ");
Serial.println(ssid);
 
WiFi.begin(ssid, password);
 
while (WiFi.status() != WL_CONNECTED) 
{
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi verbunden");
 
}


// Programmschleife, die wiederkehrend durchlaufend wird
void loop() 
{

// Einlesen der Sensorwerte
float h = dht.readHumidity();
float t = dht.readTemperature();

// Is Not A Number
// Überprüfung ob ein Zahlenwert eingelesen wurde
if (isnan(h) || isnan(t)) 
{
Serial.println("Fehler beim Lesen der Sensor Daten!");
return;
}

// Verbindung zum ThingspeakServer per http Port 80
if (client.connect(server,80)) {

// Datenstring zusammensetzen, Sensorwerte für Feld1 und Feld2
String postStr = apiKey;
postStr +="&field1=";
postStr += String(t);
postStr +="&field2=";
postStr += String(h);
postStr += "\r\n\r\n";

// Übergabe der Werte an den Server
client.print("POST /update HTTP/1.1\n");
client.print("Host: api.thingspeak.com\n");
client.print("Connection: close\n");
client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
client.print("Content-Type: application/x-www-form-urlencoded\n");
client.print("Content-Length: ");
client.print(postStr.length());
client.print("\n\n");
client.print(postStr);

// Debugausgabe
Serial.print("Temperatur: ");
Serial.print(t);
Serial.print(" Luftfeuchtigkeit: ");
Serial.print(h);
Serial.println(" - Sende Daten zu ThingSpeak");
}

// Verbindungsabbau zum Server
client.stop();

// Debugausgabe
Serial.println("Warte 20 secs");
delay(20000);
}
