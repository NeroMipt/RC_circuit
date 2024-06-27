void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(!Serial.available());
  int incByte = Serial.read();
  if(incByte == 49 || incByte == 48)
  {
    int i = 0; 
    while(i < 642)
    {
      i+=5;
      Serial.print(i);
      Serial.print(",");
      delay(50);
    }
  }
}
