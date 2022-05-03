/*
Agu 2018
Author: Sarful hassan
Website: https://mechatronicslabrpi.blogspot.com
*/ 



//library esp
#include <ESP8266WiFi.h>
const char* ssid = "JP";                                  //fill in your wifi name
const char* password = "asdffdsa";                              //fill in your wifi password


String MakerIFTTT_Key ;
String MakerIFTTT_Event;
char *append_str(char *here, String s) {  int i=0; while (*here++ = s[i]){i++;};return here-1;}
char *append_ul(char *here, unsigned long u) { char buf[20]; return append_str(here, ultoa(u, buf, 10));}
char post_rqst[256];char *p;char *content_length_here;char *json_start;int compi;

WiFiClient client;

int sensorPin = A0;    // input for LDR and rain sensor
int enable2 = 13;      // enable reading Rain sensor
int sensorValue2 = 0;  // variable to store the value coming from sensor Rain sensor


int rain_alert_count = 0;

//--------------------------setup-------------------------
void setup() {

// declare the enable and ledPin as an OUTPUT:
//pinMode(enable2, OUTPUT);

  
Serial.begin(115200);
delay(10);


WiFi.begin(ssid, password);

Serial.println();
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);
Serial.print("..........");
Serial.println();
WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
delay(500);

}
Serial.println("WiFi connected");
Serial.println();

}


void loop() {



//--------------------------Rain Sensor-------------------------



delay(500);
sensorValue2 = analogRead(sensorPin);
sensorValue2 = constrain(sensorValue2, 150, 440); 
sensorValue2 = map(sensorValue2, 150, 440, 1023, 0); 
if (sensorValue2>= 20)
{
  Serial.print("rain is detected");
  if (rain_alert_count == 0)
  {
    rain_alert();
    rain_alert_count=1;
  }
  
//digitalWrite(enable2, HIGH);
  }
  else
  
{
  Serial.print("rain not detected");
 // digitalWrite(enable2, LOW); 
  }
Serial.print("Rain value:       ");
Serial.println(sensorValue2);
Serial.println();

delay(100);



}


void rain_alert()
{
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
