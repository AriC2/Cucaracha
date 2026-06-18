// --- PINS DEL SENSOR ---
#define P_echo 11
#define P_trig 12

// --- PINS MOTORES ---
const int Motorpin1 = 2;
const int Motorpin2 = 3;
const int Motorpin3 = 4;
const int Motorpin4 = 5;

// --- PINS LEDS ---
int led = 13;    // Vermell (Maniobra enrere)
int lAzul = 8;   // Blau (Girant)
int LVerde = 7;  // Verd (Avançant)

void setup() {
  Serial.begin(9600);
 
  pinMode(Motorpin1, OUTPUT);
  pinMode(Motorpin2, OUTPUT);
  pinMode(Motorpin3, OUTPUT);
  pinMode(Motorpin4, OUTPUT);

  pinMode(P_echo, INPUT);
  pinMode(P_trig, OUTPUT);

  pinMode(led, OUTPUT);
  pinMode(lAzul, OUTPUT);
  pinMode(LVerde, OUTPUT);

  // --- 1. BALL DE LA CUCARACHA ---
  ejecutarBaile();
}

void loop() {
  long duracion, distancia;

  // LANZAR PULSO
  digitalWrite(P_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(P_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(P_trig, LOW);

  duracion = pulseIn(P_echo, HIGH, 30000);
  distancia = duracion / 58.2;

  // DEBUG
  Serial.print("Distancia: ");
  Serial.println(distancia);

  // --- 2. LÒGICA DE DETECCIÓ MILLORADA ---
 
  if(distancia > 35 || distancia == 0) {
    // CAMINA
    avanzar();
  }
  else if(distancia >= 10 && distancia <= 35) {
    // GIRA (Obstacle a mitja distància)
    esquivar();
  }
  else {
    // TIRA ENRERE, GIRA I CAMINA (Obstacle molt a prop)
    maniobraEvasiva();
  }
 
  delay(50);
}

// --- FUNCIONS DE MOVIMENT ---

void mover(int p1, int p2, int p3, int p4) {
  digitalWrite(Motorpin1, p1);
  digitalWrite(Motorpin2, p2);
  digitalWrite(Motorpin3, p3);
  digitalWrite(Motorpin4, p4);
}

void avanzar() {
  digitalWrite(LVerde, HIGH); digitalWrite(lAzul, LOW); digitalWrite(led, LOW);
  mover(1, 0, 1, 0);
}

void esquivar() {
  digitalWrite(LVerde, LOW); digitalWrite(lAzul, HIGH); digitalWrite(led, LOW);
  mover(1, 0, 0, 1); // Giro sobre el seu eix
}

void maniobraEvasiva() {
  // Tira enrere
  digitalWrite(LVerde, LOW); digitalWrite(lAzul, LOW); digitalWrite(led, HIGH);
  mover(0, 1, 0, 1);
  delay(600); // Temps reculant
 
  // Gira
  digitalWrite(led, LOW); digitalWrite(lAzul, HIGH);
  mover(1, 0, 0, 1);
  delay(500); // Temps girant
 
  // Torna a avançar
  avanzar();
}

void ejecutarBaile() {
  mover(1,0,1,0); delay(400); // Endavant
  mover(0,1,0,1); delay(400); // Enrere
  mover(1,0,0,1); delay(300); // Dreta
  mover(0,1,1,0); delay(300); // Esquerra
  mover(1,0,0,1); delay(300); // Dreta
  mover(0,1,1,0); delay(300); // Esquerra
  mover(0,1,0,1); delay(400); // Enrere
  mover(1,0,1,0); delay(400); // Endavant
  mover(0,0,0,0);
  delay(1000);
}
