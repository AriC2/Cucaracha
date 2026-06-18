// ======================================================
// CONFIGURACIÓ DE PINS I CONSTANTS
// ======================================================

// --- SENSOR ULTRASONS DAVANT ---
#define P_echo_D 11    // Pin per rebre l'eco del sensor frontal
#define P_trig_D 12    // Pin per enviar el pols del sensor frontal

// --- SENSOR ULTRASONS CUL (DARRERE) ---
#define P_echo_C 9     // Pin per rebre l'eco del sensor posterior
#define P_trig_C 10    // Pin per enviar el pols del sensor posterior

// --- PINS MOTORS (CONTROL DEL MOVIMENT) ---
const int Motorpin1 = 2; // Motor esquerre pol 1
const int Motorpin2 = 3; // Motor esquerre pol 2
const int Motorpin3 = 4; // Motor dret pol 1
const int Motorpin4 = 5; // Motor dret pol 2

// --- PINS LEDS (INDICADORS VISUALS) ---
int ledVermell = 13;     // Llum d'alerta (massa a prop)
int ledGroc = 8;        // Llum de detecció (esquivant)
int ledVerd = 7;        // Llum de camí lliure

// ======================================================
// CONFIGURACIÓ INICIAL (SETUP)
// ======================================================
void setup() {
  Serial.begin(9600);   // Iniciem la comunicació sèrie per veure dades al PC
 
  // Configurem els pins dels motors com a sortida
  pinMode(Motorpin1, OUTPUT);
  pinMode(Motorpin2, OUTPUT);
  pinMode(Motorpin3, OUTPUT);
  pinMode(Motorpin4, OUTPUT);

  // Configurem els pins dels sensors d'ultrasons
  pinMode(P_echo_D, INPUT);   // Entrada de dades
  pinMode(P_trig_D, OUTPUT);  // Sortida de pols
  pinMode(P_echo_C, INPUT);   // Entrada de dades
  pinMode(P_trig_C, OUTPUT);  // Sortida de pols

  // Configurem els pins dels LEDs com a sortida
  pinMode(ledVermell, OUTPUT);
  pinMode(ledGroc, OUTPUT);
  pinMode(ledVerd, OUTPUT);

  // Fem una petita seqüència de moviment en encendre el robot
  ejecutarBaile();
}

// ======================================================
// BUCLE PRINCIPAL (LOOP)
// ======================================================
void loop() {
  // Llegim les distàncies actuals dels dos sensors en cm
  long distDavant = llegirDistancia(P_trig_D, P_echo_D);
  long distCul = llegirDistancia(P_trig_C, P_echo_C);

  // Apaguem tots els LEDs per decidir quin encendre ara
  digitalWrite(ledVermell, LOW);
  digitalWrite(ledGroc, LOW);
  digitalWrite(ledVerd, LOW);

  // --- LOGICA DELS LEDS ---
  // Si hi ha un obstacle davant (fins a 35cm) o darrere (fins a 10cm)
  if ((distDavant > 0 && distDavant <= 35) || (distCul > 0 && distCul <= 10)) {
    if (distDavant <= 5 && distDavant > 0) {
      digitalWrite(ledVermell, HIGH); // Alerta màxima: obstacle a 5cm o menys
    } else {
      digitalWrite(ledGroc, HIGH);    // Obstacle detectat a més de 5cm
    }
  } else {
    digitalWrite(ledVerd, HIGH);      // Tot lliure: llum verda
  }

  // --- LÒGICA DE MOVIMENT (PRIORITATS) ---

  // 1. Prioritat: Si algú ens ve pel darrere (a menys de 10cm)
  if(distCul > 0 && distCul <= 10) {
    mover(1, 0, 1, 0); // Fuig cap endavant
    delay(500);        // Durant mig segon
    mover(1, 0, 0, 1); // Gira per canviar de ruta
    delay(400);
  }
  // 2. Si tenim un obstacle davant molt a prop (a menys de 10cm)
  else if(distDavant > 0 && distDavant <= 10) {
    mover(0, 1, 0, 1); // Marxa enrere per seguretat
    delay(600);
    mover(1, 0, 0, 1); // Gira per esquivar
    delay(500);
  }
  // 3. Si detectem obstacle però encara està lluny (entre 10 i 35cm)
  else if(distDavant > 10 && distDavant <= 35) {
    mover(1, 0, 0, 1); // Gira directament sense anar enrere
  }
  // 4. Si no hi ha res enlloc
  else {
    avanzar(); // Camí net, segueix recte
  }
 
  delay(50); // Petita pausa per no saturar el processador
}

// ======================================================
// FUNCIONS AUXILIARS
// ======================================================

// Funció per calcular la distància amb el sensor d'ultrasons
long llegirDistancia(int trig, int echo) {
  digitalWrite(trig, LOW);           // Netegem el pin del disparador
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);          // Enviem un pols de 10 microsegons
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
 
  // Mesurem quant triga a tornar l'eco (temps en microsegons)
  long duracion = pulseIn(echo, HIGH, 25000);
 
  if (duracion == 0) return 999;     // Si triga massa, assumim que no hi ha res
  return duracion / 58.2;            // Convertim el temps en centímetres
}

// Funció per controlar els 4 pins dels motors alhora
void mover(int p1, int p2, int p3, int p4) {
  digitalWrite(Motorpin1, p1);
  digitalWrite(Motorpin2, p2);
  digitalWrite(Motorpin3, p3);
  digitalWrite(Motorpin4, p4);
}

// Funció simple per anar recte endavant
void avanzar() {
  mover(1, 0, 1, 0); // Activa pols positius d'ambdós motors
}

// Seqüència de moviments inicial (baile)
void ejecutarBaile() {
  mover(1,0,1,0); delay(400);  // Endavant un moment
  mover(0,1,0,1); delay(400);  // Enrere un moment
  mover(0,0,0,0); delay(1000); // Aturat
}
