#include <16F876A.h>
#FUSES NOWDT, XT, PUT, NOPROTECT, NODEBUG, NOBROWNOUT, NOLVP, NOCPD, NOWRT
#use delay(clock=4000000)

#include "lib_rf2gh4_10.h"

#byte porta=0x05           // Dirección del puerto A.

#int_ext                   // Interrupción del módulo RF.
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


