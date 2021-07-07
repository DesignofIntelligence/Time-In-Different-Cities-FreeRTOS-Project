# Time-In-Different-Cities-FreeRTOS-Project

![image](https://user-images.githubusercontent.com/49645682/124762757-8bfef480-df33-11eb-92d5-ee787f2431c6.png)

Project description:
This is a “multitasking clock” project that displays the current time in different 
cities by allowing the user to choose one city of 10 choices of cities that are 
displayed on a menu on PC screen, and the choice made is displayed on the 
LCD as: chosen city name on the first line along with its current time on the 
second line. Initially the LCD displays London and its current time as default until 
user selects a different city to be displayed.

Files Description with functions:
Main.C: 
Global variables: 
cities: a 2D character array to store cities names.
timediff: an array to store time difference of each city.
currentcity: an integer that represents the currently selected city inputted by 
the user
Message: a structure that contains 3 unsigned char variables, hours, 
minutes and seconds.

Main function(): creates 2 queues, queue 1 which will be used by task 1 to 
send messages to task 2, and queue 2 which will be used by task 3 to send 
messages to task1. After creating the queues, we create 3 tasks and start 
the scheduler.void Task1(void *pvParameters): responsible for incrementing the time, we 
first create Tim, a variable of type Message, and then we receive from task 3 
the time initially. Then in the for loop, we delay the function for 1 second and 
then we increment the time, we add 1 second to the seconds and check if 
the seconds exceeded 60, we then make it equal 0 and increment the 
minutes, and so on.

void Task2(void *pvParameters): responsible for printing on the LCD, 
initially we create time, initialize LCD and clear its display, then in the for loop 
we receive the time from task 1 after getting incremented, we display it on 
the lcd, we then write the city name in the first column. After that we add the 
time difference using the time diff array (and check if the hours exceed 24 
then we subtract 24), we then print the hours, a colon, the minutes, a colon, 
the seconds and of course we check if there is a zero in the leftmost digit, if 
there is not, we add a zero.

void Task3(void *pvParameters): Responsible for interacting with the user, 
we first create time, initialize the UART and a character array time which will 
carry the time inputted. we then read the time from the user by using the 
function read_time(), after that we put this time inside Tim structure, and 
send it to queue2. In the for loop we let the user select a city by calling the 
function read_selection(), where an integer is inputted.

void ReadTime(char time[]): We create an unsigned character, which is 
sent by the uart, if the input is enter we break from the while loop, after that 
this input is moved to an array and the index is incremented.int ReadSelection(): We create an unsigned character, which is sent by the 
UART, if the input is enter we break from the while loop, the input is then 
changed into a number and returned from the function.

lcd_drivers.h: the file contains the functions prototypes Implemented in the 
lcd_driver.c file

UART.h: the file contains the functions prototypes Implemented in the 
UART.c file
UART.c: the file contains the functions implementations used for UART

void PrintString(char *out_message): This function prints a string on 
terminal of putty using UART.

lcd_driver.c: the file contains the functions implementations used for lcd.

void LCD_init(); This function is used to initialize the LCD by outputting the 
pins and using the D4 to D7 only as we will use only 4-bits.
void LCD_Clear(void): This function clears the lcd using 
LCD_Command(0x01).
void LCD_writeCharWithPos(unsigned char d, char x, char y):This 
function prints a given character with selected position on the 16x2 lcd.
void LCD_PrintLn(char i, char *s): This function prints a given string on a 
requested line
