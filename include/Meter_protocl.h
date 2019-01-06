#ifndef __METER_PROTOCL_H__
#define __METER_PROTOCL_H__

//表的结构不一定是结构体但是要保证位数一致，先发送id（32位）再发送状态 8位
//电表的状态
#define METER_ON    'a'
#define METER_OFF   'b'
#define METER_ERROR  'z' //0B10101010

typedef struct meter_message
{
    int id;         //id号 32bit
    unsigned char status;    //状态 8bit 长度
}Meter_Message;

#endif
