/**
  @file oled32.h
  @brief funciones principales para configurar y mostrar por pantalla.
  @author Antonio Cánovas
  @date 07/02/2019
*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																										PANTALLA OLED 0.96"                                                                            //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																										CONJUNTO DE LIBRERIAS                                                                          //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
#include <SSD1306.h> // PANTALLA 0.96 PULGADAS
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																										DEFINE Y VARIABLES GLOBALES                                                                    //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
#define DBUG 1
// DEFINE EL PIN CORRESPONDIENTE AL REINICIO DE LA PANTALLA PARA INICIALIZARLA
#define OLED 16
//display PARA DIRIGIRNOS HACIA LA PANTALLA
SSD1306  display(0x3c, 4, 15);
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																										PROTOTIPADO DE FUNCIONES                                                                       //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
void iniciarOLED(void);
void errorOLED(String);
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																										FUNCIONES                                                                                      //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

//INICIALIZA LA PANTALLA EN CASO DE QUEREER USARLA

/**
Inicializacion de la pantalla OLED
@note Necesita un reinicio del pin 16 para su funcionamiento*/
void iniciarOLED(){
	// EN CASO DE NECESITAR LA PANTALLA SE NECESITA UN RESET DE SU PIN PARA EL FUNCIONAMIENTO
  pinMode(OLED,OUTPUT);
  digitalWrite(OLED, LOW);
  delay(50);
  digitalWrite(OLED, HIGH); // while OLED is running, must set GPIO16 in high
  display.init();
  display.setContrast(255);
	if(DBUG == 1){
			Serial.println("La pantalla ha sido inicializada");
	}
}

/**
Muestra por pantalla la cadena de texto que se le ponga
@param CadenaError variable tipo String que aparecerá por pantalla
@note Cada vez que se llama, limpia la pantalla, escribe y los mostramos
*/
void errorOLED(String CadenaError){
  display.clear();
  display.drawString(0,20, CadenaError); 
  display.display();
		if(DBUG == 1){
			Serial.println("La pantalla esta escribiendo: " + CadenaError);
	}
}


