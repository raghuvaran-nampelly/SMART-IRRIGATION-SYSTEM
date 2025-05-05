#include "stm32f401rbt6.h"
#include "systick_delay.h"
#include "4x4_keypad.h"
#include <string.h>
#include "km_lcd.h"
#include "ALL_SensorRead.h"


char entered_pass[7]; // Store the entered password (4 digits + '\0')
char correct_pass[] = "123456"; // Fixed password
int moisture_flag=0;
char key;
void EXTI9_5_IRQHandler(void) 
{
	if (EXTI_PR & (0x01 << 8))  // Check if PA6 triggered interrupt
    {
						//		KM_mdelay(250);

        moisture_flag = !moisture_flag;  // Toggle flag when moisture changes
        EXTI_PR |= (0x01 << 8);  // Clear pending interrupt flag
    }        
}







void Automatic_soilMoisture(void)
{
	//char key;
	KM_LCD_Write_Cmd(0x01);
	KM_LCD_Write_Str("Automatic mode enabled");
	KM_Delay_ms(1000);
	KM_LCD_Write_Cmd(0x01);


	while(1)
	{

	
	
		if (moisture_flag)
        {
       //     GPIOB_ODR |=(0X01 << 13);  // Turn ON LED (PB13) / Motor ON
						GPIOB_MODER &=~(0X01<<18);
						KM_Delay_ms(50);

        }
        else
        {
           // GPIOB_ODR &= ~(0X01 << 13);   // Turn OFF LED (PB13) / Motor OFF
						GPIOB_MODER |=(0X01<<18);
						KM_Delay_ms(50);

        }
									KM_Delay_ms(50);
						KM_LCD_Write_Str("Water_Lev:");

				All_SensorRead();
				
				/***************************************BREAK THE LOOP******************************************/
				 // Exit condition If key 3 is pressed break loop
									key = keys_scan();
									if (key == '3')
										{
										KM_LCD_Write_Cmd(0x01); // Clear display
										KM_LCD_Write_Str("Exiting Auto Mode");
										KM_Delay_ms(2000);
										GPIOB_MODER &=~(0X01<<18);
										break;
										}
				
			}		/************************************************************************************************/
}	







void Ckeck_posword_on(void)
{
	int i;

	char key;
		KM_LCD_Write_Cmd(0x01); 

	KM_LCD_Write_Str("Enter possword:");
	KM_LCD_Write_Cmd(0xC0); // Move cursor to second line
for (i = 0; i < 6; )
{
    key = keys_scan();  // Read keypad input
    if (key != 0)
    {
        entered_pass[i] = key;  // Store digit
   //   KM_LCD_Write_Data('*'); // Display '*' instead of actual key
			KM_LCD_Write_Data(key); // Display digit on LCD
        KM_Delay_ms(200);  // Debounce
        i++;
    }
}
entered_pass[6] = '\0';  // Null terminate the string

// Prompt user to press #
KM_LCD_Write_Cmd(0xC7);
KM_LCD_Write_Str("ENTER #");



while (1)  // Wait for '#' key press
{
    key = keys_scan();
    if (key == '#') break;
}

// Check password
KM_LCD_Write_Cmd(0x80);
if (strcmp(entered_pass, correct_pass) == 0)  // Compare entered password with "1234"
{
    KM_LCD_Write_Str("ACCESS GRANTED");
	KM_Delay_ms(1000);
    KM_LCD_Write_Cmd(0x01);
    KM_LCD_Write_Str("MOTOR ON  3.Exit");

	 
			GPIOB_MODER |=0X01<<18;

		    KM_Delay_ms(2000);
	while (1)  // Super loop: Motor stays ON until '3' is pressed
        {
            key = keys_scan();
            if (key == '3') // Exit Manual Mode
            {
                KM_LCD_Write_Cmd(0x01);
                KM_LCD_Write_Str("Exiting...");
                KM_Delay_ms(1000);

                // Turn OFF the motor before exiting
                GPIOB_MODER &= ~(0X01 << 18);
                KM_LCD_Write_Cmd(0x01);
                KM_LCD_Write_Str("MOTOR OFF");
                KM_Delay_ms(2000);
                return;  // Return to Config Menu
            }
        }
    }

	
else
	{
    KM_LCD_Write_Str("WRONG PASSWORD");
    KM_Delay_ms(1000);
    KM_LCD_Write_Cmd(0x01);  // Clear screen
		GPIOB_MODER &=~(0X01<<18);
		KM_LCD_Write_Cmd(0x01);
		KM_LCD_Write_Str("MOTOR OFF");
		KM_Delay_ms(2000);
	}


}






