#include <PubSubClient.h>
#include <YunClient.h>
#include <DHT.h>
#include <SPI.h>

#define DHTPIN 2 
#define DHTTYPE DHT22

void callback(char* topic, byte* payload, unsigned int length) {
// handle message arrived
}

YunClient yun;
PubSubClient client("192.168.252.177", 1885, callback, yun);
DHT dht(DHTPIN, DHTTYPE);

  float humid = dht.readHumidity();
  // Read temperature as Celsius
  float tempC = dht.readTemperature();
  // Read temperature as Fahrenheit
  float tempF = dht.readTemperature(true);
  
void setup()
{

Bridge.begin();

client.connect("yun");
client.publish("/sensor/test","this works better!");
client.subscribe("/sensor/test");

}

void loop()
{
getData();
//client.loop();
}

String buildJson() {
  String data = "{";
  data+="\n";
  data+= "\"d\": {";
  data+="\n";
  data+="\"myName\": \"Arduino DHT\",";
  data+="\n";
  data+="\"temperature (F)\": ";
  data+=(int)tempF;
  data+= ",";
  data+="\n";
  data+="\"temperature (C)\": ";
  data+=(int)tempC;
  data+= ",";
  data+="\n";
  data+="\"humidity\": ";
  data+=(int)humid;
  data+="\n";
  data+="}";
  data+="\n";
  data+="}";
  Serial.println(data);
  return data;
}

void getData() {
  int chk = dht.read();
  switch (chk)
  {
  case 0: 
    Serial.println("Read OK"); 
    tempF = dht.readTemperature(true);
    tempC = dht.readTemperature();
    humid = dht.readHumidity();
    break;
  case -1: 
    Serial.println("Checksum error"); 
    break;
  case -2: 
    Serial.println("Time out error"); 
    break;
  default: 
    Serial.println("Unknown error"); 
    break;
  }
}
