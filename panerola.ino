// --- DEFINICIÓN DE PINS ---
#define P_echo 11
#define P_trig 12

const int Motorpin1 = 2;
const int Motorpin2 = 3;
const int Motorpin3 = 4;
const int Motorpin4 = 5;

int led = 13;
int lAzul = 8;
int LVerde = 7;

long duracion, distancia;

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

  // --- BAILE DE LA CUCARACHA (Independiente) ---
  baileCucaracha();
}

void loop() {
  // 1. LECTURA DEL SENSOR
  digitalWrite(P_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(P_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(P_trig, LOW);

  duracion = pulseIn(P_echo, HIGH);
  distancia = duracion / 58.2;

  // Imprimir en monitor serie para debug
  Serial.print("Distancia: ");
  Serial.println(distancia);

  // 2. DECISIONES SEGÚN DISTANCIA
  // Si no detecta nada (0) o está muy lejos, asumimos camino libre
  if(distancia > 35 || distancia == 0){
    avanzar();
  } 
  else if(distancia >= 10 && distancia <= 35){
    girar();
  } 
  else {
    detener();
  }

  delay(50); // Pequeña pausa para estabilizar lecturas
}

// --- FUNCIONES DE MOVIMIENTO ---

void baileCucaracha() {
  for(int i=0; i<3; i++) { // Repite el paso 3 veces
    digitalWrite(Motorpin1, HIGH); digitalWrite(Motorpin2, LOW);
    digitalWrite(Motorpin3, LOW);  digitalWrite(Motorpin4, HIGH);
    delay(200);
    digitalWrite(Motorpin1, LOW);  digitalWrite(Motorpin2, HIGH);
    digitalWrite(Motorpin3, HIGH); digitalWrite(Motorpin4, LOW);
    delay(200);
  }
  detener();
  delay(1000); // Pausa antes de empezar a detectar
}

void avanzar() {
  digitalWrite(LVerde, HIGH); digitalWrite(lAzul, LOW); digitalWrite(led, LOW);
  digitalWrite(Motorpin1, HIGH);
  digitalWrite(Motorpin2, LOW);
  digitalWrite(Motorpin3, HIGH);
  digitalWrite(Motorpin4, LOW);
}

void girar() {
  digitalWrite(LVerde, LOW); digitalWrite(lAzul, HIGH); digitalWrite(led, LOW);
  // Giro sobre su propio eje
  digitalWrite(Motorpin1, LOW);
  digitalWrite(Motorpin2, HIGH);
  digitalWrite(Motorpin3, HIGH);
  digitalWrite(Motorpin4, LOW);
}

void detener() {
  digitalWrite(LVerde, LOW); digitalWrite(lAzul, LOW); digitalWrite(led, HIGH);
  digitalWrite(Motorpin1, LOW);
  digitalWrite(Motorpin2, LOW);
  digitalWrite(Motorpin3, LOW);
  digitalWrite(Motorpin4, LOW);
}
