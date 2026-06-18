// --- PINS MOTORS (Canviats a pins PWM per controlar velocitat) ---
const int Motorpin1 = 3;  // PWM
const int Motorpin2 = 5;  // PWM
const int Motorpin3 = 6;  // PWM
const int Motorpin4 = 9;  // PWM

int velocitatNormal = 255; // Velocitat màxima
int velocitatLenta = 128;  // Aproximadament la meitat

// ... (la resta de pins es manté igual)

void loop() {
  // ... (lògica del polsador igual)

  if (robotActiu) {
    long distDavant = llegirDistancia(P_trig_D, P_echo_D);
    long distCul = llegirDistancia(P_trig_C, P_echo_C);
    
    int v = velocitatNormal;

    // Si detecta objecte a prop (entre 10 i 35 cm), reduïm velocitat
    if (distDavant <= 35 && distDavant > 0) {
      v = velocitatLenta;
    }

    // Lògica de moviment actualitzada amb velocitat 'v'
    if(distCul > 0 && distCul <= 10) {
      mover(v, 0, v, 0); delay(500); // Fuig cap endavant
    }
    else if(distDavant > 0 && distDavant <= 10) {
      mover(0, v, 0, v); delay(600); // Enrere
      mover(v, 0, 0, v); delay(500); // Gir
    }
    else if(distDavant > 10 && distDavant <= 35) {
      mover(v, 0, 0, v); // Gira lentament
    }
    else {
      avanzar(velocitatNormal); // Camp lliure, velocitat total
    }
  } else {
    pararRobot();
  }
  delay(50);
}

// --- FUNCIONS MODIFICADES ---

void mover(int p1, int p2, int p3, int p4) {
  // Ara usem analogWrite per al control de velocitat
  analogWrite(Motorpin1, p1);
  analogWrite(Motorpin2, p2);
  analogWrite(Motorpin3, p3);
  analogWrite(Motorpin4, p4);
}

void avanzar(int v) {
  mover(v, 0, v, 0);
}

void pararRobot() {
  mover(0, 0, 0, 0);
  // ... (resta de llums igual)
}
