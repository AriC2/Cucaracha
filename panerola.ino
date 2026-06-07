// --- PINS MOTORS ---

// Pin motor dret 1
const int Motorpin1 = 3; 
// Pin motor dret 2
const int Motorpin2 = 5;
// Pin motor esquerre 1
const int Motorpin3 = 6; 
// Pin motor esquerre 2
const int Motorpin4 = 9;
// Pin velocitat dret
const int pinENA = 10; 
// Pin velocitat esquerre
const int pinENB = 11;

// Pin del ventilador
const int pinVentilador = A0;
// Pin del brunzidor
const int pinBuzzer = A4;
// Botó del robot
const int pinPolsador = 2; 
// Botó del ventilador
const int pinPolsadorVent = A5; 

// --- SENSORS ---

// Trigger davanter
const int P_trig_D = 12; 
// Echo davanter
const int P_echo_D = 7;
// Trigger del cul
const int P_trig_C = 4; 
// Echo del cul
const int P_echo_C = 8;

// --- LEDS ---

// Pin LED vermell
const int ledVermell = A1; 
// Pin LED groc
const int ledGroc = A2; 
// Pin LED verd
const int ledVerd = A3;

// Estat marxa robot
bool robotActiu = false;
// Estat marxa ventilador
bool ventiladorActiu = false; 

// Historial botó robot
int ultimEstatPolsador = HIGH;
// Historial botó ventilador
int ultimEstatPolsadorVent = HIGH; 

void setup() {
  // Sortida motor 1
  pinMode(Motorpin1, OUTPUT); 
  // Sortida motor 2
  pinMode(Motorpin2, OUTPUT);
  // Sortida motor 3
  pinMode(Motorpin3, OUTPUT); 
  // Sortida motor 4
  pinMode(Motorpin4, OUTPUT);
  // Sortida velocitat A
  pinMode(pinENA, OUTPUT); 
  // Sortida velocitat B
  pinMode(pinENB, OUTPUT);
  // Sortida ventilador
  pinMode(pinVentilador, OUTPUT); 
  // Sortida brunzidor
  pinMode(pinBuzzer, OUTPUT);
  // Sortida LED vermell
  pinMode(ledVermell, OUTPUT); 
  // Sortida LED groc
  pinMode(ledGroc, OUTPUT); 
  // Sortida LED verd
  pinMode(ledVerd, OUTPUT);
  // Sortida Trigger davanter
  pinMode(P_trig_D, OUTPUT); 
  // Sortida Trigger posterior
  pinMode(P_trig_C, OUTPUT);
  // Entrada Echo davanter
  pinMode(P_echo_D, INPUT); 
  // Entrada Echo posterior
  pinMode(P_echo_C, INPUT);

  // Pull-up botó robot
  pinMode(pinPolsador, INPUT_PULLUP);
  // Pull-up botó ventilador
  pinMode(pinPolsadorVent, INPUT_PULLUP); 
}

void loop() {
  // --- CONTROL DEL BOTÓN DEL ROBOT ---
  
  // Llegeix botó robot
  int estatPolsador = digitalRead(pinPolsador);
  
  // Comprova polsació robot
  if (estatPolsador == LOW && ultimEstatPolsador == HIGH) {
    // Filtre anti-rebot
    delay(50); 
    // Inverteix estat robot
    robotActiu = !robotActiu;
    
    // Si està actiu balla
    if (robotActiu) {
      ejecutarBaile();
    }
    // Si està apagat para
    else {
      pararRobot();
    }
    
    // Espera de seguretat
    delay(300);
  }
  // Guarda últim estat
  ultimEstatPolsador = estatPolsador;

  // --- CONTROL DEL BOTÓN DEL VENTILADOR ---
  
  // Llegeix botó ventilador
  int estatPolsadorVent = digitalRead(pinPolsadorVent);
  
  // Comprova polsació ventilador
  if (estatPolsadorVent == LOW && ultimEstatPolsadorVent == HIGH) {
    // Filtre anti-rebot
    delay(50); 
    // Inverteix estat ventilador
    ventiladorActiu = !ventiladorActiu; 
    // Espera de seguretat
    delay(300);
  }
  // Guarda últim estat
  ultimEstatPolsadorVent = estatPolsadorVent;

  // Control físic ventilador
  if (ventiladorActiu) {
    // Engega el ventilador
    digitalWrite(pinVentilador, HIGH);
  } else {
    // Apaga el ventilador
    digitalWrite(pinVentilador, LOW);
  }

  // --- LÓGICA DE MOVIMIENTO AND SENSORES ---
  
  // Si el robot camina
  if (robotActiu) {

    // Distància de davant
    long dDavant = llegirDistancia(P_trig_D, P_echo_D);
    // Distància del cul
    long dCul = llegirDistancia(P_trig_C, P_echo_C);

    // Troba la més petita
    long dMin = min(dDavant, dCul);

    // --- LÒGICA DE LEDS ---
    
    // Reinicia LED vermell
    digitalWrite(ledVermell, LOW);
    // Reinicia LED groc
    digitalWrite(ledGroc, LOW);
    // Reinicia LED verd
    digitalWrite(ledVerd, LOW);

    // Obstacle a prop
    if (dMin <= 5) {
      // Encén LED vermell
      digitalWrite(ledVermell, HIGH);
    } 
    // Obstacle a mitja distància
    else if (dMin <= 10) {
      // Encén LED groc
      digitalWrite(ledGroc, HIGH);
    } 
    // Camí lliure
    else {
      // Encén LED verd
      digitalWrite(ledVerd, HIGH);
    }

    // --- LÒGICA DE VELOCITAT Y MOVIMENT ---
    
    // Selecciona velocitat v
    int v = (dMin < 40) ? 125 : 255;

    // Pared davant a prop
    if (dDavant < 15) {
      // Pas 1: Va enrere
      mover(0, 1, 0, 1, v);
      // Durant 800ms
      delay(800);
     
      // Pas 2: Pausa motors
      mover(0, 0, 0, 0, 0);
      // Durant 100ms
      delay(100);

      // Pas 3: Gir de 90 graus
      mover(0, 1, 1, 0, 255);
      // Durant 450ms
      delay(450); 
     
      // Pas 4: Pausa sensors
      mover(0, 0, 0, 0, 0);
      // Durant 100ms
      delay(100);

    } 
    // Pared darrere a prop
    else if (dCul < 10) {
      // Fuig cap endavant
      mover(1, 0, 1, 0, v); 
      // Durant 400ms
      delay(400); 
    } 
    // Sense obstacles
    else {
      // Avança recte normal
      mover(1, 0, 1, 0, v); 
    }

  }
}

void mover(int p1, int p2, int p3, int p4, int vel) {
  // Potència motor dret
  analogWrite(pinENA, vel);
  // Potència motor esquerre
  analogWrite(pinENB, vel);
  // Direcció motor dret 1
  digitalWrite(Motorpin1, p1); 
  // Direcció motor dret 2
  digitalWrite(Motorpin2, p2);
  // Direcció motor esquerre 1
  digitalWrite(Motorpin3, p3); 
  // Direcció motor esquerre 2
  digitalWrite(Motorpin4, p4);
}

void pararRobot() {
  // Para tots els motors
  mover(0, 0, 0, 0, 0);
  // Apaga LED vermell
  digitalWrite(ledVermell, LOW); 
  // Apaga LED groc
  digitalWrite(ledGroc, LOW); 
  // Apaga LED verd
  digitalWrite(ledVerd, LOW);
  // Apaga so brunzidor
  noTone(pinBuzzer);
}

void ejecutarBaile() {
  // Freqüències de la música
  int notas[] = {261, 261, 261, 349, 440, 0, 261, 261, 261, 349, 440};
  // Durada de les notes
  int duracion[] = {150, 150, 150, 300, 300, 100, 150, 150, 150, 300, 300};

  // Bucle per les 11 notes
  for(int i = 0; i < 11; i++) {
    // Si no és silenci
    if(notas[i] != 0) {
      // Reprodueix nota al buzzer
      tone(pinBuzzer, notas[i]);
      // Moviment A si és parell
      if(i % 2 == 0) {
        mover(1, 0, 0, 1, 255);
      }
      // Moviment B si és senar
      else {
        mover(0, 1, 1, 0, 255);
      }
    } 
    // Si és un silenci
    else {
      // Apaga so buzzer
      noTone(pinBuzzer);
      // Para motors en silenci
      mover(0, 0, 0, 0, 0);
    }
    // Manté nota i ball
    delay(duracion[i]);
    // Talla so de la nota
    noTone(pinBuzzer);
    // Separació de notes
    delay(50);
  }
  // Atura el robot al final
  mover(0, 0, 0, 0, 0);
}

long llegirDistancia(int trig, int echo) {
  // Trigger en LOW neteja
  digitalWrite(trig, LOW); 
  // Espera microsegons
  delayMicroseconds(2);
  // Dispara ultrasò HIGH
  digitalWrite(trig, HIGH); 
  // Manté disparador 10us
  delayMicroseconds(10);
  // Apaga disparador LOW
  digitalWrite(trig, LOW);
  // Mesura temps del rebot
  long duracio = pulseIn(echo, HIGH, 25000);
  
  // Si dóna error retorna 999
  if (duracio == 0) {
    return 999;
  }
  
  // Calcula centímetres finals
  return duracio / 58.2;
}
