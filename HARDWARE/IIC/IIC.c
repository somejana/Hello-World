#include "IIC.h"
#include "stm32f10x_gpio.h"
	  
 void delay_us(unsigned int time) 
{         
	unsigned int i=0;     
	while(time--)   
	{       
		i=9;  //�Լ�����      
		while(i--);        
	} 
}  


//���뼶����ʱ  
 void delay_ms(unsigned int time) 
{        
	unsigned long i=0;     
	while(time--)   
	{       
		i=10800;  //�Լ�����       
		while(i--);         
	} 
}


void SDA_IN(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);				
 GPIO_SetBits(GPIOB,GPIO_Pin_15);						 
}

void SDA_OUT(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB15
 GPIO_SetBits(GPIOB,GPIO_Pin_15);						 //PB15 �����
}
//��ʼ��IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
  //��ʹ������IO PORTCʱ�� 
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
 
	IIC_SCL=1;
	IIC_SDA=1;

}

void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
	delay_us(4);
}	  

void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	
	IIC_SCL=1; 
		delay_us(1);
	
	IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);		
}

void IIC_Send_Byte(u8 txd)
{                        
    u8 t; 
	    for(t=0;t<8;t++)
    {    	
				IIC_SCL=0;	        
        IIC_SDA=(txd&0x01);
        txd>>=1; 	  
		delay_us(5);  
		IIC_SCL=1;
		delay_us(5); 
    }	
		IIC_SCL=0;	
		delay_us(5);
		while(IIC_Wait_Ack());
		IIC_SCL=1;	
		delay_us(5);
} 	

//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;	
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(20);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(5);
	IIC_SCL=1;
	delay_us(5);
	IIC_SCL=0;
}					 				     
   
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
    IIC_SCL=0; 
    delay_us(5);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(5); 
    }					 
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}
























