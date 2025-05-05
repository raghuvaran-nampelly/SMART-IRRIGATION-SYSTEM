#include "stm32f401rbt6.h"
#include "km_lcd.h"
#include "Systick_Delay.h"
#include "4x4_Keypad.h"
int Pir_motion = 0;  // Store ADC value from PA0
int Water_level = 0;  // Store ADC value from PC1
int Temparature = 0;  // Store ADC value from PC0
int Per_Waterlevel=0;
int i;

	//	int i;



void Soil_Moisture_Config(void)
{
	RCC_AHB1ENR |= (0x01 << 0);	//Set 0th bit of RCC_AHB1ENR to Enable Port A clock

	RCC_APB2ENR |= (0x01 << 14);         //set 14th bit of RCC_APB2ENR to Enable SYSCFG clock
		
	GPIOA_MODER &= ~(0x03 << (8*2));     //clear 13-12 bits of GPIOA_MODER (by default it is in input mode only)
	GPIOA_PUPDR &= ~(0x03 << (8*2));     //clear 13-12 bits of GPIOA_PUPDR
	GPIOA_PUPDR |=  (0x02 << (8*2));     //load "10" into 13-12 bit fields in GPIOA_PUPDR to make PC0 as pull-down

	SYSCFG_EXTICR3 &= 0xFFF0;
	EXTI_RTSR |= (0x01 << 8);
	EXTI_FTSR |= (0x01 << 8);           //Enable Falling Edge Trigger
	EXTI_IMR  |= (0x01 << 8);
  NVIC_ISER0 |= (0x01 << 23);         //set 23rd bit in NVIC_ISTR0 to enable NVIC 23rd interrupt line
	GPIOB_ODR |=0X01<<13;
}






void All_analigSensor_Init_Config(void)
{
	/* Phase 1: Device Initialization and Configuration */
    RCC_AHB1ENR |= (0X01 << 0) | (0X01 << 2);  // ENABLE CLOCK FOR PORT A & PORT C

    // CONFIGURE PA0 AS ANALOG INPUT
    GPIOA_MODER &= ~(0X3 << 0);
    GPIOA_MODER |= (0X3 << 0); // Set PA0 as Analog Mode

    // CONFIGURE PC1 AS ANALOG INPUT
    GPIOC_MODER &= ~(0X3 << 2);
    GPIOC_MODER |= (0X3 << 2); // Set PC1 as Analog Mode

    // CONFIGURE PC0 AS ANALOG INPUT
    GPIOC_MODER &= ~(0X3 << 0);
    GPIOC_MODER |= (0X3 << 0); // Set PC0 as Analog Mode

    // ENABLE CLOCK FOR ADC1
    RCC_APB2ENR |= (0X01 << 8);

    // ADC CONFIGURATION
    ADC_SQR3 |= (0X0 << 0);  // Set 1st conversion as PA0 (Channel 0)
    ADC_SQR3 |= (0XB << 5);  // Set 2nd conversion as PC1 (Channel 11)
    ADC_SQR3 |= (0XA << 10); // Set 3rd conversion as PC0 (Channel 10)

    ADC_SQR1 |= (0X2 << 20);  // Set number of conversions (3 conversions)
    
    ADC_CR1 |= (0X1 << 8);   // Enable Scan Mode
    ADC_CR2 |= (0X1 << 0);   // Enable ADC
    ADC_CR2 |= (0X1 << 10);  // Enable EOC (End of Conversion)

	}

	void All_SensorRead(void)
{
	int i;
	char key;


        ADC_CR2 |= (0X01 << 30);  // Start Conversion

        while(!(ADC_SR & (0X01 << 1)));  // Wait for 1st conversion
        Pir_motion = ADC_DR;  // Store ADC value from PA0

        while(!(ADC_SR & (0X01 << 1)));  // Wait for 2nd conversion
        Water_level = ADC_DR;  // Store ADC value from PC1
				Per_Waterlevel = (Water_level * 100) / 700;
				if (Per_Waterlevel > 100) Per_Waterlevel= 100; // Ensure max is 100%
				
				KM_Delay_ms(500);
											//		KM_LCD_Write_Cmd(0xC0); // Clear display

        while(!(ADC_SR & (0X01 << 1)));  // Wait for 3rd conversion
        Temparature = ADC_DR;  // Store ADC value from PC0
				Temparature=(Temparature*3300)/(4095*10);
	
	 // Display readings on LCD
        KM_LCD_Write_Cmd(0x01);
     //   KM_LCD_Write_Str("PIR: "); 
			//	KM_LCD_Write_Int(Pir_motion);
        KM_LCD_Write_Str("Water_Level: "); 
				KM_LCD_Write_Int(Per_Waterlevel);
				        KM_LCD_Write_Str("%"); 

										//		KM_Delay_ms(2000);
        KM_LCD_Write_Cmd(0xC0);

        KM_LCD_Write_Str("% Temp: ");
				KM_LCD_Write_Int(Temparature);
        
														KM_Delay_ms(2000);

									KM_LCD_Write_Cmd(0x01);
key = keys_scan();
        if (key == '3')
        {
            KM_LCD_Write_Cmd(0x01);
            KM_LCD_Write_Str("Exiting Sensor Mode...");
            KM_Delay_ms(1000);
            return;
        }

//        for(i = 0; i < 1600; i++); // Add small delay
    
	}
	
	
	
	