#include <stdio.h>
#include <stdlib.h>

#fuses NOWDT, XT, PUT, NOPROTECT, NODEBUG, NOBROWNOUT, NOLVP, NOCPD, NOWRT
#use delay(clock=4000000)

#include "lib_rf2gh4_10.h"

#byte porta=0x05
#byte portb=0x06

#int_ext                     // Esta rutina está para un futuro si haces comunicaciones bidireccionales.
void int_RB0()               // No tiene efecto en el programa principal, ya que sólo emite.
{                            // Se encargaría de la recepción de datos.
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

   RF_INT_EN();              // Habilitar interrupción RB0/INT.
   RF_CONFIG_SPI();          // Configurar módulo SPI del PIC.
   RF_CONFIG(0x40,0x01);     // Configurar módulo RF canal y dirección de recepción. Para recibir datos tiene la dirección 0x01.
   RF_ON();                  // Activar el módulo RF.
   
   delay_ms(5);              // Le damos un mínimo de 2.5 milisegundos para que se ponga en marcha.
   
   set_tris_a(0b111111);     // Todo el puerto A como entradas.
   
   while(true)               // Bucle infinito.
   {  
      RF_DATA[0]=porta;      // Cargamos el dato en RF_DATA[0].
      RF_DIR=0x08;           // Dirección del receptor.
      ret2=RF_SEND();        // Enviar datos.
   }
}