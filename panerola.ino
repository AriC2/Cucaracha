// --- DEFINICIÓ DE PINS I VARIABLES ---
#define P_echo 11   
#define P_trig 12   

const int Motorpin1 = 5;  
const int Motorpin2 = 6;  
const int Motorpin3 = 9;  
const int Motorpin4 = 10; 

long duracion, distancia; 

int led = 13;       
int lAzul = 8;      
int LVerde = 7;     



void setup() {
  Serial.begin(9600); 
  
  // Configuració de modes de pin
  pinMode(Motorpin1, OUTPUT);
  pinMode(Motorpin2, OUTPUT);
  pinMode(Motorpin3, OUTPUT);
  pinMode(Motorpin4, OUTPUT);
  pinMode(P_echo, INPUT);    
  pinMode(P_trig, OUTPUT);   
  pinMode(led, OUTPUT);
  pinMode(lAzul, OUTPUT);
  pinMode(LVerde, OUTPUT);
  
  // Estat inicial LEDs
  digitalWrite(led, LOW);
  digitalWrite(lAzul, LOW);
  digitalWrite(LVerde, LOW);

  // === BALL DE LA CUCARACHA (Inici) ===
  // Dreta
  digitalWrite(Motorpin1, HIGH); digitalWrite(Motorpin2, LOW);
  digitalWrite(Motorpin3, LOW);  digitalWrite(Motorpin4, HIGH);
  delay(400);
  // Esquerra
  digitalWrite(Motorpin1, LOW);  digitalWrite(Motorpin2, HIGH);
  digitalWrite(Motorpin3, HIGH); digitalWrite(Motorpin4, LOW);
  delay(400);
  // Dreta
  digitalWrite(Motorpin1, HIGH); digitalWrite(Motorpin2, LOW);
  digitalWrite(Motorpin3, LOW);  digitalWrite(Motorpin4, HIGH);
  delay(400);
  // Esquerra
  digitalWrite(Motorpin1, LOW);  digitalWrite(Motorpin2, HIGH);
  digitalWrite(Motorpin3, HIGH); digitalWrite(Motorpin4, LOW);
  delay(400);
  // Recte
  digitalWrite(Motorpin1, HIGH); digitalWrite(Motorpin2, LOW);
  digitalWrite(Motorpin3, HIGH); digitalWrite(Motorpin4, LOW);
  delay(600);
  // Enrere
  digitalWrite(Motorpin1, LOW);  digitalWrite(Motorpin2, HIGH);
  digitalWrite(Motorpin3, LOW);  digitalWrite(Motorpin4, HIGH);
  delay(600);
  // Aturada
  digitalWrite(Motorpin1, LOW); digitalWrite(Motorpin2, LOW);
  digitalWrite(Motorpin3, LOW); digitalWrite(Motorpin4, LOW);
  delay(1000);
}



void loop() {
  // --- LECTURA DEL SENSOR ---
  digitalWrite(P_trig, LOW); 
  delayMicroseconds(2); 
  digitalWrite(P_trig, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(P_trig, LOW);

  duracion = pulseIn(P_echo, HIGH); 
  distancia = duracion / 58.2;     

  // --- LÒGICA DE MOVIMENT ---
  if(distancia >= 35 && distancia <= 300) {
    // CAMÍ LLIURE: Recte i LED Verd
    digitalWrite(led, LOW);
    digitalWrite(lAzul, LOW);
    digitalWrite(LVerde, HIGH);
    digitalWrite(Motorpin1, HIGH);
    digitalWrite(Motorpin2, LOW);
    digitalWrite(Motorpin3, HIGH);
    digitalWrite(Motorpin4, LOW);
  } 
  else if(distancia >= 10 && distancia < 35) {
    // OBSTACLE A PROP: Gir i LED Blau
    digitalWrite(led, LOW);
    digitalWrite(lAzul, HIGH);
    digitalWrite(LVerde, LOW);
    digitalWrite(Motorpin1, LOW);
    digitalWrite(Motorpin2, HIGH);
    digitalWrite(Motorpin3, LOW);
    digitalWrite(Motorpin4, LOW);
  } 
  else {
    // PERILL: Aturada i LED Vermell
    digitalWrite(led, HIGH);
    digitalWrite(lAzul, LOW);
    digitalWrite(LVerde, LOW);
    digitalWrite(Motorpin1, LOW);
    digitalWrite(Motorpin2, LOW);
    digitalWrite(Motorpin3, LOW);
    digitalWrite(Motorpin4, LOW);
  }
}
