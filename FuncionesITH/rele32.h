/**
  @file rele32.h
  @brief funciones principales para gestionar los 4 reles
	El rele0 corresponde al pin 2
	El rele1 corresponde al pin 12
	El rele2 corresponde al pin 13
  El rele3 corresponde al pin 23
	Configuramos los reles, activamos y desactivamos agrupados e individuales
	@author Antonio Cánovas
  @date 07/02/2019

*/


/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																										RELES                                                                                          //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																									DEFINE Y VARIABLES GLOBALES                                                                             //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//CADA RELÉ SE ENCUENTRA ASOCIADO A UN PIN
#define R0 2
#define R1 12
#define R2 13
#define R3 23
// VARIABLES DE ESTADO DE LOS RELES PARA ENVIO
bool T_StatusR0 = 0;
bool T_StatusR1 = 0;
bool T_StatusR2 = 0;
bool T_StatusR3 = 0;

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																										PROTOTIPADO DE FUNCIONES                                                                       //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
void configurarRele(void);
void des_activaReles(bool);
void des_activaRele0 (bool);
void des_activaRele1 (bool);
void des_activaRele2 (bool);
void des_activaRele3 (bool);
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																										FUNCIONES                                                                                      //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
/**
Esta funcion configura los reles como salidas
@note Configura los relés como salidas, indicando el pin en el que se encuentran*/
void configurarRele(void){
  pinMode (R0, OUTPUT);
  pinMode (R1, OUTPUT);
  pinMode (R2, OUTPUT);
  pinMode (R3, OUTPUT);
	if(DBUG == 1){
	Serial.println("Los relés han sido configurados como salidas");
	}
}
/**
Esta funcion configura los reles, haciendo un apagado o encendido general
@param estado variable bool que nos indicará si apagar o encender los reles
@note Esta funcion admite un nivel alto o bajo de entrada para activar o desactivar
el grupo de reles*/
void des_activaReles(bool estado){
  if (estado == HIGH){
    digitalWrite(R0, HIGH);
    digitalWrite(R1, HIGH);
    digitalWrite(R2, HIGH);
    digitalWrite(R3, HIGH);
    T_StatusR0 = HIGH;
    T_StatusR1 = HIGH;
    T_StatusR2 = HIGH;
    T_StatusR3 = HIGH;
			if(DBUG == 1){
				Serial.println("reles activados");
				errorOLED("Reles activados");
			}
		}
  else{
    digitalWrite(R0, LOW);
    digitalWrite(R1, LOW);
    digitalWrite(R2, LOW);
    digitalWrite(R3, LOW);
    T_StatusR0 = LOW;
    T_StatusR1 = LOW;
    T_StatusR2 = LOW;
    T_StatusR3 = LOW;
			if(DBUG == 1){
				Serial.println("Reles desactivados");
				errorOLED("Reles desactivado");
			}
		}
  }
/**
Esta funcion configura el rele0 para poder activarlo o desactivarlo
@param estado variable bool que nos indicará si queremos activar o desactivar
@note Estado alto activa, estado bajo desactiva
*/
void des_activaRele0 (bool estado){
	if (estado == HIGH){
		digitalWrite(R0, HIGH);
		T_StatusR0 = HIGH;
		if(DBUG == 1){
				Serial.println("Rele0 activado");
			}
	}
	else{
		digitalWrite(R0, LOW);
		T_StatusR0 = LOW;
		if(DBUG == 1){
				Serial.println("Rele0 desactivado");
			}
	}
}
/**
Esta funcion configura el rele1 para poder activarlo o desactivarlo
@param estado variable bool que nos indicará si queremos activar o desactivar
@note Estado alto activa, estado bajo desactiva
*/
void des_activaRele1 (bool estado){
	if (estado == HIGH){
		digitalWrite(R1, HIGH);
		T_StatusR1 = HIGH;
		if(DBUG == 1){
				Serial.println("Rele1 activado");
			}
	}
	else{
		digitalWrite(R1, LOW);
		T_StatusR1 = LOW;
		if(DBUG == 1){
				Serial.println("Rele1 desactivado");
			}
	}
}
/**
Esta funcion configura el rele2 para poder activarlo o desactivarlo
@param estado variable bool que nos indicará si queremos activar o desactivar
@note Estado alto activa, estado bajo desactiva
*/
void des_activaRele2 (bool estado){
	if (estado == HIGH){
		digitalWrite(R2, HIGH);
		T_StatusR2 = HIGH;
		if(DBUG == 1){
				Serial.println("Rele2 activado");
			}
	}
	else{
		digitalWrite(R2, LOW);
		T_StatusR2 = LOW;
		if(DBUG == 1){
				Serial.println("Rele2 desactivado");
			}
	}
}
/**
Esta funcion configura el rele3 para poder activarlo o desactivarlo
@param estado variable bool que nos indicará si queremos activar o desactivar
@note Estado alto activa, estado bajo desactiva
*/
void des_activaRele3 (bool estado){
	if (estado == HIGH){
		digitalWrite(R3, HIGH);
		T_StatusR3 = HIGH;
		if(DBUG == 1){
				Serial.println("Rele3 activado");
			}
	}
	else{
		digitalWrite(R3, LOW);
		T_StatusR3 = LOW;
		if(DBUG == 1){
				Serial.println("Rele3 desactivado");
			}
	}
}	