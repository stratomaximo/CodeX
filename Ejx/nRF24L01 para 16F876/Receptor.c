#include <16F876A.h>
#FUSES NOWDT, XT, PUT, NOPROTECT, NODEBUG, NOBROWNOUT, NOLVP, NOCPD, NOWRT
#use delay(clock=4000000)

#include "lib_rf2gh4_10.h"

#byte porta=0x05           // Direcci�n del puerto A.

#int_ext                   // Interrupci�n del m�dulo RF.
void int_RB0()
{
   int8 ret1;

   ret1 = RF_RECEIVE();
   if ( (ret1 == 0) || (ret1 == 1) )
   {
      do
      {    
         porta=RF_DATA[0];  // El puerto A contendr� el valor que le llege del emisor, a trav�s de RF_DATA[0].
         ret1 = RF_RECEIVE(); 
      } while ( (ret1 == 0) || (ret1 == 1) );
   }  
}

void main()                  //Programa principal
{
   set_tris_a(0b000000);     // Todo el puerto A como salida.
   porta=0;                  // Inicialmente lo ponemos a cero.
   
   RF_INT_EN();              // Habilitar interrupci�n RB0/INT.
   RF_CONFIG_SPI();          // Configurar m�dulos SPI del PIC.
   RF_CONFIG(0x40,0x08);     // Configurar m�dulo RF (canal y direcci�n).
   RF_ON();                  // Activar el m�dulo RF.
   
   while(true);              // Bucle infinito. 
       
}


