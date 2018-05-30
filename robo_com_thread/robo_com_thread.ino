#include <Thread.h>
#include <ThreadController.h>
#include <Ultrasonic.h>
 
//Define os pinos para o trigger e echo
#define pino_trigger 4
#define pino_echo 5

const byte mda1=11; //Alocaçâo do primeiro bit do motor da esquerda no pino 11
const byte mda2=6; //Alocaçâo do segundo bit do motor da esquerda no pino 10
const byte mea1=10; //Alocaçâo do primeiro bit do motor de direita no pino 9 
const byte mea2=8; //Alocaçâo do segundo bit do motor da direita no pino 8 

boolean estadosbf;   //Declaração de uma variavel para receber a leitura do sbf
boolean estadosbt;   //Declaração de uma variavel para receber a leitura do sbt

int pino_D0 = 6;
int pino_A0 = A0;
int pino_D0_2 = 7;
int pino_A0_2 = A1;
int valor_a = 0;
int valor_d = 0;
int valor_a_2 = 0;
int valor_d_2 = 0;

float cmMsec, inMsec;

void andarfrente() {  //Função andar pare frente 
digitalWrite(mea1,HIGH); 
digitalWrite(mea2,LOW); 
digitalWrite(mda1,HIGH); 
digitalWrite(mda2,LOW);
}

void andartras() {  //Função andar para trás 
digitalWrite(mea1,LOW); 
digitalWrite(mea2,HIGH); 
digitalWrite(mda1,LOW); 
digitalWrite(mda2,HIGH);
}

void parar() {  //Função parar o tobá 
digitalWrite(mea1,LOW); 
digitalWrite(mea2,LOW); 
digitalWrite(mda1,LOW); 
digitalWrite(mda2,LOW);
}

void girardireita(){  // Função girar para direita 
digitalWrite(mea1,HIGH); 
digitalWrite(mea2,LOW); 
digitalWrite(mda1,LOW); 
digitalWrite(mda2,HIGH);
}

void giraresquerda() {  // Função girar pare esquerda 
digitalWrite(mea1,LOW); 
digitalWrite(mea2,HIGH); 
digitalWrite(mda1,HIGH); 
digitalWrite(mda2,LOW);
}

//Inicializa o sensor nos pinos definidos acima
Ultrasonic ultrasonic(pino_trigger, pino_echo);

// ThreadController that will controll all threads
ThreadController controll = ThreadController();

//Sonic Thread (pointer)
Thread* sonicThread = new Thread();
//Infra  Thread (not pointer)
Thread infraThread = Thread();
//Actions Thread(not pointer)
Thread actionThread = Thread();

// callback for roboThread
void sonicCallback(){
	//Le as informacoes do sensor, em cm e pol

  int time1 = micros();
  long microsec = ultrasonic.timing();
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  int time2 = micros();
  int computacao = time2 - time1;
  inMsec = ultrasonic.convert(microsec, Ultrasonic::IN);
  //Exibe informacoes no serial monitor
  Serial.print("Distancia em cm: ");
  Serial.print(cmMsec);
  Serial.print(" - computaçao: ");
  Serial.println(computacao);
  
  
}

// callback for infraThread
void infraCallback(){
	valor_a = analogRead(pino_A0);
  valor_d = digitalRead(pino_D0);
  valor_a_2 = analogRead(pino_A0_2);
  valor_d_2 = digitalRead(pino_D0_2);
 
  if (valor_d != 1)
  {
    Serial.println("Fogo detectado !!!");
    estadosbf = 0;
  }
 
  if (valor_d_2 != 1)
  {
    Serial.println("Fogo detectado !!!");
    estadosbt = 0;
  }
}

// callback for actionThread
void actionCallback(){
  if(cmMsec >= 95){
    girardireita();}
  
  if(estadosbf==0) {  //Tomar a decisão de ir para tras caso algum sensor de borda na frente acusar o branco. Nota: 1 - Branco, 0- Preto 
    andartras();} 
  else if(estadosbt==0) { //Tomar a decisão de ir para frente caso algum sensor de borda atras acusar o branco. 
    andarfrente();} 
  else if(cmMsec<=95 && estadosbf==1){ // Tomar a decisão de ir para frente caso ache um oponente na frente a menos de 95 cm e os sensores de borda da frente estejam 
    andarfrente();} 
}

void setup(){
	Serial.begin(9600);

	// Configure sonicThread
	sonicThread->onRun(sonicCallback);
	sonicThread->setInterval(500);

	// Configure infraThread
	infraThread.onRun(infraCallback);
	infraThread.setInterval(500);

 // Configure actionThread
  actionThread.onRun(actionCallback);
  actionThread.setInterval(500);

	// Adds both threads to the controller
	controll.add(sonicThread);
	controll.add(&infraThread); // & to pass the pointer to it
  controll.add(&actionThread); // & to pass the pointer to it
  
  Serial.println("Lendo dados do sensor...");
  pinMode(pino_A0, INPUT);
  pinMode(pino_D0, INPUT);
  pinMode(pino_A0_2, INPUT);
  pinMode(pino_D0_2, INPUT);

  pinMode(mea1,OUTPUT);       //      ''
  pinMode(mea2,OUTPUT);       //      ''
  pinMode(mda1,OUTPUT);       //      ''
  pinMode(mda2,OUTPUT);       //      ''
  
}

void loop(){
	// run ThreadController
	// this will check every thread inside ThreadController,
	// if it should run. If yes, he will run it;
	controll.run();

}
