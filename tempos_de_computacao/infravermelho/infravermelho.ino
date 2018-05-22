//Programa: Sensor de Chama Arduino
//Autor: FILIEFLOP
 
int pino_D0 = 7;
int pino_A0 = A1;

int led = 10;
unsigned long time1 = 0;
unsigned long time2 = 0;
unsigned long computacao = 0;

int valor_a = 0;
int valor_d = 0;
 
void setup()
{
  Serial.begin(9600);
  pinMode(pino_A0, INPUT);
  pinMode(pino_D0, INPUT);
  pinMode(led, OUTPUT);
}
 
void loop()
{
  time1 = micros();
  int valor_a = analogRead(pino_A0);
  int valor_d = digitalRead(pino_D0);
  time2 = micros();
  computacao = time2 - time1;
  Serial.println("Tempo: ");
  Serial.print(computacao);
  Serial.println("");

  digitalWrite(led, LOW);
  Serial.print("Porta analogica: ");
  Serial.print(valor_a);
  Serial.print(" Porta digital: ");
  Serial.println(valor_d);
 
  if (valor_d != 1)
  {
    Serial.println("Fogo detectado !!!");
    digitalWrite(led, HIGH);
    delay(500);
  }
}
