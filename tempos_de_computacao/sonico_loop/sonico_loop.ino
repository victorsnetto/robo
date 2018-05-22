const int trigpinfrente=3;  //Alocação do primeiro bit (trig) do sensor ultrassonico da frente no pino 3 
const int echopinfrente=2;  //Alocação do segundo bit (echo) do sensor ultrassonico da frente no pino 2 

int distanciafrente=0;  //Declaração de uma variavel responsavel por receber a distancia aferida pelo sensor ultrassonico da frente 

unsigned long tempo=0;  //Declaração de uma variavel para controle do tempo do combate 
unsigned long tempoanterior=0;  //Declaraçâo de ume variavel para controle do tempo do combate 


int i;

void setup() {       //Declaração do modo dos pinos como entrada ou saida
pinMode(trigpinfrente,OUTPUT);    //      ''
pinMode(echopinfrente,INPUT);     //      ''
}

int ultrassonicofrente() { // Função responsável por retornar a distância aferida pelo sensor ultrassonico da frente 
digitalWrite(trigpinfrente,LOW); 
delayMicroseconds(2); 
digitalWrite(trigpinfrente,HIGH); 
delayMicroseconds(10); 
digitalWrite(trigpinfrente,LOW); 
unsigned long duracao=pulseIn(echopinfrente,HIGH); 
distanciafrente=duracao/58; 
return(distanciafrente);
}

void loop(){
Serial.print(ultrassonicofrente());

}
