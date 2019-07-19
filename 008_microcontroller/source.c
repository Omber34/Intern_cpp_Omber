

#include <mega103.h>
#include <delay.h>
// Declare your global variables here

void main(void)
{
// Declare your local variables here

DDRA=0x00;
PORTA=0x00;

DDRB=0b00001110;  
PORTB=0b00000001;

DDRD=0x00;
PORTD=0x00;




while (1)
      {
      if (PINB.0 == 0)
      { 
            PORTB.1=1;
            delay_ms(500);
            PORTB.1=0;
            
            PORTB.2=1;
            delay_ms(500);
            PORTB.2=0;
            
            PORTB.3=1;
            delay_ms(500);
            PORTB.3=0;
      }
      
      }
}
