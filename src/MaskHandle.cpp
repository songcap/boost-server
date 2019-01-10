
#include "MaskHandle.h"

/*
主要就是service.run可以使用线程池
然后在线程里面去响应回调函数，回调函数就直接放在线程里面好了
*/

MaskHandle::MaskHandle(boost::shared_ptr< boost::asio::ip::tcp::socket > psocket, MySQLInterface * SqlTool):socket_(psocket)
{
          if(socket_->is_open())
          {
               this->SqlTool = SqlTool;
               cout << "sock is open" << endl;
          }
}

MaskHandle::~MaskHandle()
{

}

char * MaskHandle::getBuf()
{
       return this->buf_;
}

boost::shared_ptr< boost::asio::ip::tcp::socket > MaskHandle::getSocket()
{
      return this->socket_;
}

void  MaskHandle::ReadService()
{
    //解包
    memset(this->buf_,0,100);
    this->socket_->async_read_some(boost::asio::buffer(this->buf_,sizeof(Meter_Message)),
    boost::bind(&MaskHandle::ReadHandle,this,boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred));
}


//读取回调函数
void MaskHandle::ReadHandle(const boost::system::error_code& error ,size_t bytes_transferred)
{

       if(error)
       {
                  //这里用来做一下判断，下线一共有两种，就是说第一种是读取超时，第二种就是心跳包的超时
                        cout << "read message failed, error code:" << error.value()  << " category name:" << error.category().name() << " message:" << error.message() << endl;
                        if( error.value()  == 2)
                        {
                               socket_->close();
                              delete this;
                        }
                        //return;
       }
       else
       {
              if(bytes_transferred >= sizeof(Meter_Message))
              {
                    updateMes();
                    //获取当前时间
                    MeterGetTime();
                    //记录消息到数据库
                    //MysqlInstruct(CREATE_TABLE);
                    mutex.lock();
                    if(pnow->tm_mon== 12 &&pnow->tm_mday == 31)
                    {
                          this->SqlTool->createdbTable(MysqlInstruct(CREATE_TABLE));
                    }
                    this->SqlTool->writeDataToDB(MysqlInstruct(INSERT));
                    mutex.unlock();
                    ReadService();
              }
       }

}

void MaskHandle::updateMes()
{
    memcpy(&mes,this->buf_,sizeof(Meter_Message));
    cout << mes.id << endl;
}

void  MaskHandle::MeterGetTime()
{
    time_t now_time;
    now_time  =  time(NULL);
    this->pnow =  localtime(&now_time);
    this->pnow->tm_mon += 1;
    this->pnow->tm_year += 1900;
}

string MaskHandle::MysqlInstruct(int mode)
{
    char sqlBuf[300];
    switch(mode)
    {
        case INSERT:
         sprintf(sqlBuf,"INSERT INTO `%d_meter_message`  VALUES ( %d,'%c','%d-%d-%d','%2d:%2d:%2d');" ,pnow->tm_year,mes.id, mes.status,pnow->tm_year,pnow->tm_mon,pnow->tm_mday,pnow->tm_hour,pnow->tm_min,pnow->tm_sec);
          cout << sqlBuf << endl;
          break;
         case CREATE_TABLE :
         sprintf(sqlBuf,"Create Table If Not Exists `electric`.`%d_meter_message`( `id_ele` Bigint(8) unsigned,`status` Varchar(1),`dates` Date, `hour_min` Time )Engine InnoDB", pnow->tm_year);
          break;
         default:
          break;
    }
    string str(sqlBuf);
    return sqlBuf;
}
