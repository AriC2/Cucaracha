# PROJECTE PANEROLA - ROBOT ASPIRADORA

Aquest repositori conté el codi per al funcionament d'un robot aspirador / esquiva-obstacles en Arduino.

---

## COMPONENTS UTILITZATS

* **Placa de control:** Keyestudio UNO R3 (compatible amb Arduino Uno).
* **Motors de les rodes:** Dos motorreductors de corrent continu. 
* **Controlador de motors (Driver):** Mòdul L298N. 
* **Sensors de distància:** Dos sensors d'ultrasons HC-SR04 (un davanter i un posterior).
* **Sistema de ventilació:** Un ventilador de PC (ordinador) de 5V/12V reutilitzat.
* **Placa de proves:** Placa Protoboard per connectar tots els cables.
* **Components que fan llum i so:** Tres LEDs d'estat (vermell, groc i verd) i un brunzidor (buzzer). 
* **Entrades de control:** Dos polsadors.

---

## FUNCIONAMENT DEL CODI

### 1. Inici i control de l'usuari (Polsadors)
* **Botó del robot:** En prémer el polsador del pin 2, el robot s'activa (reprodueix la sintonia de La Cucaracha i fa un ball) o s'atura completament.
* **Botó del ventilador:** En prémer el polsador del pin A5, el ventilador s'encén o s'apaga de manera 100% independent, sense importar si el robot es mou o està quiet (com el mode d'aspiració d'una Roomba real).

### 2. Detecció de l'entorn (Sensors d'ultrasons)
* **Mesura de distàncies:** El robot calcula contínuament l'espai lliure en centímetres que té a la part davantera (dDavant) i a la posterior (dCul).
* **Càlcul de proximitat:** El programa detecta automàticament quina de les dues distàncies és la més xicoteta (dMin) per actuar en conseqüència.

### 3. Indicadors d'estat (Semàfor LED)
* **LED Vermell:** S'encén quan l'obstacle més proper està a 5 cm o menys (alerta de xoc imminent).
* **LED Groc:** S'encén si l'obstacle es troba a mitjana distància, entre 5 i 10 cm (alerta de precaució).
* **LED Verd:** S'encén quan no hi ha cap objecte a prop en un radi de 10 cm (camí completament lliure).

### 4. Regulació de velocitat (Seguretat mecànica)
* **Desacceleració automàtica:** Si el robot detecta qualsevol objecte a menys de 40 cm, redueix la potència dels motors a la meitat (125) per maniobrar amb seguretat. Si no veu res, es mou a màxima velocitat (255).

### 5. Algorisme de navegació (Moviment de les rodes)
* **Evasió davantera (< 15 cm):** Quan troba una paret al davant, el robot fa una maniobra de rescat en quatre fases per obrir-se pas de forma segura:
    1. **Retrocedeix** durant 800 ms per guanyar espai.
    2. Fa una petita **pausa de seguretat** de 100 ms per protegir els engranatges de la inèrcia dels motors.
    3. Realitza un **gira pivotant** cap a un costat durant 450 ms (ajustat per obrir un angle proper als 90 graus i evitar fer un gir complet sobre el mateix obstacle).
    4. Fa una **última pausa** de 100 ms abans de reavaluar la zona amb els sensors.
* **Evasió posterior (< 10 cm):** Si detecta que un obstacle s'acosta pel darrere però té el camí lliure al davant, accelera cap endavant durant 400 ms per escapar del perill.
* **Marxa normal:** Si no es compleix cap de les condicions anteriors i el camí lliure, el robot simplement avança en línia recta de manera contínua per seguir netejant.
