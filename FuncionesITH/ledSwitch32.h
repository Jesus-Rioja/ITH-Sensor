/**
  @file ledSwitch32.h
  @brief funciones principales para gestionar los 2 reles y el switch que identifica si es nodo o concentrador
	El led Rojo corresponde al pin 32
	El led Verde corresponde al pin 33
  El microswitch corresponde al pin 25
	@author Antonio Cánovas
  @date 11/02/2019

*/


/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																										SWITCH  Y LEDs                                                                                 //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																									DEFINE Y VARIABLES GLOBALES                                                                             //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
#define DBUG 1
// DEFINE LAS VARIABLES CORRESPONDIENTES AL SWITCH Y LOS LEDs
#define PINSWITCH 25
#define PINLEDROJO 32
#define PINLEDVERDE 33
bool concentrador = 1;
bool statusSwitch = 0;
bool statusLedRojo = 0;
bool statusLedVerde = 0;
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																									PROTOTIPADO DE FUNCIONES                                                                       //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
void analizaSwitch(void);
void configuraLEDs(void);
void des_activaLEDs(bool,bool);

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																									FUNCIONES													                                                                 //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

/**
Esta funcion configura como entrada el pin del switch, lee el estado en el que se encuentra la patilla,
si esta está a uno, corresponderá al valor concenctrador = 1 para el dispositivo.
@note Esta funcion se encargará de indentificar si el dispotivo es un concentrador o un nodo*/
void analizaSwitch (){
	pinMode(PINSWITCH,INPUT);
	delay(50);
	statusSwitch = digitalRead(PINSWITCH);
	if (statusSwitch == 1){
		concentrador = 1;
	}
	else{
		concentrador = 0;
	}
	if (DBUG == 1){
		Serial.println("Switch configurado, estamos en modo concentrador: " + String(concentrador));
	}
}
/**
Esta funcion configura los leds como salidas, para poder modificar su estado
*/
void configuraLEDs(){
	pinMode(PINLEDROJO, OUTPUT);
	pinMode(PINLEDVERDE, OUTPUT);
	if (DBUG == 1){
		Serial.println("LEDs configurados");
	}
}
/**
Esta funcion activa, desactiva los leds, haciendo un apagado o encendido de cada uno de ellos
@param rojo variable bool que nos indicará si apagar o encender el led rojo
@param verde variable bool que nos indicará si apagar o encender el led verde
@note Esta funcion admite un nivel alto o bajo de entrada para activar o desactivar
cada led*/
void des_activaLEDs (bool rojo, bool verde){
	if (rojo == 1){
		digitalWrite(PINLEDROJO, HIGH);
		statusLedRojo = 1;
	}
	if (rojo == 0){
		digitalWrite(PINLEDROJO, LOW);
		statusLedRojo = 0;
	}
	if (verde == 1){
		digitalWrite(PINLEDVERDE, HIGH);
		statusLedVerde = 1;
	}
	if (verde == 0){
		digitalWrite(PINLEDVERDE, LOW);
		statusLedVerde = 0;
	}
	if (DBUG == 1){
		Serial.println("El led rojo vale: " + String(statusLedRojo) + "\nEl led verde vale: " + String(statusLedVerde));
	}
}