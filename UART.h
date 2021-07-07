////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

////////// ---------->>>>>>>>>> UART HELPING FUNCTIONS HEADER FILE <<<<<<<<<<---------- //////////


// Stellaris includes
#include "TM4C123GH6PM.h"
#include <stdint.h>


// APIs to the UART functions
void UARTInit();														// UART module initialization over PortA
void UARTTransmit( uint8_t data_message );	// UART character transmission
void PrintString(char *string);							// UART full message line transmission
char UARTRecieve();													// UART character recieving
void ReadTime(char message_buffer[]);				// Read initial time
unsigned int ReadSelection();								// Read city selection