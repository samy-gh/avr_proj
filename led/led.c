#include <avr/io.h>



int main(void)
{
	DDRB = 0xff;
//	PORTB = 0xff;
	PORTB = 0x00;

//	PORTB |= (1<<PB2);		//PORTB��2�ԃs����1�ɂȂ�
//	PORTB &= ~(1<<PB2);		//PORTB��2�ԃs����0�ɂȂ�

	while(1){;}

	return 0;
}
