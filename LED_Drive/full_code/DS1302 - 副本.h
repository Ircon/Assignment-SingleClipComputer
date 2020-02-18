#ifndef _HEAD_DS1302_
#define _HEAD_DS1302_
/*
sbit CLK = P2 ^ 5;
sbit IO = P2 ^ 6;
sbit RSET = P2 ^ 7;*/
// ===================
#include <lang.h>

sbit T_IO=P3^5;								//ʵʱʱ������������
sbit T_CLK=P3^6;								//ʵʱʱ��ʱ��������
sbit T_RST=P3^7;								//ʵʱʱ�Ӹ�λ������
sbit ACC0=ACC^0;							//�ӷ������λ
sbit ACC7=ACC^7;							//�ӷ������λ

void RTI_1302_1Byte(uchar d)					//��DS1302д��1Byte����(�ڲ�����)
{
	uchar i;									//ѭ������
	ACC=d;									//�������ۼ���A
	for(i=8;i>0;i--)							//ѭ��8�Σ�
	{
		T_IO=ACC0;							//��ACC0λֵ���ȵ�λ�����λ�������ݶ˿�
		T_CLK=1;							//����ʱ���ߵ�ƽд����ֵ
		T_CLK=0;							//����ʱ���ߵ�ƽֹͣд��
		ACC=ACC>>1;						//��A���������� 1 λ
	}
}

uchar RTO_1302_1Byte()						//��DS1302��ȡ1Byte����(�ڲ�����)
{											
	uchar i;									//��������
	for(i=8;i>0;i--)							//ѭ��8��
	{
		ACC=ACC>>1;						//��ACC���������� 1 λ
		ACC7=T_IO;							//��ȡһλֵ���ȵ�λ�����λ���͵�ACC7λ
		T_CLK=1;							//ֹͣ��ȡ
		T_CLK=0;							//�ͷ�����
	}
	return(ACC);								//����ACCֵ
}

void Write_1302(uchar ucAddr,uchar ucDa)			//��DS1302д�����ݣ��ȵ�ַ������
{
	T_RST=0;								//��λ����Ϊ�͵�ƽ���������ݴ�����ֹ
	T_CLK=0;								//��ʱ������
	T_RST=1;								//��λ����Ϊ�ߵ�ƽ���߼�������Ч
	RTI_1302_1Byte(ucAddr);					//дʵʱʱ�ӵ�ַ������
	RTI_1302_1Byte(ucDa);						//дʵʱʱ��д1Byte����
	T_CLK=1;								//ʱ�������أ�����������Ч
	T_RST=0;								//�߼�������ϣ��� RST
}

uchar Read_1302(uchar ucAddr)					//��ȡDS1302��ָ����ַ����
{
	uchar ucData;								//�������ݱ���
	T_RST=0;								//��λ����Ϊ�͵�ƽ���������ݴ�����ֹ
	T_CLK=0;								//����ʱ�ӵ�ƽ����ʱ������
	T_RST=1;								//��λ����Ϊ�ߵ�ƽ���߼�������Ч
	RTI_1302_1Byte(ucAddr);					//д��ָ����ַ������
	ucData=RTO_1302_1Byte();					//��ȡ1Byte����
	T_CLK=1;								//ʱ�������أ�����������Ч
	T_RST=0;								//�߼�������ϣ��� RST
	return(ucData);							//��������ֵ
}
/*
//��ʼ�����ý��޵�һ��ʹ��DS1302,֮�󼴿���������,�����δ˳����
void Set_1302(uchar *pClock)					//����DS1302��ʼʱ��
{
	uchar i;									//ѭ����������
	uchar ucAddr=0x80;						//��ַ�������壨��ֵ0x80��
	Write_1302(0x8e,0x00);						//��д���ơ�����,WP=0,����д����
	//˵����������0X8E Ϊд���ƣ�������0x8F Ϊ�����ƣ�
	//��д��0x00�����λΪWP=0�������д��	��д��0X80�����λΪWP=1����ֹ��д��	
	for(i=7;i>0;i--)							//ѭ��7�Σ�����д��
	{
		Write_1302(ucAddr,*pClock);			//�룬�֣�ʱ���գ��£����ڣ���
		pClock++;							//ÿдһ���ַ������ݵ�ַ�� 1
		ucAddr+=2;							//DS1302�ڲ��Ĵ�����ַ�� 2
	}
	Write_1302(0x8e,0x80);						//��������,WP=1,д����
}
*/
uchar BCD_Decimal(uchar bcd)					//BCD��תʮ���ƺ���������BCD��
{
	uchar Decimal;							//��������
	Decimal=bcd>>4;							//BCD������ 4 λ�������
	Decimal=Decimal*10+(bcd&=0x0F);			//ת����ʮ����
	return Decimal;							//����ʮ��ֵ
}

void Get_1302(uchar ucCurtime[])					//��ȡDS1302ʵʱ����(BCD��)
{
	uchar i;
	uchar ucAddr=0x81;						//��ʼ��ַ: ��Ĵ���
	for(i=0;i<7;i++)							//���ζ�ȡ7��ֵ
	{
		ucCurtime[i]=							//˳��: ���ʱ����������
			BCD_Decimal(Read_1302(ucAddr));	//ת����ʮ����	
		ucAddr+=2;							//ÿ��һ��ֵ��,��ַ��2
	}
}

#endif