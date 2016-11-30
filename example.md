#Example
Full code is available at example.c
##Output
Every time when a character is received, following messages will be sent:<br>
```
Hello World!
abcdefghijklmnopqrstuvwxyz
```
##What happened?
9600 is set as baud rate
```c
UBRR0 = 103;
```
Receiver and transmitter are enabled. Receive Complete Interrupt is also enabled.
```c
UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
```
8-bit is set as character size
```c
UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
```
In the RX interrupt, a dummy variable is used to empty the UDR0 register.<br>
```c
char dummy = UDR0;
```
Multiple sendChar() and sendMsg() are used to demostrate.<br>
We can send put lots of characters in the queue and the processor now handle the transmission for us.
```c
sendMsg("Hello ");
sendMsg("World!");
sendMsg("\n\r");
for (int i=0; i<26; i++)
{
	sendChar(i+'a');
}
sendMsg("\r\n");
```
When the queue is empty, UDRE interrupt will be disabled.
```c
UCSR0B &= ~(1<<UDRIE0);
```
When the something is added to the queue, UDRE will be enabled.
```c
UCSR0B |= (1<<UDRIE0);
```
