#include "USARTA0.h"

extern char info_to_send[10];
void  USARTA0_init() {   
	UCA0CTL1 |= UCSWRST;         
    P1SEL |= BIT1 + BIT2;         
    P1SEL2 |= BIT1 + BIT2;        
	
    UCA0CTL1 |= UCSSEL1 + UCRXEIE; 
    UCA0BR0 = 1000000 / 9600;
    UCA0BR1 = 0;
    UCA0MCTL = UCBRF_0 + UCBRS_1;
    UCA0CTL1 &= ~UCSWRST;       
}

void USARTA0_send(char *str) {
	unsigned int j = 0;
	while(str[j] != '\0') {
		while((IFG2&UCA0TXIFG) == 0);
		UCA0TXBUF = str[j++];
	}
}
