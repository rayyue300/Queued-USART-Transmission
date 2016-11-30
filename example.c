#include <avr/io.h>
#include <avr/interrupt.h>

#define QUEUEMAX 100
char queue[QUEUEMAX];
int nextIndex = 0;

void addToQueue(char);
char getFromQueue();
void sendMsg();
void sendChar(char);

int main()
{
	UBRR0 = 103;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
	
	sei();
	while(1);
}

ISR(USART_RX_vect)
{
	char dummy = UDR0;
	sendMsg("Hello ");
	sendMsg("World!");
	sendMsg("\n\r");
	for (int i=0; i<26; i++)
	{
		sendChar(i+'a');
	}
	sendMsg("\r\n");
}

ISR(USART_UDRE_vect)
{
	if (nextIndex != 0)
		UDR0 = getFromQueue();
	else
		UCSR0B &= ~(1<<UDRIE0);
}

void addToQueue(char input)
{
	if (nextIndex < QUEUEMAX)
	{
		queue[nextIndex] = input;
		nextIndex++;
		UCSR0B |= (1<<UDRIE0);
	}
	else
	{
		//ERROR: Overflow
	}
}

char getFromQueue()
{
	char output;
	if (nextIndex != 0)
	{
		output = queue[0];
		for (int i = 1; i<nextIndex; i++)
		{
			queue[i-1] = queue[i];
		}
		nextIndex--;
	}
	else
	{
		//ERROR: Nothing in queue
	}
	return output;
}

void sendMsg(char *input)
{
	for (char *inputPtr = input; *inputPtr; ++inputPtr)
	{
		addToQueue(*inputPtr);
	}
}

void sendChar(char input)
{
	addToQueue(input);
}
