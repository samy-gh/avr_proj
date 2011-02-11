#include <avr/io.h>



int main(void)
{
	DDRB = 0xff;
//	PORTB = 0xff;
	PORTB = 0x00;

//	PORTB |= (1<<PB2);		//PORTB‚Ì2”Ôƒsƒ“‚ª1‚É‚È‚é
//	PORTB &= ~(1<<PB2);		//PORTB‚Ì2”Ôƒsƒ“‚ª0‚É‚È‚é

	while(1){;}

	return 0;
}
