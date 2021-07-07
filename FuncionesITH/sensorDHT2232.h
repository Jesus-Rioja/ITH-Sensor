/**
  @file sensorDHT2232.h
  @brief funciones principales para controlar el sensor de temperatura y humedad.
	Encontramos la funcion que se encarga de recoger la humedad y temperatura aportada por el sensor 
	y seguidamente calculamos el ITH.
  @author Antonio C�novas
  @date 07/02/2019
*/
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																										SENSOR DHT22                                                                                   //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																										CONJUNTO DE LIBRERIAS                                                                          //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
#include <Adafruit_Sensor.h>
#include <DHT.h>     // SENSOR DHT
#include <DHT_U.h>
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																									DEFINE Y VARIABLES GLOBALES                                                                             //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
#define DBUG 1
//PIN Y TIPO DE SENSOR DE HUMEDAD Y TEMPERATURA
#define DHTPIN  17
#define DHTTYPE DHT22
//DEFINE INDICE ITH
#define indiceITH 80
//dhtSensor PARA DIRIGIRNOS HACIA EL SENSOR
//DHT_Unified dhtSensor(DHTPIN, DHTTYPE);
DHT dhtSensor(DHTPIN, DHTTYPE);
// VARIABLES DE TEMPERATURA, HUMEDAD Y ITH CORRESPONDIENTE AL DHT22 PARA EL ENVIO

union f2b {
  float fbyte;
  byte bbyte[3];
};

float T_temp = 0;
float T_hum = 0;
int T_ITH = 0;

//VARIABLE PARA IDENTIFICAR QUE SE HA LEIDO EL SENSOR 
bool flagDHT22 = 0;
int iDHT = 0;
float auxTemperatura = 0;
float auxHumedad = 0;
float sumaT = 0;
float sumaH = 0;
int count = 0;
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																										PROTOTIPADO DE FUNCIONES                                                                       //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
void leerDHT22(void);
int calcularITH (float,float);
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																										FUNCIONES                                                                                      //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
// LEER LA INFORMACI�N APORTADA POR EL SENSOR DE TEMPERATURA Y HUMEDAD DHT22
// NO ACEPTAR NINGUN DATO QUE NO SE HAYA LEIDO BIEN Y HACER LA MEDIA DE 8 VALORES
/**
Esta funcion lee del sensor DHT 22 la humedad y la temperatura
@note Hacemos 5 medidas (0-4) y en el caso de que alguna de ellas no sea correcta, volvemos a realizarla
Actualizamos las variables globales T_temp y T_hum con la media aritm�tica de las medidas anteriores.
*/
void leerDHT22(){
	if (flagDHT22 == 0){  // no se tiene constancia de una temperatura y una humedad
		if (DBUG == 1){
			Serial.println("Entramos a la funcion leerDHT22");
		}
		//sensors_event_t event;
		//dhtSensor.temperature().getEvent(&event);
		T_temp = dhtSensor.readTemperature();	
		//dhtSensor.humidity().getEvent(&event);
		T_hum = dhtSensor.readHumidity();		
		if (isnan(T_temp) or isnan(T_hum)){			
			delay(2000);
			leerDHT22();
		}
		if(DBUG == 1){
			Serial.println("La temperatura vale: " + String(T_temp) + "ºC" + "\nLa humedad vale: " + String(T_hum) + "%");
	  	}
		flagDHT22 = 1;
		if(DBUG == 1){
			Serial.println("La variable flagDHT22 tiene el valor de:  " + String (flagDHT22));
		}	
	}
}

// LEER LA INFORMACI�N APORTADA POR EL SENSOR DE TEMPERATURA Y HUMEDAD DHT22
// NO ACEPTAR NINGUN DATO QUE NO SE HAYA LEIDO BIEN Y HACER LA MEDIA DE 8 VALORES
/**
Esta funcion lee del sensor DHT 22 la humedad y la temperatura
@note Hacemos 5 medidas (0-4) y en el caso de que alguna de ellas no sea correcta, volvemos a realizarla
Actualizamos las variables globales T_temp y T_hum con la media aritm�tica de las medidas anteriores.
*/
/*
void leerDHT22(){
	if (flagDHT22 == 0){  // no se tiene constancia de una temperatura y una humedad
		if (DBUG == 1){
			Serial.println("Entramos a la funcion leerDHT22");
		}
		iDHT = 0;
		for (iDHT; iDHT < 4; iDHT++){
				sensors_event_t event;
				dhtSensor.temperature().getEvent(&event);
				auxTemperatura = event.temperature;	
				dhtSensor.humidity().getEvent(&event);
				auxHumedad = event.relative_humidity;		
				delay(2000);
				if (isnan(auxTemperatura) or isnan(auxTemperatura)){			
					//auxTemperatura = event.temperature;
					//auxHumedad = event.relative_humidity;
					delay(2000);
					//nuevo 10/09/2019
					auxTemperatura = 0;
					auxHumedad = 0;
					leerDHT22();
					//
			}
			sumaT = sumaT + auxTemperatura;
			sumaH = sumaH + auxHumedad;
			if(DBUG == 1){
				Serial.println("Vamos por la iteracion:  " + String (iDHT));
				Serial.println("AuxTemp: " + String(auxTemperatura) + " �C" + "AuxHum: " + String(auxHumedad) + " %");
				Serial.println("lA SUMA_T VALE: " + String(sumaT) + "\nLA SUMA_H VALE: " + String(sumaH));
			}
		}
		T_temp = sumaT / 4;
		T_hum = sumaH / 4;
		if(DBUG == 1){
			Serial.println("La temperatura vale: " + String(T_temp) + "�C" + "\nLa humedad vale: " + String(T_hum) + "%");
	  }
		flagDHT22 = 1;
		if(DBUG == 1){
			Serial.println("La variable flagDHT22 tiene el valor de:  " + String (flagDHT22));
		}
	}
}
*/
/**
Esta funcion calcula el ITH a trav�s de la temperatura y la humedad que ha obtenido el sensor
@param temperatura tipo float
@param humedad tipo float
@returns int en este caso el valor del ITH siendo este un entero
@note Calcula el �ndice ITH
*/
int calcularITH (float temperatura,float humedad){
  auxTemperatura = 0;
	auxHumedad = 0;
	sumaT = 0;
	sumaH = 0;
	iDHT = 0;   
	float ITH = (((1.8 * temperatura) + 32.0) - (0.55 - (((0.55 * humedad) / 100.0))) * ((1.8 * temperatura) - 26));
  //float ITH = (((1.8 * temperatura) + 32.0) + (0.55 - (((0.55 * humedad) / 100.0))) * ((1.8 * temperatura) - 26));
	int RoundITH = int(round(ITH));
  if(DBUG == 1){
		Serial.println("El ITH tiene un valor de: " + String (RoundITH));
		}  
  return RoundITH;     
}


