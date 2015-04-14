#include <PubSubClient.h>
#include <YunClient.h>
#include <DHT.h>
#include <SPI.h>

#define DHTPIN 2 //reading from pin #2
#define DHTTYPE DHT22 //DHT is the temperature sensor

void callback(char* topic, byte* payload, unsigned int length) {
// handle message arrived
}


//ETL's Arduino Yun Mac address; identifies the unique address for IBM to listen in on
byte mac[] = { 0x90, 0xA2, 0xDA, 0xF0, 0x4E, 0x08 };
char macstr[] = "90a2daf04e08";

//IBM's MQTT connection information 
char serverName[] = "quickstart.messaging.internetofthings.ibmcloud.com";
//char serverName[] = "192.168.252.177"; //my local moquitto server to test on
String clientName = String("d:quickstart:arduino:") + macstr;
String topicName = String("iot-2/evt/status/fmt/json");

YunClient yun;
PubSubClient client(serverName, 1883, callback, yun); //The port is in here!
DHT dht(DHTPIN, DHTTYPE);

//Initializing temperature variables
float humid = 0;
float tempC = 0;
float tempF = 0;

void setup()
{
Bridge.begin();
Serial.begin(9600);
delay(2000);
}

void loop(){
//converting strings into characters as pubsub library requests
char topicStr[26];
topicName.toCharArray(topicStr,26);
char clientStr[34];
clientName.toCharArray(clientStr,34);


if (!client.connected())
  Serial.println ("Trying to connect...");
  Serial.println (clientStr);
  client.connect(clientStr);
  if (client.connected())
    Serial.println ("Connected!");
    getData();
    Serial.println ("Data collected");
    String json = buildJson();
    Serial.println ("Json formatted"); 
    char jsonStr[200]; 
    json.toCharArray(jsonStr,200);

    boolean pubresult = client.publish(topicStr,jsonStr); //store the success result

Serial.print("attempt to send ");
Serial.println(jsonStr);
Serial.print("to ");
Serial.println(topicStr);

if (pubresult)
  Serial.println("successfully sent");
else
  Serial.println("unsuccessfully sent");
//client.loop();
delay(5000); //5 second pauses in between

}

String buildJson() {
//Just a bunch of formatting to make the string palatable
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
  return data;
}

void getData() {
  //Reading temperature from DHT sensor
  humid = dht.readHumidity();
  tempC = dht.readTemperature();
  tempF = dht.readTemperature(true); 
}
