//pir sensor project



#include "stm32f401rbt6.h"
#include "systick_delay.h"
#include "4x4_keypad.h"
#include <string.h>
#include "km_lcd.h"
#include "ALL_SensorRead.h"
#include "Modes.h"
//int result_PA0 = 0;  // Store ADC value from PA0
//int result_PC1 = 0;  // Store ADC value from PC1
//int result_PC0 = 0;  // Store ADC value from PC0
//int i;


void show_ConfigMenu(void)
{
	KM_LCD_Write_Cmd(0x01); // Clear display
    KM_LCD_Write_Str("Config Menu:");
    KM_LCD_Write_Cmd(0x80);
    KM_LCD_Write_Str("1.Manual 2.Automatic");
    KM_LCD_Write_Cmd(0xC0);
    KM_LCD_Write_Str("3.Exit");
}
	
	
	
	

	int main()
	{
		char key;
	
		Lcd_gpiob_init_config();
		KM_LCD_Init();
		keypad_init_config();
		Soil_Moisture_Config();
		GPIOB_MODER &=~(0X01<<18);
		KM_LCD_Write_Cmd(0x01); //clear display
		All_analigSensor_Init_Config();
		

		
while(1)
		{
				//	All_SensorRead();

						show_ConfigMenu();
						while(!(key=keys_scan()));
		
		switch(key)
		{
			case '1':Ckeck_posword_on();
								break;
			case '2':Automatic_soilMoisture();
								break;
			case '3':KM_LCD_Write_Cmd(0x01); // Clear screen
                KM_LCD_Write_Str("System Exiting...");
                KM_Delay_ms(1000);
								break;
			case '4':All_SensorRead();
								break;
				
								//GPIOB_MODER &=~(0X01<<18);
			return 0;
		}
	}
}
	
				
	


