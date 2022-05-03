int rain_val_analog;

void setup()
{
  pinMode(A0, INPUT);
  Serial.begin(115200);
}

void loop()
{
  rain_val_analog=analogRead(A0); 
 Serial.print("Analog value : ");
 Serial.println(rain_val_analog); 
 Serial.println("");
  delay(100);
}
