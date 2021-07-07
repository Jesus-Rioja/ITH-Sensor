/**
  @file funcionesITH.h
  @brief Funciones que engloban las funciones creadas en los demas archivos.
  @author Jesus Rioja
  @date 29/06/2021
*/
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																										PLACA DE DESARROLLO LORA WIFI ESP32 V2                                                         //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																								Â¿ QUE QUEDA POR HACER ?                                                                            //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																										CONJUNTO DE LIBRERIAS                                                                          //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
#include <Arduino.h>
#include <oled32.h> 
#include <sensorDHT2232.h>
#include <timers32.h>
#include <rele32.h>
#include <chip32.h>
#include <EEPROM32.h>
#include <ledSwitch32.h>
#include <bateria32.h>
#include <TTN_esp32.h>
#include "TTN_CayenneLPP.h"

const char* devEui = "A8610A3130467613"; // Change to TTN Device EUI
const char* appEui = "70B3D57ED002B1DB"; // Change to TTN Application EUI
const char* appKey = "CB8F23E4246C68442A04C20B224F9497"; // Chaneg to TTN Application Key

TTN_esp32 ttn ;
TTN_CayenneLPP lpp;

//LIBRERIA PARA DESCOMPONER EL PAQUETE

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																									DEFINE Y VARIABLES GLOBALES                                                                             //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

//MODO INFORMACION POR PUERTO SERIE. SI QUIERES MODO DEPURACION DBUG = 1
#define DBUG 1

int tipoDeFuncion;

//VARIABLE PARA INTERPRETAR SI HAY DATO O NO EN UNA FUNCIÓN
int dato;

//VARIABLE BOOLEANA QUE INTERPRETA SI ES AUTOMATICO O MANUAL PARA ACTIVAR LOS RELES --- automaático 1 y manual 0
bool T_auto_manual = 1;
String R_auto_manual = "";

//VARIABLE PARA IDENTIFICAR SI SE HA RECIBIDO ALGO POR UN NODO
bool recibidoNodo = 0; 					//1 indica que se ha recibido de un nodo

//VARIABLE QUE SE UTILIZARÁ PARA VER SI EL PAQUETE TIENE O NO INTEGRIDAD
bool comprobar = 0;

//VARIABLE QUE IDENTIFICA SI HA HABIDO O NO HEALTH BIT
bool healthBit = 0;

//VARIABLE QUE IDENTIFICA SI SE HA RECIBIDO O NO CONTESTACION AL HEALTBIT
bool esperaPaquete = 0;

//VARIABLE AUXILIAR PARA CONFIGURAR VAQUERIZA Y ZONA
String R_auxchipid = "";

//VARIABLE AUXILIAR PARA VERIFICAR NODO CON BATERÍA PUEDE DORMIR
bool tareaCumplida = 0;
bool empezarTarea = 1;

//VARIABLE AUXILIAR PARA REPETIR PAQUETE EN CASO DE ERROR
String R_tryAgain = "";
//VARIALBE AUXILIAR PARA RECOGER ID VAQUERIZA E ID ZONA DE LA CONFIGURACION
String auxConfiguracionVaquerizaZona = "";
//VARIALBE AUXILIAR PARA VERIFICAR EL ESTADO DEL DISPOSTIVO (AUTO O MANUAL)
String auxConfiguracionAutoManual = "";

//VARIABLES PARA GARANTIZAR EL ENVIO DEL PAQUETE DE CONFIGURACION DE LOS RELES
String auto_manualV = "";
String R_StatusR0V = "";
String R_StatusR1V = "";
String R_StatusR2V = "";
String R_StatusR3V = "";

String ssidB;
String passwordB;
char buf1 [25];
unsigned int leng1 = 50;
char buf2 [25];
unsigned int leng2 = 50;

int ID_Vaqueriza;
int ID_Zona;

byte msg[19];


/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																										PROTOTIPADO DE FUNCIONES                                                                                         //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

//FUNCIONES GLOBALES
void inicializar(void);
void comprobarITH (int);
void siHayDatoDHT();
void queFuncionHago();
void InterrupcionTimer0 ();
void InterrupcionTimer1 ();
void InterrupcionTimer2 ();
void InterrupcionTimer3 ();
void compruebaInterrupcion();


void queFuncionHago()
{
	if (bateria == 0){
		tipoDeFuncion = 2; 	//NODO SIN BATERÍA
	}
	else{
		tipoDeFuncion = 1;  //NODO CON BATERÍA
	}

}

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																										INICIALIZAR EL DISPOSITIVO                                                               //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
/**
Funcion principal de inicializacion 
@note Engloba las inicializaciones de las otras funciones, LoRa, DHT, CHIP, OLED...
Distingue si el dispositivo tiene bateria o no, en caso de no tener baterÃ­a, configurarÃ¡ los reles, timers
y si es concentrador, EEPROM, Consulta si tiene los datos ssid y password y si no crearÃ¡ el punto de acceso
*/
void inicializar(){
  	Serial.begin(115200);
  	//iniciarLoRa();
	ttn.begin();
  	ttn.join(devEui, appEui, appKey);
  	Serial.print("Joining TTN ");
    while (!ttn.isJoined())
    {
        Serial.print(".");
        delay(500);
    }
    Serial.println("\njoined !");
    //ttn.showStatus();
  	dhtSensor.begin();
	chipID();
	analizaSwitch();
	configuraLEDs();
	hayBateria();
	Serial.println(bateria);
	if (bateria == 0){
		T_levelBateria = 99.99;
		des_activaLEDs(true, false);
		configurarRele();
		configurarTimer(false,false,false,true);
		arrancarTimer(false,false,false,true);		
		des_activaLEDs(false, true);
	}
	else{
		configuracionBateria();
	}
	iniciarOLED();
	errorOLED("La configuracion está OK");	
	queFuncionHago();
	if(DBUG == 1){
		Serial.println("Velocidad puerto serie 115200 bps, SPI inicializado, pantalla preparada, DHT preparado, Switch OK, Relés OK, timer OK");
		Serial.println("Termine configuración");
	}		
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																										COMPROBAR ITH PARA ACTIVAR RELE                                                               //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
/**
Funcion que comprueba que el ITH no sea superior al umbral establecido
@param ITH el ITH calculado se comprueba con el indice establecido
@note En caso de que ITH haya sido un valor superior al indice establecido, y nos encontremos en modo automatico
saltarán los reles y configuramos y arrancamos los TIMERS que gestionan los reles (No el controla el flujo de programa) 
*/
void comprobarITH (int ITH)
{							
	if(ITH >= indiceITH)
	{
		if(T_auto_manual == 1 && bateria == 0)
		{   // si no tiene baterí­a el dispositivo y esta en modo automático
			des_activaReles(HIGH);								// activa los relés (todos)
			configurarTimer(true,true,true,false);// configura los timers que se van a usar (0,1,2)
			arrancarTimer(true,true,true,false);  // y arranca los timers que se han configurado
		}
		else
		{
			return;
		}
	}
	/*else
	{
		if(T_auto_manual == 1 && bateria == 0)
		{   // si no tiene baterí­a el dispositivo y esta en modo automático
			des_activaReles(HIGH);								// activa los relés (todos)
			configurarTimer(false,false,false,true);// configura los timers que se van a usar (0,1,2)
			arrancarTimer(false,false,false,true);  // y arranca los timers que se han configurado
		}
		else
		{
			return;
		}
	}*/
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																										CALCULAR ITH                                                                //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
/**
Funcion que en caso de que se haya leido el sensor DHT22, si es el concentrador subirá los datos a la BBDD
@note Lineas comentadas, anteriormente se enviaban los datos después de leerlos, ahora se envian después del HEALTH BIT, no después de leerlos*/
void siHayDatoDHT(){
	f2b t;
	f2b h;
	f2b bat;

	if (flagDHT22 == 1){
		T_ITH = calcularITH(T_temp, T_hum);
		comprobarITH(T_ITH);
		errorOLED("ITH CALCULADO");

		Serial.println("Temperatura: " + String(T_temp));

		t.fbyte = T_temp;
		h.fbyte = T_hum;
		bat.fbyte = T_levelBateria;

		Serial.println("Humedad: " + String(T_hum));

		msg[0] = (unsigned int)((T_ITH >> 24) & 0xFF) ;
  		msg[1] = (unsigned int)((T_ITH >> 16) & 0xFF) ;
  		msg[2] = (unsigned int)((T_ITH >> 8) & 0XFF);
  		msg[3] = (unsigned int)((T_ITH & 0XFF));
		msg[4] = t.bbyte[0];
		msg[5] = t.bbyte[1];
		msg[6] = t.bbyte[2];
		msg[7] = t.bbyte[3];
		msg[8] = h.bbyte[0];
		msg[9] = h.bbyte[1];
		msg[10] = h.bbyte[2];
		msg[11] = h.bbyte[3];
		msg[12] = bat.bbyte[0];
		msg[13] = bat.bbyte[1];
		msg[14] = bat.bbyte[2];
		msg[15] = bat.bbyte[3];
		msg[16] = (unsigned int)(ID_Vaqueriza & 0xFF) ;
		msg[17] = (unsigned int)(ID_Zona & 0xFF) ;


		des_activaLEDs(false, false);

		Serial.println("Enviando datos... ");
		ttn.sendBytes(msg, sizeof(msg));
		//ttn.sendBytes(t.bbyte, sizeof(t.bbyte));
		
		flagDHT22 = 0;
	}
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																										GESTION DE LOS TIMER POR BANDERA                                                               //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
/**
Funcion que se ejecutará tras activarse la bandera de interrupcion timer3
@note Cada 20 minutos se ejecutará esta interrupcion que consiste en
leer el sensor DHT22 y en caso de ser el concentrador consultar la IP por si ha cambiado.
*/
void InterrupcionTimer3 (){
	if (contadorInterrupcionConcentrador == 1){
    portENTER_CRITICAL(&timer3Mux);
    contadorInterrupcionConcentrador = 0;
    portEXIT_CRITICAL(&timer3Mux);
		leerDHT22();
		if (concentrador == 0){
			healthBit = 0;
		}
    if (DBUG == 1){
		Serial.println(contadorInterrupcionConcentrador);
		contadorTotalInterrupcionConcentrador++;
    Serial.println("se han producido un total de: " + String(contadorTotalInterrupcionConcentrador) + " interrupciones por timer 3 ");   
			}  
    }
}
/**
Funcion que se ejecutará tras activarse la bandera de interrupcion timer2
@note En caso de que ITH haya sido un valor superior al indice establecido, habrá saltado en rele
y el timer de 5 minutos que lo controla se habrá inicializado. Tras el paso de este, salta la interrupcion y lo detiene
*/
void InterrupcionTimer2 (){
	if (contadorInterrupcion5 == 1){
    portENTER_CRITICAL(&timer2Mux);
    contadorInterrupcion5 = 0;
    portEXIT_CRITICAL(&timer2Mux);
		des_activaRele0(false);
    if (DBUG == 1){
		contadorTotalInterrupcion5++;
    Serial.println(contadorInterrupcion5);
		Serial.println("se han producido un total de: " + String(contadorTotalInterrupcion5) + " interrupciones por timer 0");   
			}
		}
}
/**
Funcion que se ejecutará tras activarse la bandera de interrupcion timer1
@note En caso de que ITH haya sido un valor superior al indice establecido, habrá saltado en rele
y el timer de 15 minutos que lo controla se habrá inicializado. Tras el paso de este, salta la interrupcion y lo detiene
*/
void InterrupcionTimer1 (){
	if (contadorInterrupcion15 == 1){
    portENTER_CRITICAL(&timer1Mux);
    contadorInterrupcion15 = 0;
    portEXIT_CRITICAL(&timer1Mux);
		des_activaRele1(false);
    if (DBUG == 1){
		Serial.println(contadorInterrupcion15);
		contadorTotalInterrupcion15++;
    Serial.println("se han producido un total de: " + String(contadorTotalInterrupcion15) + " interrupciones por timer 1");   
			}
		}
}
/**
Funcion que se ejecutará tras activarse la bandera de interrupcion timer0
@note En caso de que ITH haya sido un valor superior al indice establecido, habrá saltado en rele
y el timer de 20 minutos que lo controla se habrá inicializado. Tras el paso de este, salta la interrupcion y lo detiene
*/
void InterrupcionTimer0 (){
	if (contadorInterrupcion20 == 1){
    portENTER_CRITICAL(&timer0Mux);
    contadorInterrupcion20 = 0;
    portEXIT_CRITICAL(&timer0Mux);
		des_activaRele2(false);
		des_activaRele3(false);
		if (DBUG == 1){
		Serial.println(contadorInterrupcion20);
		contadorTotalInterrupcion20++;
    Serial.println("se han producido un total de: " + String(contadorTotalInterrupcion5) + " interrupciones por timer 2");   
			}
		}
}
/**
Funcion que permite la lectura de todas las interrupciones por si se han producido
*/
void compruebaInterrupcion(){
	InterrupcionTimer3();
	InterrupcionTimer2();
	InterrupcionTimer1();
	InterrupcionTimer0();
}