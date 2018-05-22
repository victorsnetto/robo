#include <Thread.h>
#include <ThreadController.h>
#include <Ultrasonic.h>
 
//Define os pinos para o trigger e echo
#define pino_trigger 4
#define pino_echo 5

int pino_D0 = 7;
int pino_A0 = A1;
 
int valor_a = 0;
int valor_d = 0;

//Inicializa o sensor nos pinos definidos acima
Ultrasonic ultrasonic(pino_trigger, pino_echo);

// ThreadController that will controll all threads
ThreadController controll = ThreadController();

//My Thread (as a pointer)
Thread* myThread = new Thread();
//His Thread (not pointer)
Thread hisThread = Thread();

// callback for myThread
void niceCallback(){
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

// callback for hisThread
void boringCallback(){
	int valor_a = analogRead(pino_A0);
  int valor_d = digitalRead(pino_D0);
 
  if (valor_d != 1)
  {
    Serial.println("Fogo detectado !!!");
  }
}

void setup(){
	Serial.begin(9600);

	// Configure myThread
	myThread->onRun(niceCallback);
	myThread->setInterval(500);

	// Configure myThread
	hisThread.onRun(boringCallback);
	hisThread.setInterval(500);

	// Adds both threads to the controller
	controll.add(myThread);
	controll.add(&hisThread); // & to pass the pointer to it

  Serial.println("Lendo dados do sensor...");
  pinMode(pino_A0, INPUT);
  pinMode(pino_D0, INPUT);
}

void loop(){
	// run ThreadController
	// this will check every thread inside ThreadController,
	// if it should run. If yes, he will run it;
	controll.run();

	// Rest of code
	float h = 3.1415;
	h/=2;
}
