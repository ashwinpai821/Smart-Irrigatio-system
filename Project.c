#include<reg51.h>

/********************************************* LCD control signals declaration ***************************************************/
 
sbit RS = P0^0;     
sbit RW = P0^1;  				// Read/write line
sbit Enable = P0^2; 		// Enable line
#define LCD_PORT P2 		// define port
sbit sensor = P1^1;			//sensor(switch)
sbit motor = P1^0;			//motor(LED)
unsigned char c;
unsigned char a[32];
int num[10];
 
/********************************************* LCD function prototypes ************************************************/
void send_cmd(unsigned char);
void send_char(unsigned char);
void LCD_init(void);
void delayms(unsigned int);
void display1();
void display4();
void watering();
void number(unsigned int);
void halt();

/********************************************* Main Funciton declaration ***********************************************/
void main()
{

	int temp=0;
LCD_PORT = 0x00;  // Make the port as output port
sensor = 0x00;	  // Make the port as output port
motor = 0x0F;  		// LCD initialization
LCD_init();
display1();	      
	while(1){
if(sensor==1)     //Soil sensor sensing LOW moisture
{
	display4();     //displays that moisture is low 
	motor = 1;      //motor ON
	watering(); 	  //calling the function that displays watering on LCD and shows time of watering
}else if(sensor ==0)//Soil sensor sensing Enough moisture
{	
	halt();					// funtion that displays enough moisture and turns motor off 
}
	} 
} 
 
/********************************************* LCD Initialization Function declaration ********************************/
 
void LCD_init()
{
 send_cmd(0x38);      // configuring LCD as 2 line 5x7 matrix
 send_cmd(0x0E);      // Display on, Cursor blinking
 send_cmd(0x01);      // Clear Display Screen
 send_cmd(0x06);      // Increment Cursor (Right side)
}
 
void send_char(unsigned char character)
{
 LCD_PORT = character;
 RS = 1;    // Select Data Register
 RW = 0;    // write operation
 Enable = 1;      // High to Low pulse provided on the enable pin with nearly 1ms(>450ns)
 delayms(1);   // 1 millisec delay
 Enable = 0;
 delayms(10);   // 100 millisec delay
}
 
 
/*********************************************LCD Command Sending Function declaration********************************/
 
void send_cmd(unsigned char Command)
{
 LCD_PORT = Command;
 RS = 0;      // Select Command Register
 RW = 0;    // write operation
 Enable = 1;      // High to Low pulse provided on the enable pin with nearly 1ms(>450ns)
 delayms(1);   // 1 millisec delay
 Enable = 0;
}
 

 
/******************************************* delayms Function declaration***********************************************/
void delayms(unsigned int val)
{
 unsigned int i,j;
   
 for(i=0;i<=val;i++)
 {
  for(j=0;j<=2;j++)  ;
  //_nop_();   // no operation produce 1us time delay
 }
  
}
void display1()
{
	unsigned char a[14]="   Start!";
	int l =0;
	LCD_init();
	while (a[l]!='\0')
{
	send_char(a[l]);
	l++;
	delayms(15);
}
}
void watering()
{
	int t=0;
	int i;
	unsigned char a[14]="Watering!";
	int l =0;
	LCD_init();
	while (a[l]!='\0')
{
	send_char(a[l]);
	l++;
	delayms(15);
}
	do
	{
		t++;
		for(i=0;i<429;i++);//1 second
	}while(sensor==1);
	//for(i =0;i<t;i++){display1();}
	number(t);
	delayms(15);
}
void number(unsigned int i)
{
	int p;
	
int k=0;
while(i>0)
{
  num[k]=i%10;
  i=i/10;
  k++;
}
k--;
LCD_init();
send_char('t');
send_char('i');
send_char('m');
send_char('e');
send_char(':');


for (p=k;p>=0;p--)
{
	c = num[p]+48;
  send_char(c);
}
send_char(' ');
send_char('s');
send_char('e');
send_char('c');
}
void halt()
{
	unsigned char a[32]="Enough moisture!   Motor OFF!";
	int l =0;
	LCD_init();
	while (a[l]!='\0')
{
	send_char(a[l]);
	l++;
	delayms(15);
}
motor = 0;
while(sensor==0);
}
void display4()
{
	unsigned char a[32]="Moisture Low !     Motor ON!";
	int l =0;
	LCD_init();
	while (a[l]!='\0')
{
	send_char(a[l]);
	l++;
	delayms(15);
}
}
