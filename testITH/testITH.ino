#include <FuncionesITH.h>


#define DORMIR 2


unsigned long timeout;
bool actualiza = 1;

void setup() {
  // put your setup code here, to run once:

  ID_Vaqueriza = 1;
  ID_Zona = 3;
  
  inicializar();
}

void loop() {
 // put your main code here, to run repeatedly:
  switch (tipoDeFuncion) {
    case 1: // NODO CON BATERÍA
      //Serial.println("soy Nodo con batería");
      if (empezarTarea) {     //Cuando despierta se reinicia y toma los valores con los que se ha inicializado el dispositivo empezarTarea = 1;
        leerDHT22();
        siHayDatoDHT();
        empezarTarea = 0;
      }
      //NOVEDAD
      if (actualiza == 1) {
        timeout = millis();
        actualiza = 0;
      }
      if (millis() - timeout > 7000) {
        //Enviar TTN?
        actualiza = 1;
      }
      //NOVEDAD^
      if (tareaCumplida) {
        pinMode(DORMIR, OUTPUT);
        while (1) {
          Serial.println("Estoy DENTRO");
          digitalWrite(DORMIR, HIGH);
          delay(1);
          digitalWrite(DORMIR, LOW);
          delay(1);
        }
        //Adormir();
      }
      break;
    case 2: // NODO SIN BATERÍA
      //Serial.println("soy Nodo sin batería");
      compruebaInterrupcion();
      siHayDatoDHT();
      
      /*      //NOVEDAD
            if (actualiza == 1) {
              timeout = millis();
              actualiza = 0;
            }
            if (millis() - timeout > 7000) {
              enviaHealthBit();
              healthBit = 0;
              actualiza = 1;
            }
            //NOVEDAD^ */
      break;
    default:
      ESP.restart();
      break;
  }
}
