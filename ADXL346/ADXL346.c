#include<iocc2540.h>

void accInit(void);
void accWriteReg(short reg, short val);
void accReadReg(short, short *);
void spiWriteByte(short write);
void spiReadByte(short *read, short write);
void multiread(short, short *);

short pval[6];

#define SCK             P1_5
#define MISO            P1_7
#define MOSI            P1_6

#define CS              P2_0

#define WAIT_1_3US(t)                   \
    do{                                 \
        for (int i = 0; i<t; i++)     \
            asm("NOP");                 \
    }while(0)

#define CS_DISABLED     1
#define CS_ENABLED      0

short test1,test2,test3;
int i,j;
int main( void )
{
  CLKCONCMD = 0x80;

  WAIT_1_3US(3000);

  accInit();

  //accReadReg(0xc0,&test1);
  accReadReg(0x80,&test1);

  accWriteReg(0x31,0x0b);
  accWriteReg(0x2d,0x08);
  accWriteReg(0x2e,0x80);
// while(1){
  accReadReg(0xb1,&test1);
  accReadReg(0xad,&test2);
  accReadReg(0xae,&test3);
 //  accReadReg(0xee,&test3);
 // }
  while(P1_3!=1);

  /*  for(i=0;i<5000;i++){
    for(j=0;j<50;j++){
  __asm("nop;");
    }
  }*/
 /* accReadReg(0xf2,pval);
  accReadReg(0xf3,pval+1);
  accReadReg(0xf4,pval+2);
  accReadReg(0xf5,pval+3);
  accReadReg(0xf6,pval+4);
  accReadReg(0xf7,pval+5);*/

  multiread(0xf2,pval); 



  return 0;
}

//brief Initialize SPI interface and adxl346 accelerometer


void accInit(void)
{
    //*** Setup USART 1 SPI at alternate location 2 ***

    // USART 1 at alternate location 2
    PERCFG |= 0x02;
    // Peripheral function on SCK, MISO and MOSI (P1_5-7)
    P1SEL |= 0xE0;
    // Configure CS (P2_0) output

    P2DIR |= 0x01;

    P0DIR |= 0x81;
    P0 |= 0x81;

    CS = CS_DISABLED;

   // WAIT_1_3US(2);
    //*** Setup the SPI interface ***
    // SPI master mode
    U1CSR = 0x00;
      // MSB first
    U1GCR|= 0xe0;
     // SCK frequency = 1 MHz
    U1GCR |= 15;
    U1BAUD = 0;


}

void accWriteReg(short reg, short val)
{
    CS = CS_ENABLED;
    WAIT_1_3US(1);
    spiWriteByte(reg);
    spiWriteByte(val);
    CS = CS_DISABLED;
}


void accReadReg(short reg, short *pVal)
{
    CS = CS_ENABLED;
   WAIT_1_3US(1);
    spiWriteByte(reg);
    spiReadByte(pVal, 0xFF);
    CS = CS_DISABLED;
}


void spiWriteByte(short write)
{
        U1CSR &= ~0x02;                 // Clear TX_BYTE
        U1DBUF = write;
        while (!(U1CSR & 0x02));        // Wait for TX_BYTE to be set
}

void spiReadByte(short *read, short write)
{
        U1CSR &= ~0x02;                 // Clear TX_BYTE
        U1DBUF = write;
        while (!(U1CSR & 0x02));        // Wait for TX_BYTE to be set
        *read = U1DBUF;
}


void multiread(short reg, short *pVal)
{
    CS = CS_ENABLED;
    WAIT_1_3US(2);
    spiWriteByte(reg);
    spiReadByte(pVal, 0xFF);
    spiReadByte(pVal+1, 0xFF);
    spiReadByte(pVal+2, 0xFF);
    spiReadByte(pVal+3, 0xFF);
    spiReadByte(pVal+4, 0xFF);
    spiReadByte(pVal+5, 0xFF);
    CS = CS_DISABLED;
}