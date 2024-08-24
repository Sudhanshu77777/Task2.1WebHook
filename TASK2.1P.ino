#include <DHT.h>
#include <WiFiNINA.h>
#include "secrets.h"
#include <ThingSpeak.h> // always include thingspeak header file after other header files and custom macros

#define DHTPIN 2 
#define DHTTYPE DHT22 

char ssid[] = SECRET_SSID ;    
char pass[] = SECRET_PASS;   

WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY ;

DHT dht(DHTPIN, DHTTYPE);

void setup() 
{
  Serial.begin(9600);  // Initialize serial 
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected!");

  Serial.println(F("DHTxx test!"));
  dht.begin();
  ThingSpeak.begin(client);  // Initialize ThingSpeak 
}

void loop() 
{
   // Connect or reconnect to WiFi
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Print sensor values of temperature and humidity to Serial Monitor

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
 
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // set the fields with the values
  ThingSpeak.setField(1, t);
  ThingSpeak.setField(2, h);
  
  // write to the ThingSpeak channel 
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  
  delay(60000); // Wait 20 seconds to update the channel again
}
