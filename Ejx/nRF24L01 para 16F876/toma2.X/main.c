#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

// PIC16F876 Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config CP = OFF         // FLASH Program Memory Code Protection bits (Code protection off)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low Voltage In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection (Code Protection off)
#pragma config WRT = OFF        // FLASH Program Memory Write Enable (Unprotected program memory may not be written to by EECON control)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include"lib_rf2gh4_10.h"
#define	_XTAL_FREQ 4000000	
//
porta=0x05;           // Dirección del puerto A.

int_ext();                   // Interrupción del módulo RF.
void int_RB0()
{
   int8 ret1;

   ret1 = RF_RECEIVE();
   if ( (ret1 == 0) || (ret1 == 1) )
   {
      do
      {    
         porta=RF_DATA[0];  // El puerto A contendrá el valor que le llege del emisor, a través de RF_DATA[0].
         ret1 = RF_RECEIVE(); 
      } while ( (ret1 == 0) || (ret1 == 1) );
   }  
}

void main()                  //Programa principal
{
   set_tris_a(0b000000);     // Todo el puerto A como salida.
   porta=0;                  // Inicialmente lo ponemos a cero.
   
   RF_INT_EN();              // Habilitar interrupción RB0/INT.
   RF_CONFIG_SPI();          // Configurar módulos SPI del PIC.
   RF_CONFIG(0x40,0x08);     // Configurar módulo RF (canal y dirección).
   RF_ON();                  // Activar el módulo RF.
   
   while(true);              // Bucle infinito. 
       
}