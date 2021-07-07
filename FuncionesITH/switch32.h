/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																										SWITCH                                                                                         //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																									DEFINE Y VARIABLES GLOBALES                                                                             //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
#define DBUG 1
// DEFINE LAS VARIABLES CORRESPONDIENTES A LOS PINES DEL SWITCH
#define Sx 17
#define Sy 38
#define Sz 21
#define St 4
int tipoFuncion; //cogera el valor en funcion de cuales sean las patillas del switch
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																										PROTOTIPADO DE FUNCIONES                                                                       //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
void configurarSwitch(void);
int leerSwitch(void);
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																									FUNCIONES													                                                                 //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

// CONFIGURA LOS PINES DEL SWITCH COMO ENTRADAS 
void configurarSwitch (){
  pinMode(Sx,INPUT);
  pinMode(Sy,INPUT);
  pinMode(Sz,INPUT);
  pinMode(St,INPUT);
	if(DBUG == 1){
	Serial.println("Las entradas del switch han sido configuradas como entradas");
	}
}
// LEE LAS ENTRADAS DEL SWITCH PARA IDENTIFICAR QUE VALOR TIENE   //Siendo Sx el bit LSB y St el MSB
int  leerSwitch (void) {
  byte binario = digitalRead(Sx) + digitalRead(Sy) <<1  + digitalRead(Sz) <<2 + digitalRead(St) << 3;     
  int decimal = (int) Binario;
	if(DBUG == 1){
		Serial.print("el valor del Switch en binario es: ");
		Serial.println(Binario);
		Serial.print("El valor del Switch en decimal es: ");
		Serial.println(Decimal);
	}
	return Decimal;
}
