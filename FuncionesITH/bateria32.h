/**
  @file bateria32.h
  @brief Aqui encontraremos la configuracion básica que necesitamos para poder controlar el nivel de batería de nuestros dispositivos.
	Destacar dos cosas:
		- Necesitamos poner Vext (pin 21) a nivel bajo para poder activar leer el nivel de la batería,
			informacion del fabricante (Vext connected to 3.3V via a MOS-FET, the gate pin connected to GPIO21)
		- El nivel de la batería es detectado a traves del pin 13.
	@author Antonio Cánovas
  @date 21/02/2019

*/


/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																										BATERIA                                                                                       //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																									DEFINE Y VARIABLES GLOBALES                                                                             //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
#define Vext 21
#define Fbattery    4200  //The default battery is 3700mv when the battery is fully charged.
#define Vbateria 13
#define identificaBateria 39

float XS = 0.00225;      //The returned reading is multiplied by this XS to get the battery voltage.
uint16_t MUL = 1000;
uint16_t MMUL = 100;
//VARIABLE QUE SE UTILIZARÁ PARA SABER SI ES UN NODO O UN CONCENTRADOR
bool bateria = 0;
//VARIABLES QUE INDICARÁN NIVEL DE BATERÍA A TRANSMITIR Y RECIBIDO
float T_levelBateria;
String R_levelBateria = "";
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																									PROTOTIPADO DE FUNCIONES                                                                       //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
void configuracionBateria(void);
void leerBateria(void);
void hayBateria(void);
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																									FUNCIONES													                                                                 //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
/**
Esta funcion configura el pin 39 para identificar si el dispositivo funciona o no con bateria.
*/
void hayBateria(void){
	pinMode(identificaBateria, INPUT);
	bool readBateria = digitalRead(identificaBateria);
	//readBateria = 1;/* PARA NODO CON BATERÍA*/
	if (readBateria == 0){
		bateria = 0;
		if (DBUG == 1){
			Serial.println("Estamos a nivel bajo, NO HAY BATERIA");
		}
	}
	else {
		bateria = 1;
		if (DBUG == 1){
			Serial.println("Estamos a nivel alto, HAY BATERIA");
		}
	}
}
/**
Esta funcion configura los pines 21 y 13 para poder acceder al contenido de la tensión en la batería,
*/
void configuracionBateria (void){
	pinMode(Vext, OUTPUT);
	digitalWrite(Vext, LOW);
	adcAttachPin(Vbateria);
	analogSetClockDiv(255);
}
/**
Esta funcion lee la tensión de la bateria,
@note Leemos el pin 13 y ajustamos la lectura a voltios*/
void leerBateria(void){
	uint16_t c  =  analogRead(13) * XS * MUL ; // mV
	T_levelBateria = (float)c/1000;
//  uint16_t d  =  (analogRead(13) * XS * MUL * MMUL) / Fbattery; // porcentaje de la bateria si fuera necesario
}