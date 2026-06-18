// ======================================================
// CONFIGURACIÓ DE PINS I CONSTANTS
// ======================================================

// --- POLSADOR ---
const int pinPolsador = 6;     // Pin on connectarem el polsador
bool robotActiu = false;       // Estat del robot (encès/apagat)
int ultimEstatPolsador = HIGH; // Per detectar quan es prem

// --- SENSOR ULTRASONS DAVANT ---
#define P_echo_D 11    
#define P_trig_D 12    

// --- SENSOR ULTRASONS CUL (DARRERE) ---
#define P_echo_C 9    
#define P_trig_C 10    

// --- PINS MOTORS ---
const int Motorpin1 = 2;
const int Motorpin2 = 3;
const int Motorpin3 = 4;
const int Motorpin4 = 5;

// --- PINS LEDS ---
int ledVermell = 13;    
int ledGroc = 8;        
int ledVerd = 7;        

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

  pinMode(ledVermell, OUTPUT);
  pinMode(ledGroc, OUTPUT);
  pinMode(ledVerd, OUTPUT);

  // Configurem el polsador amb la resistència interna d'Arduino
  pinMode(pinPolsador, INPUT_PULLUP);

  // El robot comença aturat (llums apagades i motors parats)
  pararRobot();
}

void loop() {
  // --- LÒGICA DEL POLSADOR (ON/OFF) ---
  int estatPolsador = digitalRead(pinPolsador);

  // Detectem si s'ha premut (pas de HIGH a LOW)
  if (estatPolsador == LOW && ultimEstatPolsador == HIGH) {
    robotActiu = !robotActiu; // Canviem l'estat (de false a true o viceversa)
    delay(200); // Anti-rebots per evitar falses pulsacions
   
    if (robotActiu) {
      ejecutarBaile(); // Fa el ball només en arrencar
    } else {
      pararRobot();    // S'atura immediatament al apagar
    }
  }
  ultimEstatPolsador = estatPolsador;

  // --- SI EL ROBOT ESTÀ ACTIU, FUNCIONA NORMALMENT ---
  if (robotActiu) {
    long distDavant = llegirDistancia(P_trig_D, P_echo_D);
    long distCul = llegirDistancia(P_trig_C, P_echo_C);

    digitalWrite(ledVermell, LOW);
    digitalWrite(ledGroc, LOW);
    digitalWrite(ledVerd, LOW);

    if ((distDavant > 0 && distDavant <= 35) || (distCul > 0 && distCul <= 10)) {
      if (distDavant <= 5 && distDavant > 0) {
        digitalWrite(ledVermell, HIGH);
      } else {
        digitalWrite(ledGroc, HIGH);
      }
    } else {
      digitalWrite(ledVerd, HIGH);
    }

    if(distCul > 0 && distCul <= 10) {
      mover(1, 0, 1, 0); delay(500);
      mover(1, 0, 0, 1); delay(400);
    }
    else if(distDavant > 0 && distDavant <= 10) {
      mover(0, 1, 0, 1); delay(600);
      mover(1, 0, 0, 1); delay(500);
    }
    else if(distDavant > 10 && distDavant <= 35) {
      mover(1, 0, 0, 1);
    }
    else {
      avanzar();
    }
  }
  else {
    // Si està apagat, ens assegurem que els motors no es moguin
    pararRobot();
  }
 
  delay(50);
}

// --- NOVES FUNCIONS I MODIFICACIONS ---

void pararRobot() {
  mover(0, 0, 0, 0);
  digitalWrite(ledVermell, LOW);
  digitalWrite(ledGroc, LOW);
  digitalWrite(ledVerd, LOW);
}

long llegirDistancia(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long duracion = pulseIn(echo, HIGH, 25000);
  if (duracion == 0) return 999;
  return duracion / 58.2;
}

void mover(int p1, int p2, int p3, int p4) {
  digitalWrite(Motorpin1, p1);
  digitalWrite(Motorpin2, p2);
  digitalWrite(Motorpin3, p3);
  digitalWrite(Motorpin4, p4);
}

void avanzar() {
  mover(1, 0, 1, 0);
}

void ejecutarBaile() {
  mover(1,0,1,0); delay(400);
  mover(0,1,0,1); delay(400);
  mover(0,0,0,0); delay(500);
}
