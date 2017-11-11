#include <stdio.h>
#include <stdlib.h>

#fuses NOWDT, XT, PUT, NOPROTECT, NODEBUG, NOBROWNOUT, NOLVP, NOCPD, NOWRT
#use delay(clock=4000000)

#include "lib_rf2gh4_10.h"

#byte porta=0x05
#byte portb=0x06

#int_ext                     // Esta rutina est� para un futuro si haces comunicaciones bidireccionales.
void int_RB0()               // No tiene efecto en el programa principal, ya que s�lo emite.
{                            // Se encargar�a de la recepci�n de datos.
   int8 ret1;
   
   ret1 = RF_RECEIVE();
   if ( (ret1 == 0) || (ret1 == 1) )
   {
      do
      {  
         ret1 = RF_RECEIVE(); 
      }while ( (ret1 == 0) || (ret1 == 1) );
   }  
}

void main()
{  
   int8 ret2;

   RF_INT_EN();              // Habilitar interrupci�n RB0/INT.
   RF_CONFIG_SPI();          // Configurar m�dulo SPI del PIC.
   RF_CONFIG(0x40,0x01);     // Configurar m�dulo RF canal y direcci�n de recepci�n. Para recibir datos tiene la direcci�n 0x01.
   RF_ON();                  // Activar el m�dulo RF.
   
   delay_ms(5);              // Le damos un m�nimo de 2.5 milisegundos para que se ponga en marcha.
   
   set_tris_a(0b111111);     // Todo el puerto A como entradas.
   
   while(true)               // Bucle infinito.
   {  
      RF_DATA[0]=porta;      // Cargamos el dato en RF_DATA[0].
      RF_DIR=0x08;           // Direcci�n del receptor.
      ret2=RF_SEND();        // Enviar datos.
   }
}