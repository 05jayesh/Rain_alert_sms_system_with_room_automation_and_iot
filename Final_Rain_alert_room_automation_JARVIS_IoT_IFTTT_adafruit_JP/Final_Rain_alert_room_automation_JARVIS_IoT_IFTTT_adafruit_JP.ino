#include<ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define Relay1  D1
#define Relay2  D2


#define WLAN_SSID "JP"
#define WLAN_PASS "asdffdsa"

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "05jayesh"
#define AIO_KEY         "23b02f3337964e6ebc2ebd8d91a6d586"

WiFiClient client;

int rain_val_analog;
int rain_alert_count = 0;
String MakerIFTTT_Key ;
String MakerIFTTT_Event;
char *append_str(char *here, String s) {  int i=0; while (*here++ = s[i]){i++;};return here-1;}
char *append_ul(char *here, unsigned long u) { char buf[20]; return append_str(here, ultoa(u, buf, 10));}
char post_rqst[256];char *p;char *content_length_here;char *json_start;int compi;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Subscribe lights = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/lights");
Adafruit_MQTT_Subscribe fan    = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/fan");



void setup()
{
  pinMode(A0, INPUT);

  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);


  digitalWrite(Relay1, LOW);
  digitalWrite(Relay2, LOW);


  void MQTT_connect();

  Serial.begin(115200);

  Serial.println();
  Serial.print("Connecting to");
  Serial.print("WLAN_SSID");

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  mqtt.subscribe(&lights);
  mqtt.subscribe(&fan);
}

void loop()
{
  MQTT_connect();
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(1000)))
  {
    if (subscription == &lights)
    {
      Serial.print(F("Got: "));
      Serial.println((char *)lights.lastread);
      int lights_state = atoi((char*)lights.lastread);
      digitalWrite(Relay1, lights_state);
    }
    if (subscription == &fan)
    {
      Serial.print(F("Got: "));
      Serial.println((char *)fan.lastread);
      int fan_state = atoi((char*)fan.lastread);
      digitalWrite(Relay2, fan_state);
    }
  }
  rain_val_analog = analogRead(A0);
  Serial.print("Analog value : ");
  Serial.println(rain_val_analog);
  Serial.println("");
  if (rain_val_analog > 200 && rain_val_analog < 900)
  {
    if (rain_alert_count == 0)
    {
    send_rain_alert();
    rain_alert_count = 1;
    }
  }
  delay(100);
}

void MQTT_connect()
{
  int8_t ret;

  if (mqtt.connected())
  {
    return;
  }
  Serial.print("Connecting to MQTT...");
  uint8_t retries = 1;

  while ((ret = mqtt.connect()) != 0)
  {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 3 seconds...");
    mqtt.disconnect();
    delay(500);
    //retries--;
    if (retries == 0)
    {
      while (1);
    }
  }
  Serial.println("MQTT connected!");
}

void send_rain_alert()
{
    Serial.println((WiFi.localIP().toString()));
  if (client.connect("maker.ifttt.com",80)) {
    MakerIFTTT_Key ="nbnlvOBOjaw5MUZXEjn2TVZ0cnaxezt-_tluP1JeYfq";
    MakerIFTTT_Event ="sms";
    p = post_rqst;
    p = append_str(p, "POST /trigger/");
    p = append_str(p, MakerIFTTT_Event);
    p = append_str(p, "/with/key/");
    p = append_str(p, MakerIFTTT_Key);
    p = append_str(p, " HTTP/1.1\r\n");
    p = append_str(p, "Host: maker.ifttt.com\r\n");
    p = append_str(p, "Content-Type: application/json\r\n");
    p = append_str(p, "Content-Length: ");
    content_length_here = p;
    p = append_str(p, "NN\r\n");
    p = append_str(p, "\r\n");
    json_start = p;
    p = append_str(p, "{\"value1\":\"");
    p = append_str(p, "9113376503");
    p = append_str(p, "\",\"value2\":\"");
    p = append_str(p, "JAYESH!!  It started RAINING!!!");
    p = append_str(p, "\",\"value3\":\"");
    p = append_str(p, "");
    p = append_str(p, "\"}");

    compi= strlen(json_start);
    content_length_here[0] = '0' + (compi/10);
    content_length_here[1] = '0' + (compi%10);
    client.print(post_rqst);

  }
}
