//********************************************************************************
//*    lib_rf2gh4_10.h                                                           *
//*     version: 1.0                                                             *
//*     Esta librer�a contiene las funciones necesarias para gestionar el m�dulo *
//*    RF2GH4 con programas del compilador CCS                                   *
//*    Copyright (C) 2007  Bizintek Innova S.L.                                  *
//********************************************************************************
//*    This program is free software; you can redistribute it and/or modify      *
//*    it under the terms of the GNU General Public License as published by      *
//*    the Free Software Foundation; either version 2 of the License, or         *
//*    (at your option) any later version.                                       *
//*                                                                              *
//*    This program is distributed in the hope that it will be useful,           *
//*    but WITHOUT ANY WARRANTY; without even the implied warranty of            *
//*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
//*    GNU General Public License for more details.                              *
//*                                                                              *
//*    You should have received a copy of the GNU General Public License along   *
//*    with this program; if not, write to the Free Software Foundation, Inc.,   *
//*    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.               *
//********************************************************************************


//********************
//*   DEFINICIONES   *
//********************

// PORTB
#define RF_IRQ        PIN_B0
#define RF_IRQ_TRIS   TRISB,0

// PORTC
#define   RF_CS       PIN_C1
#define   RF_CE       PIN_C2
#define   SCK         PIN_C3
#define   SDI         PIN_C4
#define   SDO         PIN_C5
                      
#define   RF_CS_TRIS  TRISC,1
#define   RF_CE_TRIS  TRISC,2
#define   SCK_TRIS    TRISC,3
#define   SDI_TRIS    TRISC,4
#define   SDO_TRIS    TRISC,5



//*****************
//*   VARIABLES   *
//*****************
#BYTE TRISA     =  0x85
#BYTE TRISB     =  0x86
#BYTE TRISC     =  0x87
#BYTE INTCON    =  0x0B

//Variables internas
static int1        interRF;
static int16       noRF;
static int1        RCVNW=0;
static int8        DATA_N_SND=0;
static int8        DATA_N_RCV=0;

//Variables configurables
static int8        RF_DATA[8];
static int8        RF_DIR;


//**************
//*   C�DIGO   *
//**************

//*****************************************************
//*               RF_CONFIG_SPI()                     *
//*****************************************************
//*Descripci�n: La funci�n configura el m�dulo SPI del*
//*microcontrolador.En ella se especifica como salida *
//*SDO y como entrada SDI entre otros par�metros del  *
//*protocolo SPI.                                     *
//*****************************************************
//*Variables de entrada:                              *
//*Variables de salida:                               *
//*****************************************************
void RF_CONFIG_SPI()
{
   //Configuraci�n I/O.
   bit_clear(SCK_TRIS);
   bit_set(SDI_TRIS);
   bit_clear(SDO_TRIS);

   //Configuraci�n m�dulo comunicaciones.
   setup_spi(SPI_MASTER|SPI_L_TO_H|SPI_XMIT_L_TO_H
   |SPI_CLK_DIV_4|SPI_SAMPLE_AT_END);
}
//*****************************************************





//*****************************************************
//*                    RF_INT_EN()                    *
//*****************************************************
//*Descripci�n:Se encarga de habilitar la interrupci�n*
//*externa (RB0) utilizada por el m�dulo de RF en la  *
//*recepci�n de datos.                                *
//*****************************************************
//*Variables de entrada:                              *
//*Variables de salida:                               *
//*****************************************************
void RF_INT_EN()
{
   //Habilitar interrupciones externas con flanco de
   //bajada.
   disable_interrupts(global);
   enable_interrupts(int_ext);
   ext_int_edge( H_TO_L );
   bit_set(RF_IRQ_TRIS);
   enable_interrupts(global);
}
//*****************************************************





//*****************************************************
//*        RF_CONFIG(int canal, int dir)              *
//*****************************************************
//*Descripci�n:Esta funci�n se encarga de configurar  *
//*el transceptor habilitando su propia direcci�n de  *
//*escucha y el canal entre otros par�metros.         *
//*****************************************************
//*Variables de entrada:- Canal                       *
//*                     - Direccion                   *
//*Variables de salida:                               *
//*****************************************************
void RF_CONFIG(int canal, int dir)
{

   bit_clear(RF_CS_TRIS);
   bit_set(RF_IRQ_TRIS);
   bit_clear(RF_CE_TRIS);

   output_low(RF_CE);

   // TX_ADDR (0xFF)
   //Configuraci�n de la direcci�n de envio aleatoria.
   //En la funci�n de enviar se configura la direccion
   //deseada por el usuario.
   output_low(RF_CS);
   spi_write(0x30);
   spi_write(0xFF);
   spi_write(0xC2);
   spi_write(0xC2);
   spi_write(0xC2);
   spi_write(0xC2);
   output_high(RF_CS);

   // RX_ADDR_P0 (0xFF) ACK
   //Configuraci�n de la direcci�ndel Pipe0 para la
   //recepci�n de ACK.
   output_low(RF_CS);
   spi_write(0x2A);
   spi_write(0xFF);
   spi_write(0xC2);
   spi_write(0xC2);
   spi_write(0xC2);
   spi_write(0xC2);
   output_high(RF_CS);

   // RX_ADDR_P1 (dir)
   //Configuraci�n de la direcci�ndel Pipe1 para la
   //recepci�n de tramas.
   output_low(RF_CS);
   spi_write(0x2B);
   spi_write(dir);
   spi_write(0xC2);
   spi_write(0xC2);
   spi_write(0xC2);
   spi_write(0xC2);
   output_high(RF_CS);

   // RX_ADDR_P2 (0x00) BROADCAST
   //Configuraci�n de la direcci�ndel Pipe2 para la
   //recepci�n de tramas
   output_low(RF_CS);
   spi_write(0x2C);
   spi_write(0x00);
   output_high(RF_CS);

   // EN_AA
   //Habilitar AutoAck en los Pipe0,Pipe1 y Pipe2.
   output_low(RF_CS);
   spi_write(0x21);
   spi_write(0x07);
   output_high(RF_CS);

   // EN_RXADDR
   //Habilitar los Pipe0,Pipe1 y Pipe2.
   output_low(RF_CS);
   spi_write(0x22);
   spi_write(0x07);
   output_high(RF_CS);

   // SETUP_AW
   //Configuraci�n de la longitud de las direcciones.
   //Direcciones de 5 bytes.
   output_low(RF_CS);
   spi_write(0x23);
   spi_write(0x03);
   output_high(RF_CS);

   //SETUP_RETR
   //Configuraci�n de las retrasmisiones en la transmisi�n.
   //Diez retransmisiones cada 336us.
   output_low(RF_CS);
   spi_write(0x24);
   spi_write(0x0A);
   output_high(RF_CS);

   //RF_CH
   //Configuraci�n del canal.
   //Canal elegido por el usuario (0x01 - 0x7F).
   output_low(RF_CS);
   spi_write(0x25);
   spi_write(canal);
   output_high(RF_CS);

   //RF_SETUP
   //Configuraci�n aspectos RF.
   //Ganancia m�xima de LNA, 0dBm potencia de salida y 1Mbps de velocidad.
   output_low(RF_CS);
   spi_write(0x26);
   spi_write(0x07);
   output_high(RF_CS);

   //STATUS
   //Reseteo del registro STATUS
   output_low(RF_CS);
   spi_write(0x27);
   spi_write(0x70);
   output_high(RF_CS);

   //RX_PW_P0
   //N� de bytes en Pipe0.
   //1 byte (ACK).
   output_low(RF_CS);
   spi_write(0x31);
   spi_write(0x01);
   output_high(RF_CS);

   //RX_PW_P1
   //N� de bytes en Pipe1.
   //10 byte (Direccion emisor y trama).
   output_low(RF_CS);
   spi_write(0x32);
   spi_write(0x0A);
   output_high(RF_CS);

   //RX_PW_P2
   //N� de bytes en Pipe2.
   //10 byte (Direccion emisor y trama).
   output_low(RF_CS);
   spi_write(0x33);
   spi_write(0x0A);
   output_high(RF_CS);
}
//*****************************************************








//*****************************************************
//*                    RF_ON()                         *
//*****************************************************
//*Descripci�n:Esta rutina activa el m�dulo de        *
//*radiofrecuencia en modo escucha para poder recibir *
//*datos enviados a su direcci�n.                     *
//*****************************************************
//*Variables de entrada:                              *
//*Variables de salida:                               *
//*****************************************************
void RF_ON()
{

   output_low(RF_CE);

   // CONFIG
   //Se activa el modulo, se pone en recepci�n,
   //se activa el CRC para que utilice 2 bytes.
   output_low(RF_CS);
   spi_write(0x20);
   spi_write(0x0F);
   output_high(RF_CS);

   delay_ms(2);
   output_high(RF_CE);
   delay_us(150);
}
//*****************************************************









//*****************************************************
//*                 RF_OFF()                         *
//*****************************************************
//*Descripci�n:Este procedimiento desactiva el m�dulo *
//*de radiofrecuencia.                                *
//*****************************************************
//*Variables de entrada:                              *
//*Variables de salida:                               *
//*****************************************************
void RF_OFF()
{
   output_low(RF_CE);

   // CONFIG
   //Se desactiva el modulo
   output_low(RF_CS);
   spi_write(0x20);
   spi_write(0x0C);
   output_high(RF_CS);
}
//*****************************************************








//*****************************************************
//*                 RF_SEND()                         *
//*****************************************************
//*Descripci�n:Esta funci�n env�a 8 Bytes de datos a  *
//*la direcci�n indicada informando de la correcta    *
//*recepci�n en el destinatario.                      *
//*****************************************************
//*Variables de entrada:- RF_DATA[]                   *
//*                     - RF_DIR
//*Variables de salida: -                             *
//*Salida:              - 0: Env�o correcto (ACK OK)  *
//*                     - 1: No recepcibido (NO ACK)  *
//*                     - 2: No enviado               *
//*****************************************************
int RF_SEND()
{
   int i;
   int estado;


   if(bit_test(INTCON,7))
      interRF=1;
   else
      interRF=0;

   disable_interrupts(GLOBAL);

   // INICIO
   output_low(RF_CE);

   //STATUS
   //Reseteo del registro STATUS
   output_low(RF_CS);
   spi_write(0x27);
   spi_write(0x70);
   output_high(RF_CS);

   // EN_RXADDR
   //Se habilita el Pipe0 para la recepci�n del ACK
   output_low(RF_CS);
   spi_write(0x22);
   spi_write(0x01);
   output_high(RF_CS);

   // TX_ADDR
   //Se configura la direcci�n de transmisi�n=RF_DIR
   output_low(RF_CS);
   spi_write(0x30);
   spi_write(RF_DIR);
   spi_write(0xC2);
   spi_write(0xC2);
   spi_write(0xC2);
   spi_write(0xC2);
   output_high(RF_CS);

   // RX_ADDR_P0
   //Para la recepci�n del ACK se debe configurar el Pipe0 con
   //la misma direcci�n a trasmitir.
   output_low(RF_CS);
   spi_write(0x2A);
   spi_write(RF_DIR);
   spi_write(0xC2);
   spi_write(0xC2);
   spi_write(0xC2);
   spi_write(0xC2);
   output_high(RF_CS);

   // RX_ADDR_P1
   //Se mete en RF_DIR la direccion propia.
   //De esta manera el receptor sabe la direcci�n
   //del transmisor.
   output_low(RF_CS);
   spi_write(0x0B);
   RF_DIR=spi_read(0);
   spi_read(0);
   spi_read(0);
   spi_read(0);
   spi_read(0);
   output_high(RF_CS);

   // W_TX_PAYLOAD
   //Se manda los datos al transductor
   output_low(RF_CS);
   spi_write(0xA0);

   DATA_N_SND++;
   spi_write(DATA_N_SND);
   spi_write(RF_DIR);
   for (i=0;i<8;i++)
      spi_write(RF_DATA[i]);

   output_high(RF_CS);

   // CONFIG
   //Se pasa a modo transmisi�n.
   output_low(RF_CS);
   spi_write(0x20);
   spi_write(0x0E);
   output_high(RF_CS);

   // Pulso de comienzo de env�o
   output_high(RF_CE);
   delay_us(15);
   output_low(RF_CE);

   noRF=0;

   while (input(RF_IRQ)==1) {
      noRF++;
      //Si no da respuesta en 7ms, no se ha enviado.
      if(noRF==500){
      break;
         }
   }


   // STATUS
   //Lectura del estado en el registro estatus.
   output_low(RF_CS);
   estado=spi_read(0x27);
   spi_write(0x70);
   output_high(RF_CS);


   // EN_RXADDR
   //Habilitar los Pipe0,Pipe1 y Pipe2.
   output_low(RF_CS);
   spi_write(0x22);
   spi_write(0x07);
   output_high(RF_CS);

      // TX_FLUSH
   //Limpieza de la FIFO de salida
   output_low(RF_CS);
   spi_write(0xE1);
   output_high(RF_CS);

   // CONFIG
   //Paso a modo recepci�n
   output_low(RF_CS);
   spi_write(0x20);
   spi_write(0x0F);
   output_high(RF_CS);

   // FIN
   output_high(RF_CE);

   delay_us(150);

   //Si no da respuesta en 7ms, no se ha enviado.
   if(noRF==500){
        if(interRF==1)
        enable_interrupts(GLOBAL);
        clear_interrupt(int_ext);
        return(2);
   }

   //estado
   //Chequeo de los bit del registro STATUS que indican si se ha recibido
   //ACK y si se ha terminado las retrasmisiones sin ningun ACK.
   if ((bit_test(estado,4)==0) && (bit_test(estado,5)==1)){
      if(interRF==1)
      enable_interrupts(GLOBAL);
      clear_interrupt(int_ext);
      return(0);
      }
   else{
      if(interRF==1)
      enable_interrupts(GLOBAL);
      clear_interrupt(int_ext);
      return(1);
      }
}
//*****************************************************









//*****************************************************
//*                 RF_RECEIVE()                      *
//*****************************************************
//*Descripci�n: Esta rutina se encarga de comprobar si*
//*se ha producido una recepci�n y de ser as�,        *
//*devuelve la trama recibida.                        *
//*****************************************************
//*Variables de entrada:-                             *
//*Variables de salida: - RF_DATA[]                   *
//*                     - RF_DIR                      *
//*Salida:         - 0: Recepci�n correcta y �nica    *
//*                - 1: Recepci�n correcta y m�ltiple *
//*                - 2: No se ha producido recepci�n  *
//*                - 3: No se ha producido recepci�n  *
//*****************************************************
int RF_RECEIVE()
{

   int i;
   int mas;
   int estado;

   if (input(RF_IRQ)==1 && RCVNW==0){
      return (2);
      }

   //STATUS
   //Lectura y reseteo del registro STATUS
   output_low(RF_CS);
   estado=spi_read(0x27);
   spi_write(0x70);
   output_high(RF_CS);

   //estado
   //Chequeo de la interrupci�n de recepci�n.
   if (bit_test(estado,6)==0 && RCVNW==0){
      return(3);
      }

   //R_RX_PAYLOAD
   //Lectura de los datos recibidos.
   output_low(RF_CS);
   spi_write(0x61);
   DATA_N_RCV=spi_read(0);
   RF_DIR=spi_read(0);
   for (i=0;i<8;i++)
   {
      RF_DATA[i]=spi_read(0);
   }
   output_high(RF_CS);

   //FIFO_STATUS
   //Comprobaci�n del estado de la FIFO de
   //recepci�n para comprobar si hay m�s datos
   output_low(RF_CS);
   spi_write(0x17);
   mas=spi_read(0);
   output_high(RF_CS);

   if (bit_test(mas,0)==0){
      RCVNW=1;
      return(1);
   }
      RCVNW=0;
      return(0);
}
//*****************************************************









