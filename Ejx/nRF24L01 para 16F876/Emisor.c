#include <16F876A.h>
#FUSES NOWDT, XT, PUT, NOPROTECT, NODEBUG, NOBROWNOUT, NOLVP, NOCPD, NOWRT
#use delay(clock=4000000)

#include "lib_rf2gh4_10.h"

#byte porta=0x05
#byte portb=0x06

#int_ext
void int_RB0()
{
   int8 ret1;
   
   ret1 = RF_RECEIVE();
   if ( (ret1 == 0) || (ret1 == 1) )
   {
      do
      {  
         ret1 = RF_RECEIVE(); 
      }  while ( (ret1 == 0) || (ret1 == 1) );
   }  
}

void main()
{  
   int8 ret2;

   RF_INT_EN();              // Habilitar interrupción RB0/INT.
   RF_CONFIG_SPI();          // Configurar módulo SPI del PIC.
   RF_CONFIG(0x40,0x01);     // Configurar módulo RF canal y dirección.
   RF_ON();                  // Activar el módulo RF.
   
   delay_ms(5);
   
   set_tris_a(0b111111);     // Todo el puerto A como entradas.
   
   while(true)
   {  
      RF_DATA[0]=porta;
      RF_DIR=0x08;           // Dirección del receptor.
      ret2=RF_SEND();        // Enviar datos.
   }
}
