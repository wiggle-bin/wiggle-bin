// https://create.arduino.cc/projecthub/MisterBotBreak/how-to-use-a-rain-sensor-bcecd9
const int capteur_D = 4;
const int capteur_A = A0;

int val_analogique;

void setup()
{
  pinMode(capteur_D, INPUT);
  pinMode(capteur_A, INPUT);
  Serial.begin(9600);
}

void loop()
{
if(digitalRead(capteur_D) == LOW) 
  {
    Serial.println("Digital value : wet"); 
    delay(10); 
  }
else
  {
    Serial.println("Digital value : dry");
    delay(10); 
  }
val_analogique=analogRead(capteur_A); 
 Serial.print("Analog value : ");
 Serial.println(val_analogique); 
 Serial.println("");
 delay(0.1 * 60 * 1000);
}
