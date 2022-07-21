#include <AT89X52.H>
#include "delay/delay.h"


#define LED2  				P2_1
#define LED3  				P2_2
#define LED4  				P2_3

#define LED_PORT			P0

#define ADC0804_PORT		P1
#define ADC0804_WR			P2_6
#define ADC0804_RD			P2_5
#define ADC0804_INTR		P2_7

#define BUZZER              P2_4
#define BUTTON              P2_0

unsigned char led7_data[10] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90 }; //Decimal to Led7seg
unsigned char dv,chuc,tram,nghin;
unsigned char adc_value;
unsigned char huy = 0;						    //Bien luu tru nhiet do giam xuong nguong bao dong
unsigned int count = 40;
unsigned int i=0;								// Bien dem Timer

/*Ham doc du lieu tu adc0804*/
unsigned char read_adc0804(void);
/* Chuong trinh phuc vu ngat ngoai */
void ISR_EX1(void) interrupt 2
{
   count++;
   tram  = (count/100);
   chuc  = (count%100)/10;
   dv    =	 count%10;	
   LED2 = 1;
   LED_PORT = led7_data[tram];
   delay_us(250);
   LED2 = 0; 
   LED3 = 1;
   LED_PORT = led7_data[chuc];
   delay_us(250);
   LED3 = 0; 
   LED4 = 1;
   LED_PORT = led7_data[dv];
   delay_us(250);
   LED4 = 0; 
   delay_ms(100);
   if(count==150) {count=1;}					// Moi khi dua nguong canh bao den 150 do thi tu Reset ve nguong 1 do
} 	  
void ISR_Timer(void) interrupt 3
{
  i++;
  if (i==140)
     {
	  adc_value = read_adc0804();
	  tram  = (adc_value/100);
      chuc  = (adc_value%100)/10;
	  dv    =	 adc_value%10;
	  i=0;
	 }
  // Nap lai gia tri cho bo dem
  TF1=0;
  TR1=0;
  TMOD = 0x10;   
  TH1=TL1=0x00;   
  TR1=1;          
}	
//------------------------------------------------
void main()
{
	BUZZER =0;
	// Cau hinh su dung ngat
	EX1 = 1;     // Cho phep ngat ngoai tai INT1
	EA = 1;     // Cho phep ngat toan cuc
	IT1 = 1;    // Che do ngat kich phat suon xuong
	// Cau hinh su dung Timer
	TMOD = 0x10;   // Che do Timer 1 16 bit  (Mode1)
	TH1=TL1=0x00;   // Nap gia tri bat dau 16 bit
	TR1=1;          // Khoi dong Timer 1
	ET1=1;         // Chon ngat Timer 1
    EA=1;          // Cho phep ngat
	// Doc gia tri nhiet do luc vua khoi dong
	adc_value = read_adc0804();
	tram  = (adc_value/100);
	chuc  = (adc_value%100)/10;
	dv    =	 adc_value%10;								 
while(1)
	{
	// Hien thi Led
	if(adc_value>=count)
		   {
		    BUZZER =1;
			LED2 = 1;
			LED_PORT = led7_data[tram];
			delay_us(250);
			LED2 = 0; 
			LED3 = 1;
			LED_PORT = led7_data[chuc];
			delay_us(250);
			LED3 = 0; 
			LED4 = 1;
			LED_PORT = led7_data[dv];
			delay_us(250);
			LED4 = 0; 
			delay_ms(100);
			huy =1;
		   }
	if(adc_value<count && huy==0)
		    {
			BUZZER = 0;
			LED2 = 1;
			LED_PORT = led7_data[tram];
			delay_us(250);
			LED2 = 0; 
			LED3 = 1;
			LED_PORT = led7_data[chuc];
			delay_us(250);
			LED3 = 0; 
			LED4 = 1;
			LED_PORT = led7_data[dv];
			delay_us(250);
			LED4 = 0; 
			}
	 if(adc_value<count && huy==1 && BUTTON==1)
		    {
			BUZZER = 1;
			LED2 = 1;
			LED_PORT = led7_data[tram];
			delay_us(250);
			LED2 = 0; 
			LED3 = 1;
			LED_PORT = led7_data[chuc];
			delay_us(250);
			LED3 = 0; 
			LED4 = 1;
			LED_PORT = led7_data[dv];
			delay_us(250);
			LED4 = 0; 
			delay_ms(100);
			} 
    if(adc_value<count && huy==1 && BUTTON==0)
	        {
			BUZZER = 0;
			LED2 = 1;
			LED_PORT = led7_data[tram];
			delay_us(250);
			LED2 = 0; 
			LED3 = 1;
			LED_PORT = led7_data[chuc];
			delay_us(250);
			LED3 = 0; 
			LED4 = 1;
			LED_PORT = led7_data[dv];
			delay_us(250);
			LED4 = 0; 
			huy = 0;
			}	    		    
    }  		  
}



unsigned char read_adc0804(void)
{
	unsigned char temp_value;
	// INTR,RD ban dau =1, WR ban dau =0
	ADC0804_WR = 0;
	ADC0804_WR = 1;
	while(ADC0804_INTR);
	ADC0804_RD = 0;
	delay_us(250);
	temp_value = ADC0804_PORT;
	ADC0804_RD = 1;
	ADC0804_INTR = 1;	
	
	return temp_value;
}