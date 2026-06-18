// --- PINS MOTORS ---
const int Motorpin1 = 3; const int Motorpin2 = 5;
const int Motorpin3 = 6; const int Motorpin4 = 9;
const int pinENA = 10; const int pinENB = 11;

const int pinVentilador = A0;
const int pinBuzzer = A4;
const int pinPolsador = 2; // Botón para encender/apagar el ROBOT
const int pinPolsadorVent = A5; // NUEVO: Botón para encender/apagar el VENTILADOR

// --- SENSORS ---
const int P_trig_D = 12; const int P_echo_D = 7;
const int P_trig_C = 4; const int P_echo_C = 8;

// --- LEDS ---
const int ledVermell = A1; const int ledGroc = A2; const int ledVerd = A3;

bool robotActiu = false;
bool ventiladorActiu = false; // NUEVO: Estado del ventilador

int ultimEstatPolsador = HIGH;
int ultimEstatPolsadorVent = HIGH; // NUEVO: Último estado del botón del ventilador

void setup() {
  pinMode(Motorpin1, OUTPUT); pinMode(Motorpin2, OUTPUT);
  pinMode(Motorpin3, OUTPUT); pinMode(Motorpin4, OUTPUT);
  pinMode(pinENA, OUTPUT); pinMode(pinENB, OUTPUT);
  pinMode(pinVentilador, OUTPUT); pinMode(pinBuzzer, OUTPUT);
  pinMode(ledVermell, OUTPUT); pinMode(ledGroc, OUTPUT); pinMode(ledVerd, OUTPUT);
  pinMode(P_trig_D, OUTPUT); pinMode(P_trig_C, OUTPUT);
  pinMode(P_echo_D, INPUT); pinMode(P_echo_C, INPUT);

  pinMode(pinPolsador, INPUT_PULLUP);
  pinMode(pinPolsadorVent, INPUT_PULLUP); // NUEVO: Configuración del botón del ventilador
}

void loop() {
  // --- CONTROL DEL BOTÓN DEL ROBOT ---
  int estatPolsador = digitalRead(pinPolsador);
  if (estatPolsador == LOW && ultimEstatPolsador == HIGH) {
    delay(50); // Antirebote
    robotActiu = !robotActiu;
    if (robotActiu) ejecutarBaile();
    else pararRobot();
    delay(300);
  }
  ultimEstatPolsador = estatPolsador;

  // --- CONTROL DEL BOTÓN DEL VENTILADOR ---
  int estatPolsadorVent = digitalRead(pinPolsadorVent);
  if (estatPolsadorVent == LOW && ultimEstatPolsadorVent == HIGH) {
    delay(50); // Antirebote
    ventiladorActiu = !ventiladorActiu; // Cambia el estado (ON/OFF)
    delay(300);
  }
  ultimEstatPolsadorVent = estatPolsadorVent;

  // Acción física sobre el ventilador según su estado guardado
  if (ventiladorActiu) {
    digitalWrite(pinVentilador, HIGH);
  } else {
    digitalWrite(pinVentilador, LOW);
  }

  // --- LÓGICA DE MOVIMIENTO AND SENSORES ---
  if (robotActiu) {

    long dDavant = llegirDistancia(P_trig_D, P_echo_D);
    long dCul = llegirDistancia(P_trig_C, P_echo_C);

    long dMin = min(dDavant, dCul);

    // --- LÒGICA DE LEDS ---
    digitalWrite(ledVermell, LOW);
    digitalWrite(ledGroc, LOW);
    digitalWrite(ledVerd, LOW);

    if (dMin <= 5) {
      digitalWrite(ledVermell, HIGH);
    } else if (dMin <= 10) {
      digitalWrite(ledGroc, HIGH);
    } else {
      digitalWrite(ledVerd, HIGH);
    }

    // --- LÒGICA DE VELOCITAT Y MOVIMENT ---
    int v = (dMin < 40) ? 125 : 255;

    if (dDavant < 15) {
      // 1. Va enrere per separar-se bé
      mover(0, 1, 0, 1, v);
      delay(800);
     
      // 2. Petita pausa mecànica de seguretat
      mover(0, 0, 0, 0, 0);
      delay(100);

      // 3. Gir pivotant més curt per obrir un bon angle (Abans era 1000)
      mover(0, 1, 1, 0, 255);
      delay(450); // <--- AJUSTAT AQUÍ (Gira menys de la meitat de temps per evitar fer els 360º)
     
      // 4. Pausa abans de reactivar els sensors
      mover(0, 0, 0, 0, 0);
      delay(100);

    } else if (dCul < 10) {
      mover(1, 0, 1, 0, v); delay(400); // Escapa endavant
    } else {
      mover(1, 0, 1, 0, v); // Endavant normal
    }

  }
}

void mover(int p1, int p2, int p3, int p4, int vel) {
  analogWrite(pinENA, vel);
  analogWrite(pinENB, vel);
  digitalWrite(Motorpin1, p1); digitalWrite(Motorpin2, p2);
  digitalWrite(Motorpin3, p3); digitalWrite(Motorpin4, p4);
}

void pararRobot() {
  mover(0, 0, 0, 0, 0);
  digitalWrite(ledVermell, LOW); digitalWrite(ledGroc, LOW); digitalWrite(ledVerd, LOW);
  noTone(pinBuzzer);
}

void ejecutarBaile() {
  int notas[] = {261, 261, 261, 349, 440, 0, 261, 261, 261, 349, 440};
  int duracion[] = {150, 150, 150, 300, 300, 100, 150, 150, 150, 300, 300};

  for(int i=0; i<11; i++) {
    if(notas[i] != 0) {
      tone(pinBuzzer, notas[i]);
      if(i % 2 == 0) mover(1, 0, 0, 1, 255);
      else mover(0, 1, 1, 0, 255);
    } else {
      noTone(pinBuzzer);
      mover(0, 0, 0, 0, 0);
    }
    delay(duracion[i]);
    noTone(pinBuzzer);
    delay(50);
  }
  mover(0, 0, 0, 0, 0);
}

long llegirDistancia(int trig, int echo) {
  digitalWrite(trig, LOW); delayMicroseconds(2);
  digitalWrite(trig, HIGH); delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long duracio = pulseIn(echo, HIGH, 25000);
  if (duracio == 0) return 999;
  return duracio / 58.2;
}
