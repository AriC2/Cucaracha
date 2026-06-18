// --- PINS DEL SENSOR ---
#define P_echo 11
#define P_trig 12

// --- PINS MOTORES ---
const int Motorpin1 = 2;
const int Motorpin2 = 3;
const int Motorpin3 = 4;
const int Motorpin4 = 5;

// --- PINS LEDS ---
int led = 13;    // Rojo (Parado / Error)
int lAzul = 8;   // Azul (Girando)
int LVerde = 7;  // Verde (Avanzando)

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

  // --- 1. BAILE DE LA CUCARACHA (Independiente del sensor) ---
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

  // pulseIn con TIMEOUT de 30ms para que no se bloquee si no hay sensor
  duracion = pulseIn(P_echo, HIGH, 30000); 
  distancia = duracion / 58.2;

  // DEBUG: Para ver en el PC qué está pasando
  Serial.print("Distancia: ");
  Serial.println(distancia);

  // --- 2. LÓGICA DE DETECCIÓN ---
  // Si distancia es 0 (no hay sensor) o mayor a 35, avanza
  if(distancia > 35 || distancia == 0) {
    avanzar();
  } 
  // Si detecta algo entre 10 y 35 cm, gira
  else if(distancia >= 10 && distancia <= 35) {
    esquivar();
  } 
  // Si está muy cerca, para
  else {
    detener();
  }
  
  delay(50); 
}

// --- FUNCIONES DE MOVIMIENTO ---

void ejecutarBaile() {
  // Adelante, Atrás, Derecha, Izquierda, Derecha, Izquierda, Atrás, Adelante
  mover(1,0,1,0); delay(400); // Adelante
  mover(0,1,0,1); delay(400); // Atrás
  mover(1,0,0,1); delay(300); // Derecha
  mover(0,1,1,0); delay(300); // Izquierda
  mover(1,0,0,1); delay(300); // Derecha
  mover(0,1,1,0); delay(300); // Izquierda
  mover(0,1,0,1); delay(400); // Atrás
  mover(1,0,1,0); delay(400); // Adelante
  detener();
  delay(1000); 
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

void esquivar() {
  digitalWrite(LVerde, LOW); digitalWrite(lAzul, HIGH); digitalWrite(led, LOW);
  mover(1, 0, 0, 1); // Giro sobre su eje
}

void detener() {
  digitalWrite(LVerde, LOW); digitalWrite(lAzul, LOW); digitalWrite(led, HIGH);
  mover(0, 0, 0, 0);
}
