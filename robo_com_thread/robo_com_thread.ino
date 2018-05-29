#include <Thread.h>
#include <ThreadController.h>
#include <Ultrasonic.h>
 
//Define os pinos para o trigger e echo
#define pino_trigger 4
#define pino_echo 5

int pino_D0 = 6;
int pino_A0 = A0;
int pino_D0_2 = 7;
int pino_A0_2 = A1;
int valor_a = 0;
int valor_d = 0;
int valor_a_2 = 0;
int valor_d_2 = 0;

//Inicializa o sensor nos pinos definidos acima
Ultrasonic ultrasonic(pino_trigger, pino_echo);

// ThreadController that will controll all threads
ThreadController controll = ThreadController();

//Sonic Thread (pointer)
Thread* sonicThread = new Thread();
//Infra 1 Thread (not pointer)
Thread infra1Thread = Thread();
//Infra 2 Thread (not pointer)
Thread infra2Thread = Thread();

// callback for roboThread
void sonicCallback(){
	//Le as informacoes do sensor, em cm e pol
  float cmMsec, inMsec;
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
void infra1Callback(){
	int valor_a = analogRead(pino_A0);
  int valor_d = digitalRead(pino_D0);
 
  if (valor_d != 1)
  {
    Serial.println("Fogo detectado !!!");
  }
}

// callback for infra2Thread
void infra2Callback(){
  int valor_a_2 = analogRead(pino_A0_2);
  int valor_d_2 = digitalRead(pino_D0_2);
 
  if (valor_d_2 != 1)
  {
    Serial.println("Fogo detectado !!!");
  }
}

void setup(){
	Serial.begin(9600);

	// Configure roboThread
	sonicThread->onRun(sonicCallback);
	sonicThread->setInterval(500);

	// Configure infra1Thread
	infra1Thread.onRun(infra1Callback);
	infra1Thread.setInterval(500);

  // Configure infra2Thread
  infra2Thread.onRun(infra2Callback);
  infra2Thread.setInterval(500);

	// Adds both threads to the controller
	controll.add(sonicThread);
	controll.add(&infra1Thread); // & to pass the pointer to it
  controll.add(&infra2Thread); // & to pass the pointer to it

  Serial.println("Lendo dados do sensor...");
  pinMode(pino_A0, INPUT);
  pinMode(pino_D0, INPUT);
}

void loop(){
	// run ThreadController
	// this will check every thread inside ThreadController,
	// if it should run. If yes, he will run it;
	controll.run();

}
