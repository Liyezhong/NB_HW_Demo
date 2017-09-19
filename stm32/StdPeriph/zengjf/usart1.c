#include "zengjf.h"

/*
 * ��������USART1_Config
 * ����  ��USART1 GPIO ����,����ģʽ���á�115200 8-N-1
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */
void USART1_Config(int baudrate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    /* ���ô���1 ��USART1�� ʱ��*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
    /*����GPIO�˿�����*/
    /* ���ô���1 ��USART1 Tx (PA.09)��*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);    
    /* ���ô���1 USART1 Rx (PA.10)*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* ����1����ģʽ��USART1 mode������ */
    USART_InitStructure.USART_BaudRate = baudrate;    //һ������Ϊ115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure); 
    USART_Cmd(USART1, ENABLE);//ʹ�ܴ���
}

/*
 * ��������fputc
 * ����  ���ض���c�⺯��printf��USART1
 * ����  ����
 * ���  ����
 * ����  ����printf����
 */
int fputc(int ch, FILE *f)
{
    /* ��Printf���ݷ������� */
    USART_SendData(USART1, (unsigned char) ch);
    while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);  
    
    led_toggle(GPIOE, GPIO_Pin_6);
    
    return (ch);
}

/*
 * ��������fputc
 * ����  ���ض���c�⺯��scanf��USART1
 * ����  ����
 * ���  ����
 * ����  ����scanf����
 */
int fgetc(FILE *f)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
    
    led_toggle(GPIOE, GPIO_Pin_6);
    
    int ch = (int)USART_ReceiveData(USART1); 

    if (ch == '\r' || ch == '\n')
    {
        fputc('\r', f);
        fputc('\n', f);
    } else {
        fputc(ch, f);
    }
    
    return ch;
}
