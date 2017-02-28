#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "LCD.h"
#include "font.h"

//Define the LCD Operation function
void LCD5110_LCD_write_byte(unsigned char dat,unsigned char LCD5110_MOde);
void LCD5110_LCD_delay_ms(unsigned int t);

//Define the hardware operation function
void LCD5110_GPIO_Config(void);
void LCD5110_SCK(unsigned char temp);
void LCD5110_MO(unsigned char temp);
void LCD5110_CS(unsigned char temp);
void LCD5110_RST(unsigned char temp);
void LCD5110_DC(unsigned char temp);

void LCD5110_init()
{
	LCD5110_GPIO_Config();

	LCD5110_DC(1);//LCD_DC = 1;
	LCD5110_MO(1);//SPI_MO = 1;
	LCD5110_SCK(1);//SPI_SCK = 1;
	LCD5110_CS(1);//SPI_CS = 1;
	
	LCD5110_RST(0);//LCD_RST = 0;
	LCD5110_LCD_delay_ms(10);
	LCD5110_RST(1);//LCD_RST = 1;

	LCD5110_LCD_write_byte(0x21,0);
	LCD5110_LCD_write_byte(0xc6,0);
	LCD5110_LCD_write_byte(0x06,0);
	LCD5110_LCD_write_byte(0x13,0);
	LCD5110_LCD_write_byte(0x20,0);
	LCD5110_clear();
	LCD5110_LCD_write_byte(0x0c,0);
}

void LCD5110_LCD_write_byte(unsigned char dat,unsigned char mode)
{
	unsigned char i;

	LCD5110_CS(0);//SPI_CS = 0;

	if (0 == mode)
		LCD5110_DC(0);//LCD_DC = 0;
	else
		LCD5110_DC(1);//LCD_DC = 1;

	for(i=0;i<8;i++)
	{
		LCD5110_MO(dat & 0x80);//SPI_MO = dat & 0x80;
		dat = dat<<1;
		LCD5110_SCK(0);//SPI_SCK = 0;
		LCD5110_SCK(1);//SPI_SCK = 1;
	}

	LCD5110_CS(1);//SPI_CS = 1;

}

void LCD5110_write_char(unsigned char c)
{
	unsigned char line;
	unsigned char ch = 0;

	c = c - 32;

	for(line=0;line<6;line++)
	{
		ch = font6_8[c][line];
		LCD5110_LCD_write_byte(ch,1);
		
	}
}
void LCD5110_write_char_reg(unsigned char c)
{
	unsigned char line;
	unsigned char ch = 0;

	c = c - 32;

	for(line=0;line<6;line++)
	{
		ch = ~font6_8[c][line];
		LCD5110_LCD_write_byte(ch,1);
		
	}
}

void LCD5110_write_string(char *s)
{
	unsigned char ch;
  	while(*s!='\0')
	{
		ch = *s;
		LCD5110_write_char(ch);
		s++;
	}
}


void LCD5110_clear()
{
	unsigned char i,j;
	for(i=0;i<6;i++)
		for(j=0;j<84;j++)
			LCD5110_LCD_write_byte(0,1);	
}

void LCD5110_set_XY(unsigned char X,unsigned char Y)
{
	unsigned char x;
	x = 6*X;

	LCD5110_LCD_write_byte(0x40|Y,0);
	LCD5110_LCD_write_byte(0x80|x,0);
}

void LCD5110_Write_Dec(unsigned int b)
{

	unsigned char datas[3];

	datas[0] = b/1000;
	b = b - datas[0]*1000;
	datas[1] = b/100;
	b = b - datas[1]*100;
	datas[2] = b/10;
	b = b - datas[2]*10;
	datas[3] = b;

	datas[0]+=48;
	datas[1]+=48;
	datas[2]+=48;
	datas[3]+=48;

	LCD5110_write_char(datas[0]);
	LCD5110_write_char(datas[1]);
	LCD5110_write_char(datas[2]);
	LCD5110_write_char(datas[3]);

	//a++;
}

void LCD5110_LCD_delay_ms(unsigned int nCount)
{
  HAL_Delay(nCount);
}

void LCD5110_GPIO_Config()
{

}

void LCD5110_CS(unsigned char temp)
{
	if (temp) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
}

void LCD5110_RST(unsigned char temp)
{
	if (temp) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6, GPIO_PIN_RESET);
}

void LCD5110_DC(unsigned char temp)
{
	if (temp) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
	else HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
}

void LCD5110_MO(unsigned char temp)
{
	if (temp) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET);
	else HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);
}

void LCD5110_SCK(unsigned char temp)
{
	if (temp) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET);
	else HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);
}
