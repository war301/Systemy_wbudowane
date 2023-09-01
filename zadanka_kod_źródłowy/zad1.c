#include <p24fj128ga010.h>

//Konfiguracja dla Explorer 16 z progr. icd2
_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_OFF & COE_OFF & FWDTEN_OFF)
_CONFIG2(FCKSM_CSDCMD & OSCIOFNC_ON & POSCMOD_HS & FNOSC_PRI)

#define SCALE 308L

int nthbit(int num, int n){
    int wartoscBita = (num >> n) & 1;
    return wartoscBita;
}

int prng(seed){
	int i = 0;
    int check = -1;
    while (check < 1)
	{
    	if (seed == 1)
		{
        	check +=1;
    	}
    int first = nthbit(seed,0);
    int second = nthbit(seed, 1);
    int fifth = nthbit(seed,4);
    int sixth = nthbit(seed,5);


    int xor = first ^ second ^ fifth ^ sixth;

    int seed_moved = seed >> 1;
    int xor_moved = xor << 5;

    int random_number = seed_moved | xor_moved;

    seed =  random_number;
	if (check<1){
        return random_number;
    }
    }
	return seed;
}


int main(void){
unsigned long i;
unsigned long j=0;
unsigned long x=0;
unsigned long d=0;
unsigned long tak=1;
unsigned char display=0;
unsigned int przelacznik=1;
unsigned int b=7;
unsigned int k=1;
unsigned int nie=128;
unsigned int licznik=0;
unsigned int seed1=1;
//inicjalizacja
 PORTA=0x0000;
 TRISA=0xFF00;
 TRISD=0xFFFF;

 
//nieko�?cz�?ca si�? p�?tla 
again:
	Nop();
	PORTA=(unsigned int) display;
	for(i=500L*SCALE;i>0;i--) Nop();
	if (PORTDbits.RD13 == 0){
	   	przelacznik=przelacznik+1;
		display=0;
	}
	else if(PORTDbits.RD6 == 0){
	   	przelacznik=przelacznik-1;
		display=0;
	}	
	



	switch (przelacznik)
	{
	
		case 1:
		for(i=500L*SCALE;i>0;i--) Nop();
		   display=display+1;
		goto again;
		
		goto again;
		break;
		
		case 2:	
		
		for(i=500L*SCALE;i>0;i--) Nop();
		  display=display-1;
		goto again;
		
		goto again;
		break;

		case 3:
		for(i=500L*SCALE;i>0;i--) Nop();
			j=j+1;	
			display=j^(j>>1);
		goto again;

		goto again;
		break;
		
		case 4:
		for(i=500L*SCALE;i>0;i--) Nop();	
			x=x-1;	
			display=x^(x>>1);
		goto again;

		goto again;
		break;

		case 5:
		for(i=500L*SCALE;i>0;i--) Nop();
			d++;
			display = ((d/10)<<4)^(d%10);
		goto again;

		goto again;
		break;
 
		case 6:
		for(i=500L*SCALE;i>0;i--) Nop();
			d--;
			display = ((d/10)<<4)^(d%10);
		goto again;

		goto again;
		break; 		
		
		case 7:
		for(i=500L*SCALE;i>0;i--) Nop();			
			if (b==224){
				tak=0;
			}
			if (b==7){
				tak=1;
			}
			if (tak==1){
				display = b;
				b=b<<1;
			}else if (tak ==0){
				display= b;
				b=b>>1;	
			}
				
		goto again;

		goto again;
		break; 		
		
		case 8:
		for(i=500L*SCALE;i>0;i--) Nop();			
			display= k+licznik;
			k=k<<1;
			if(licznik==255){
				k=1;
				licznik=0;
				nie=128;		
			}
			if(k==0) k++;
			if(k==nie){
				licznik=licznik+nie;
				nie=nie/2;
				k=0;
			}
		goto again;

		goto again;
		break; 
		
		
		case 9:
		for(i=500L*SCALE;i>0;i--) Nop();
			seed1=prng(seed1);			
			display= seed1;
		goto again;

		goto again;
		break; 
	

		default:
		for(i=500L*SCALE;i>0;i--) Nop();
		   display=display+1;
		

	}

	goto again;
		
	goto again;
}
