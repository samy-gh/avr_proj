#include <avr/io.h>



int main(void)
{
	DDRB = 0xff;
//	PORTB = 0xff;
	PORTB = 0x00;

//	PORTB |= (1<<PB2);		//PORTBの2番ピンが1になる
//	PORTB &= ~(1<<PB2);		//PORTBの2番ピンが0になる

	while(1){;}

	return 0;
}
