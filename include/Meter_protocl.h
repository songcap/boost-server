#ifndef __METER_PROTOCL_H__
#define __METER_PROTOCL_H__

//��Ľṹ��һ���ǽṹ�嵫��Ҫ��֤λ��һ�£��ȷ���id��32λ���ٷ���״̬ 8λ
//����״̬
#define METER_ON    'a'
#define METER_OFF   'b'
#define METER_ERROR  'z' //0B10101010

typedef struct meter_message
{
    int id;         //id�� 32bit
    unsigned char status;    //״̬ 8bit ����
}Meter_Message;

#endif
