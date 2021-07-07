/**
  @file timers32.h
  @brief funciones principales para gestionar los timers
	Aqui encontramos las rutinas de servicio a la interrupcion, la configuracion
	y las funciones usadas para arrancar los timers.
  @author Antonio C�novas
  @date 07/02/2019
*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																										TIMERS                                                                                         //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																									DEFINE Y VARIABLES GLOBALES                                                                             //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
#define DBUG 1
// DEFINIMOS LAS VARIABLES DE LOS 3 TIMERS QUE NECESITAMOS DE LOS RELES
//T0 = 20min || T1 = 15min || T2 = 5min 
#define T0 0
#define minutos19 1170000000
#define T1 1
#define minutos15 900000000
#define T2 2
#define minutos5  300000000
// 80Mhz de ciclo de reloj, dividiremos entre 80 para que valga un segundo cada cuenta del timer
#define reloj 80
//DEFINIMOS OTRO TIMER QUE SER� PARA CONTABILIZAR EL PASO DE 20MIN QUE SER� CUANDO EL CONCENTRADOR HAGA LA LECTURA DEL SENSOR.
//PARA ELLO USAREMOS T3 = 20MIN
#define T3 3
#define minutos20 1200000000 //300000000 10000 //

hw_timer_t * timer0 = NULL;
volatile bool contadorInterrupcion20 = 0;
int contadorTotalInterrupcion20 = 0;
portMUX_TYPE timer0Mux = portMUX_INITIALIZER_UNLOCKED;

hw_timer_t * timer1 = NULL;
volatile bool contadorInterrupcion15 = 0;
int contadorTotalInterrupcion15 = 0;
portMUX_TYPE timer1Mux = portMUX_INITIALIZER_UNLOCKED;

hw_timer_t * timer2 = NULL;
volatile bool contadorInterrupcion5 = 0;
int contadorTotalInterrupcion5 = 0;
portMUX_TYPE timer2Mux = portMUX_INITIALIZER_UNLOCKED;

hw_timer_t * timer3 = NULL;
volatile bool contadorInterrupcionConcentrador = 1;
int contadorTotalInterrupcionConcentrador = 0;
portMUX_TYPE timer3Mux = portMUX_INITIALIZER_UNLOCKED;
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																										PROTOTIPADO DE FUNCIONES                                                                       //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
void configurarTimer(bool, bool, bool, bool);
void arrancarTimer(bool, bool, bool, bool);
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
//																										FUNCIONES                                                                                      //
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

/**
RUTINA DE SERVICIO A LA INTERRUPCI�N DE TIMER0 (19min)
@note Cada vez que pasen 19 minutos se ejecutar� esta interrupcion que consistir� en activar una bandera indicando que ha pasado dicho tiempo
esta es probocada por el timer 0, que se lanzar� cuando se activen los rel�s 2 y 3 de forma autom�tica.
*/
void IRAM_ATTR onTimer0(){
  portENTER_CRITICAL_ISR(&timer0Mux);
  contadorInterrupcion20 = 1;
  portEXIT_CRITICAL_ISR(&timer0Mux);
}
/**
RUTINA DE SERVICIO A LA INTERRUPCI�N DE TIMER1 (15min)
@note Cada vez que pasen 15 minutos se ejecutar� esta interrupcion que consistir� en activar una bandera indicando que ha pasado dicho tiempo
esta es probocada por el timer 1, que se lanzar� cuando se activen el rel� 1 de forma autom�tica.
*/
void IRAM_ATTR onTimer1(){
  portENTER_CRITICAL_ISR(&timer1Mux);
  contadorInterrupcion15 = 1;
  portEXIT_CRITICAL_ISR(&timer1Mux);
}
/**
RUTINA DE SERVICIO A LA INTERRUPCI�N DE TIMER1 (5min)
@note Cada vez que pasen 5 minutos se ejecutar� esta interrupcion que consistir� en activar una bandera indicando que ha pasado dicho tiempo
esta es probocada por el timer 2, que se lanzar� cuando se activen el rel� 0 de forma autom�tica.
*/
void IRAM_ATTR onTimer2(){
  portENTER_CRITICAL_ISR(&timer2Mux);
  contadorInterrupcion5 = 1;
  portEXIT_CRITICAL_ISR(&timer2Mux);
}
/**
RUTINA DE SERVICIO A LA INTERRUPCI�N DE TIMER1 (20min)
@note Cada vez que pasen 20 minutos se ejecutar� esta interrupcion que consistir� en activar una bandera indicando que ha pasado dicho tiempo.
Esta interrupcion se ejecutar� siempre y cuando el dispositivo no tenga bater�a, de este modo cada 20 minutos tomar� los datos de humedad y temperatura.
*/
void IRAM_ATTR onTimer3(){
  portENTER_CRITICAL_ISR(&timer3Mux);
  contadorInterrupcionConcentrador = 1;
  portEXIT_CRITICAL_ISR(&timer3Mux);
}
/**
Esta funcion configura los timers, para poder arrancarlos
@param T__0 variable bool que nos indicar� si queremos configurar o no el timer0
@param T__1 variable bool que nos indicar� si queremos configurar o no el timer1
@param T__2 variable bool que nos indicar� si queremos configurar o no el timer2
@param T__3 variable bool que nos indicar� si queremos configurar o no el timer3
@note Inicializacion: Cada timer tiene detr�s su n�mero correspondiente, lo inicializamos con n�mero, dividimos entre 80 para
que la cuenta sea cada segundo y cuenta ascendente. Interrupcion Timer: Declaramos la interrupcion de cada timer y le asignamos cual va a ser su subrutina.
Configuracion: Para que timer es, duracion del timer, repeticion peridodica o no.
*/
void configurarTimer(bool T__0, bool T__1, bool T__2, bool T__3){
	if (DBUG == 1){
	Serial.println("Configurando los timers...");
	}
	
	if (T__0 == HIGH){
	timer0 = timerBegin(T0, reloj, true);       
  timerAttachInterrupt(timer0, &onTimer0, true);    
  timerAlarmWrite(timer0, minutos19, false); 						
	}
	if (T__1 == HIGH){
	timer1 = timerBegin(T1, reloj, true);        
  timerAttachInterrupt(timer1, &onTimer1, true);   
  timerAlarmWrite(timer1, minutos15, false); 				
	}
	if (T__2 == HIGH){
	timer2 = timerBegin(T2, reloj, true);       
  timerAttachInterrupt(timer2, &onTimer2, true);
  timerAlarmWrite(timer2, minutos5, false); 
	}
	if (T__3 == HIGH){
	timer3 = timerBegin(T3, reloj, true);     
  timerAttachInterrupt(timer3, &onTimer3, true);  
  timerAlarmWrite(timer3, minutos20, true); 
	}
	if (DBUG == 1){
	Serial.println("Los timer han sido configurados con éxito");
	}
}
/**
Esta funcion arranca los timers
@param T__0 variable bool que nos indicar� si queremos arrancar o no el timer0
@param T__1 variable bool que nos indicar� si queremos arrancar o no el timer1
@param T__2 variable bool que nos indicar� si queremos arrancar o no el timer2
@param T__3 variable bool que nos indicar� si queremos arrancar o no el timer3
@note Activamos cada timer en funcion de su valor correspondiente de entrada*/
void arrancarTimer(bool T__0, bool T__1, bool T__2,bool T__3){
	if (DBUG == 1){
	Serial.println("Arrancando los timers...");
	}
	if (T__0 == HIGH){
	timerAlarmEnable(timer0); 
	}
	if (T__1 == HIGH){
	timerAlarmEnable(timer1);
	}
	if (T__2 == HIGH){
	timerAlarmEnable(timer2);
	}
	if (T__3 == HIGH){
	timerAlarmEnable(timer3);
	}
	if (DBUG == 1){
	Serial.println("Los timers han sido arrancandos con �xito");
	}
}

