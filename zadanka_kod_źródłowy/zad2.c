#include <p24fj128ga010.h>

_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_OFF & COE_OFF & FWDTEN_OFF)
_CONFIG2(FCKSM_CSDCMD & OSCIOFNC_ON & POSCMOD_HS & FNOSC_PRI)

#define SCALE 308L //1mili sec

/* numer którym dostajemy siê do czujnika temperatury */

#define TSENS 4
#define AINPUTS 0xffcf
void ADCinit(int amask)

{
	AD1PCFG = amask;
	AD1CON1 = 0x00e0;
	AD1CSSL = 0;
	AD1CON2 = 0;
	AD1CON3 = 0x1f02;
	AD1CON1bits.ADON = 1;
}

int readADC(int ch)

{
	AD1CHS = ch;
	AD1CON1bits.SAMP = 1;
	while (!AD1CON1bits.DONE);
	return ADC1BUF0;
}


int main(void)
{
	TRISA = 0xFF00;
	TRISD = 0xFFFF;

	unsigned long i;
	unsigned long nastawa=200;
	unsigned char display = 0;
	int temp;
	int licznik =0;
	unsigned int szybkosc = 10L;
	unsigned int pom =0;

	PORTA = 0x0000;
	ADCinit(AINPUTS); /*inicjalizacja konwertera AD*/
	
	while (1) {
		Nop();

		for (i = szybkosc * SCALE; i > 0; i--) Nop();
		temp = readADC(TSENS);
		if(temp>nastawa && pom==0){
			PORTA=1;
			for (i = 300L * SCALE; i > 0; i--) Nop();
			PORTA=0;
			licznik=licznik+1;
			if(licznik==10){
				pom=1;
			}
			else{
				pom=0;
			}
		}
		else if (pom==1){
				PORTA=255;
				if(temp<nastawa ){	
					PORTA=0;
					licznik=0;
					pom=0;
				}	
				else{
					PORTA=255;
				}

		}
		if(PORTDbits.RD6 == 0){
			PORTA=0;
			licznik=0;
			pom=0;
		}	
	
	}
}