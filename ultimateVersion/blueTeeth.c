#include "reg52.h"
#include "intrins.h"

#define uchar unsigned char 
#define uint unsigned int

/*���ڳ�ʼ������*/

void Uart0_Init(){
    RCAP2L=0xD9;//9600�����ʶ�Ӧ FFD9����λΪD9
    RCAP2H=0xFF;//��λΪFF
    T2CON=0x34;//RCLK��TCLK��TR2��1
    SCON=0x50;//���ڹ���ģʽ1������ʹ��
    ES=1;//�򿪽����ж�
    EA=1;//�����ж�

}
/*��������*/
void sendDate(char date)
{
    SBUF=date;		  //���յ������ݷ��뷢�ͻ���������
    while(!TI);       //�ȴ������������
    TI=0;			  //���������ɱ�־λ
}
 