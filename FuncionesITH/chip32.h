/**
  @file chip32.h
  @brief funciones principales para obtener el serial number del dispositivo
	Obtener el serial number y pasar de una variable de 64 bits a un string.
	@author Antonio Cánovas
  @date 11/02/2019
*/


/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																									CHIP ESP32                                                                 //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																									DEFINE Y VARIABLES GLOBALES                                                                             //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
// VARIABLE DE 64 BITS QUE RECOGE CHIP ID
uint64_t chipidReal;
String T_StringRealID = "";
String R_StringRealID = "";
uint32_t chipid;
String R_chipid = "";
String R_EIC = "";
uint32_t T_EIC;
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																									PROTOTIPADO DE FUNCIONES                                                                       //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
void chipID (void);
String uint64ToString(uint64_t);
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																									FUNCIONES													                                                                 //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
/**
Esta funcion obtiene el serial number del dispositivo.
@note Se usará una variable de 32 bits para quedarnos con la parte baja del dispositivo y una de 64 bits, para pasar a String el Serial Number
*/
void chipID (){
	chipid = ESP.getEfuseMac();						//The chip ID is essentially its MAC address(length: 6 bytes).
	chipidReal = ESP.getEfuseMac();
	T_StringRealID = uint64ToString(chipidReal);
	if (DBUG == 1){	
		Serial.printf("ESP32 Chip ID = %04X",(uint16_t)(chipid>>32));//print High 2 bytes
		Serial.printf("%08X\n",(uint32_t)chipid);//print Low 4bytes.
		Serial.println("ESP32 RealChip: " + T_StringRealID);
	}
}
/**
Esta funcion convierte el dato de entrada (64 bits) en una cada (String)
@param input entramos el serial number (64 bits)
@returns almacena en una variable de tipo String el resultado
*/
String uint64ToString(uint64_t input) {
  String result = "";
  uint8_t base = 10;

  do {
    char c = input % base;
    input /= base;

    if (c < 10)
      c +='0';
    else
      c += 'A' - 10;
    result = c + result;
  } while (input);
  return result;
}