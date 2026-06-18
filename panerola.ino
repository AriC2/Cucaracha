// --- SENSOR DAVANT ---
#define P_echo_D 11
#define P_trig_D 12

// --- SENSOR CUL ---
#define P_echo_C 9
#define P_trig_C 10

// --- PINS MOTORS ---
const int Motorpin1 = 2;
const int Motorpin2 = 3;
const int Motorpin3 = 4;
const int Motorpin4 = 5;

// --- PINS LEDS ---
int led = 13;    // Vermell
int lAzul = 8;   // Blau
int LVerde = 7;  // Verd

void setup() {
  Serial.begin(9600);
 
  pinMode(Motorpin1, OUTPUT);
  pinMode(Motorpin2, OUTPUT);
  pinMode(Motorpin3, OUTPUT);
  pinMode(Motorpin4, OUTPUT);

  pinMode(P_echo_D, INPUT);
  pinMode(P_trig_D, OUTPUT);
  pinMode(P_echo_C, INPUT);
  pinMode(P_trig_C, OUTPUT);

  pinMode(led, OUTPUT);
  pinMode(lAzul, OUTPUT);
  pinMode(LVerde, OUTPUT);

  ejecutarBaile();
}

void loop() {
  long distDavant = llegirDistancia(P_trig_D, P_echo_D);
  long distCul = llegirDistancia(P_trig_C, P_echo_C);

  // DEBUG per consola
  Serial.print("Davant: "); Serial.print(distDavant);
  Serial.print(" | Cul: "); Serial.println(distCul);

  // 1. SI DETECTA ALGO AL CUL (<10cm): Fuig endavant i gira
  if(distCul > 0 && distCul <= 10) {
    digitalWrite(LVerde, HIGH); digitalWrite(led, HIGH);
    mover(1, 0, 1, 0); // Endavant
    delay(500);
    mover(1, 0, 0, 1); // Gira dreta
    delay(400);
  }
  // 2. SI DETECTA ALGO DAVANT MOLT A PROP (<10cm): Tira enrere i gira
  else if(distDavant > 0 && distDavant < 10) {
    digitalWrite(led, HIGH);
    mover(0, 1, 0, 1); // Enrere
    delay(600);
    mover(1, 0, 0, 1); // Gira
    delay(500);
  }
  // 3. SI DETECTA ALGO DAVANT A PROP (10-35cm): Esquiva girant
  else if(distDavant >= 10 && distDavant <= 35) {
    digitalWrite(lAzul, HIGH);
    mover(1, 0, 0, 1);
  }
  // 4. CAMÍ LLIURE: Avança
  else {
    avanzar();
  }
 
  delay(50);
}

long llegirDistancia(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long duracion = pulseIn(echo, HIGH, 25000);
  if (duracion == 0) return 0;
  return duracion / 58.2;
}

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

void ejecutarBaile() {
  mover(1,0,1,0); delay(400);
  mover(0,1,0,1); delay(400);
  mover(0,0,0,0); delay(1000);
}
