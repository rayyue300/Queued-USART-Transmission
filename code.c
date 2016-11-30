//JUST BEFORE YOUR MAIN PROGRAM:
#define QUEUEMAX 100
char queue[QUEUEMAX];
int nextIndex = 0;

void addToQueue(char);
char getFromQueue();
void sendMsg();
void sendChar(char);

//AT THE END OF YOUR MAIN PROGRAM
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
