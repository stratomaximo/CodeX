#include <16f876.h>
#include <stdlib.h>
#include <stdio.h>
#FUSES NOWDT, XT, PUT, NOPROTECT, NODEBUG, NOBROWNOUT, NOLVP, NOCPD, NOWRT
#use delay(clock=4000000)

#include <nRF24L01.h>

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