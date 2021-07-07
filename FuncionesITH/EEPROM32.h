/**
  @file EEPROM32.h
  @brief funciones principales para gestionar la memoria EEPROM 24LC1025
	Aqui encontramos las funciones de escritura y lectura hacia una memoria 24LC1025.
  @author Antonio Cánovas
  @date 11/02/2019
*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																										EEPROM                                                                                         //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
#include <Wire.h>
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																									PROTOTIPADO DE FUNCIONES                                                                         //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
void i2c_eeprom_write_byte (int, unsigned int, byte); 
void i2c_eeprom_write_page(int, unsigned int, byte*, byte);
byte i2c_eeprom_read_byte( int, unsigned int);
void i2c_eeprom_read_buffer( int, unsigned int, byte*, int);
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																										FUNCIONES                                                                                      //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
/**
Esta funcion permite la escritura en una posicion de la memoria
@param deviceaddress direccion en la que se encuentra la memoria
@param eeaddress direccion en la que quiereres escribir en la memoria
@param data dato que quieres escribir en la memoria
@note Comenzamos la transmisición, indicamos la posicion de meoria y escribimos el dato.
*/
void i2c_eeprom_write_byte (int deviceaddress, unsigned int eeaddress, byte data) {
  int rdata = data;
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8)); // Bit más significativo
  Wire.write((int)(eeaddress & 0xFF)); // Bit menos signiticativo
  Wire.write(rdata);
	Serial.println(rdata);
  Wire.endTransmission();
}
/**
Esta funcion permite la escritura en una posicion de la memoria de manera paginada
@param deviceaddress direccion en la que se encuentra la memoria
@param eeaddresspage direccion de la pagina en la que quiereres escribir en la memoria
@param data cadena de datos que quieres escribir en la memoria
@param length longitud de la cadena de datos que quieres escribir en la memoria
@note Comenzamos la transmisición, indicamos la posicion de la pag de la meoria y escribimos la cadena.
ADVERTENCIA: la dirección es una dirección de página, el final de 6 bits se ajustará
también, los datos pueden tener un máximo de aproximadamente 30 bytes, debido a que la biblioteca de Wire tiene un búfer de 32 bytes
*/
void i2c_eeprom_write_page(int deviceaddress, unsigned int eeaddresspage, byte* data, byte length) {
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddresspage >> 8));
  Wire.write((int)(eeaddresspage & 0xFF));
  byte c;
  for (c = 0; c < length; c++) {
    Wire.write(data[c]);
		delay(500);
		Serial.print(data[c]);
  }
  Wire.endTransmission();
	Serial.println("\nFin de escritura");
}
/**
Esta funcion permite la lectura en una posicion de la memoria
@param deviceaddress direccion en la que se encuentra la memoria
@param eeaddress direccion en la que quiereres leer en la memoria
@returns rdata tipo byte que contiene el contenido de esa posicion de memoria
@note Comenzamos la transmisición, indicamos la posicion de meoria y mientras haya dato leemos.
*/
byte i2c_eeprom_read_byte( int deviceaddress, unsigned int eeaddress ) {
  byte rdata = 0xFF;
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress, 1);
  if (Wire.available()) rdata = Wire.read();
	return rdata;
}
/**
Esta funcion permite la lectura en una posicion de la memoria
@param deviceaddress direccion en la que se encuentra la memoria
@param eeaddress direccion en la que quiereres leer en la memoria
@param *buffer buffer para leer de la memoria
@note quizas no lea mas de 30 o 32 bytes al mismo tiempo!.
*/
void i2c_eeprom_read_buffer( int deviceaddress, unsigned int eeaddress, byte *buffer, int length ) {
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress, length);
  int c = 0;
  for ( c = 0; c < length; c++ )
    if (Wire.available()) buffer[c] = Wire.read();
}
/*  // put your setup code here, to run once:
  char somedata[] = "Esto esta en la EEPROM"; // data to write
	i2c_eeprom_write_page(0x50, 0, (byte *)somedata, sizeof(somedata)); // write to EEPROM
*/
/**
Esta funcion inicializa al EEPROM, ajusta la frecuencia para una resistencia de 10KOhm
@note F=10KHz, R=10KHz , PULL UP.
*/

void inicializarEEPROM() {
  Wire.begin(); // initialise the connection
  Wire.setClock(10000);
	delay(100); //add a small delay
  Serial.println("Memory written");
}
/**
Esta funcion es un ejemplo de como leer la memoria mientras haya un dato
@note NO USADA EN EL FLUJO DE PROGRAMA.
*/
void bucleRepetitivoLoop() {
  // put your main code here, to run repeatedly:
  int addr = 0; //first address
  byte b = i2c_eeprom_read_byte(0x50, 0); // access the first address from the memory

  while (b != 0)
  {
    Serial.print((char)b); //print content to serial port
    addr++; //increase address
    b = i2c_eeprom_read_byte(0x50, addr); //access an address from the memory
  }
  Serial.println(" ");
  delay(2000);

}