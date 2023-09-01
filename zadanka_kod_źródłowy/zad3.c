// CONFIG1H
#pragma config OSC = HSPLL      // Oscillator Selection bits (HS oscillator, PLL enabled (Clock Frequency = 4 x FOSC1))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = OFF     // Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-003FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (004000-007FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (008000-00BFFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (00C000-00FFFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (008000-00BFFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (00C000-00FFFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF

#include <xc.h>

#define LENA  PORTEbits.RE1
#define LDAT  PORTEbits.RE2
#define LPORT PORTD

#define L_ON	0x0F
#define L_OFF	0x08
#define L_CLR	0x01
#define L_L1	0x80
#define L_L2	0xC0
#define L_CR	0x0F		
#define L_NCR	0x0C	

#define L_CFG   0x38

void delay(unsigned int ms)
{
    unsigned int i;
    unsigned char j;
    
 for (i =0; i< ms; i++)
 {
 
  for (j =0 ; j < 200; j++)
   {
      Nop();
      Nop();
      Nop();
      Nop();
      Nop();
   }
 }
}

unsigned int adc(unsigned char kanal)
{
    switch(kanal)
    {
        case 0: ADCON0=0x01; break; //P1
        case 1: ADCON0=0x05; break; //P2
        case 2: ADCON0=0x09; break; 
    }
    
    ADCON0bits.GO=1;
    while(ADCON0bits.GO == 1);

   return ((((unsigned int)ADRESH)<<2)|(ADRESL>>6));
}

void lcd_wr(unsigned char val)
{
  LPORT=val;
}

void lcd_cmd(unsigned char val)
{
	LENA=1;
        lcd_wr(val);
        LDAT=0;
        //delay(3);
        LENA=0;
        //delay(3);
	LENA=1;
}
 
void lcd_dat(unsigned char val)
{
	LENA=1;
        lcd_wr(val);
        LDAT=1;
        //delay(3);
        LENA=0;
        //delay(3);
	LENA=1;
}

void lcd_init(void)
{
	LENA=0;
	LDAT=0;
	delay(20);
	LENA=1;
	
	lcd_cmd(L_CFG);
	delay(5);
	lcd_cmd(L_CFG);
        delay(1);
	lcd_cmd(L_CFG); //configura
	lcd_cmd(L_OFF);
	lcd_cmd(L_ON); //liga
	lcd_cmd(L_CLR); //limpa
	lcd_cmd(L_CFG); //configura
        lcd_cmd(L_L1);
}

void lcd_str(const char* str)
{
 unsigned char i=0;
  
 while(str[i] != 0 )
 {
   lcd_dat(str[i]);
   i++;
 }  
}
void CreateCustom(unsigned char *Pattern,const char Location)
{
    lcd_cmd(0x40+(Location*8));
    for(int i=0;i<8;i++)
        lcd_dat(Pattern[i]);    
}

void main(void) {
    
    //Inicjalizacja konwertera analogowo cyfrowego
    ADCON0=0x01;
    ADCON1=0x0B;
    ADCON2=0x01;
    
    TRISA=0xC3;
    TRISB=0x3F;   
    TRISC=0x01;
    TRISD=0x00;
    TRISE=0x00;
    
    lcd_init(); //Inicjalizacja wy?wietlacza
    lcd_cmd(L_CLR); //Czyszczenie wy?wietlacza
    
    const char customChar1[8] = {
        0x06,
        0x0F,
        0x09,
        0x00,
        0x09,
        0x07,
        0x00,
        0x00
    };
    const char customChar2[8] = {
        0x00,
        0x00,
        0x10,
        0x10,
        0x18,
        0x1C,
        0x1E,
        0x0F
    };
    const char customChar3[8] = {
        0x00,
        0x00,
        0x00,
        0x01,
        0x07,
        0x0E,
        0x1E,
        0x1C
    };
    const char customChar4[8] = {
        0x04,
        0x0C,
        0x18,
        0x10,
        0x00,
        0x00,
        0x00,
        0x00
    };
    const char customChar5[8] = {
        0x00,
        0x00,
        0x00,
        0x00,
        0x01,
        0x03,
        0x01,
        0x00
    };
    const char customChar6[8] = {
        0x03,
        0x07,
        0x0F,
        0x1E,
        0x1C,
        0x18,
        0x10,
        0x00
    };
    const char customChar7[8] = {
        0x10,
        0x18,
        0x1C,
        0x1E,
        0x0F,
        0x07,
        0x03,
        0x00
    };
    const char customChar8[8] = {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00
    };
    CreateCustom( customChar1, 1);
    CreateCustom( customChar2, 2);
    CreateCustom( customChar3, 3);
    CreateCustom( customChar4, 4);
    CreateCustom( customChar5, 5);
    CreateCustom( customChar6, 6);
    CreateCustom( customChar7, 7);
    CreateCustom( customChar8, 8);
    while(1)
    {
       delay(1000);
       lcd_cmd(L_CLR);
       lcd_cmd(L_L1); //Ustawienie karetki w pierwszej linii
       lcd_str("Wkurzasz sie ze"); //napis
       lcd_cmd(L_L2); //Przej??ie do drugiej linii
       lcd_str("cos nie dziala");
       
       delay(1000);
       lcd_cmd(L_CLR);
       lcd_cmd(L_L1); //Ustawienie karetki w pierwszej linii
       lcd_str("Przyjdz do nas"); //napis
       lcd_cmd(L_L2); //Przej??ie do drugiej linii
       lcd_str("naprawimy to !");
       
       delay(1000);
       lcd_cmd(L_CLR);
       lcd_cmd(L_L1); //Ustawienie karetki w pierwszej linii
       lcd_dat(1);
       lcd_dat(2);
       lcd_dat(3);
       lcd_dat(4);
       lcd_dat(1);
       lcd_dat(2);
       lcd_dat(3);
       lcd_dat(4);
       lcd_dat(1);
       lcd_dat(2);
       lcd_dat(3);
       lcd_dat(4);
       lcd_dat(1);
       lcd_dat(2);
       lcd_dat(3);
       lcd_dat(4);
       lcd_cmd(L_L2); //Przej??ie do drugiej linii
       lcd_dat(5);
       lcd_dat(6);
       lcd_dat(7);
       lcd_dat(8);
       lcd_dat(5);
       lcd_dat(6);
       lcd_dat(7);
       lcd_dat(8);
       lcd_dat(5);
       lcd_dat(6);
       lcd_dat(7);
       lcd_dat(8);
       lcd_dat(5);
       lcd_dat(6);
       lcd_dat(7);
       lcd_dat(8);
       
        
    }
    
    return;
}