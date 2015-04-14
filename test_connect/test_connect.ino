  #include <PubSubClient.h>
  #include <YunClient.h>

void callback(char* topic, byte* payload, unsigned int length) {
// handle message arrived
}

YunClient yun;
PubSubClient client("192.168.252.177", 1885, callback, yun);

void setup()
{

Bridge.begin();

client.connect("yun");
client.publish("/sensor/test","this works better!");
client.subscribe("/sensor/test");

}

void loop()
{
client.loop();
}
