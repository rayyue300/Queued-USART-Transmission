# Queued-USART-Transmission
Queued USART Transmission using Interrupt for ATmega328p.<br>
This project uses UDRE interrupt for serial transmission.
For the principle, read the bottom.

# Content in this Project
* README.md
* code.c:
  * two parts of code for copy & paste
* example.c:
  * example code

# How to use
### 1. Copy and Paste at the Beginning of Code
```c
#define QUEUEMAX 100
char queue[QUEUEMAX];
int nextIndex = 0;

void addToQueue(char);
char getFromQueue();
void sendMsg();
void sendChar(char);
```
### 2. Copy and Paste at the End of Code
```c
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
```
### 3. In the main program, initialize USART transmission
Set baud rate, enable transmitter, set character size, etc.<br>
You don't have to set UDRIE0 yourself because it is enabled when addToQueue() is executed.
### 4. Send!
To send a character:
```c
sendChar('a');
```
To send a string:
```c
sendString("abc");
```
# Principle
When we use polling method, the program waits until UDR0 is empty and sends a character then. The processor cannot do other things while waiting. The use of interrupt reduce the occupation of processor.<br>
UDRE interrupt is used. The interrupt is triggered once UDR0 is empty. When UDR0 is empty, a character can be sent. Therefore, we can make use of this interrupt to send characters one by one in every execution of the interrupt.<br>
Sometimes lots of characters are going to be sent. However, the processor can send a single character at the same time only. A queue is required to make sure all characters can be sent successfully.<br>
First-In-First-Out(FIFO) queue is used in this project. When some characters are going to be sent, they are put at the end of the queue. When UDR0 is empty for transmission, the first character of the queue is sent.<br>
Unless you have a paragraph of characters to send at the same time, this program should be able to transmit strings or chars and minimize the effect on the main program flow.
