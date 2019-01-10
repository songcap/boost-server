#include "server_main.h"


MeterServer::MeterServer(io_service & io_svr, string bindIp,int port):io_service_(io_svr),myAccept(io_svr,tcp::endpoint(ip::address::from_string(bindIp), port))
{
    //"172.31.4.150"
     this->SqlTool = new MySQLInterface();
     this->SqlTool->connectMySQL("cd-cdb-pv7st8qo.sql.tencentcdb.com","public","2198569wmh","electric",63382);
}

MeterServer::~MeterServer()
{

}

void MeterServer::StartAccept()
{
      boost::shared_ptr< boost::asio::ip::tcp::socket > pSocket(new ip::tcp::socket(this->io_service_));

     //这里面需要一个socket的指针去接收对象 这个socket需要被保存起来
      this->myAccept.async_accept(*pSocket,boost::bind(&MeterServer::AcceptHandle,this,pSocket,_1));
}

 //连接回调函数
 void  MeterServer::AcceptHandle(boost::shared_ptr<ip::tcp::socket> pSocket,boost::system::error_code ec)
{
          if(!ec)
          {
                    if(pSocket->is_open())
                     {
                         MaskHandle * task = new MaskHandle(pSocket,this->SqlTool);
                         client_sockets.push_back(pSocket);
                           //执行操作
                          task->ReadService();
                     }
                     StartAccept(); //接着监听
          }
          else
           {

           }
}



void MeterServer::StartService(void)
{
        StartAccept();
        //this->io_service_.run();
        boost::thread_group group;
         for (int num = 0 ; num < 20 ; ++num)
         {
                    //create_thread()是一个工厂函数，可以创建thead对象并运行线程，同时加入内部的list
                    // group.create_thread(boost::bind(&runchild , num)) ;
                    group.create_thread(boost::bind(&ThreadRun)) ;
          }
         group.join_all() ; //等待所有线程执行结束
         //io_service_.run();
}

void  MeterServer::StartSend(char * buffer,size_t buff_size)
{
     this->socket_->async_send(boost::asio::buffer(buffer, buff_size),boost::bind(&MeterServer::SendHandle,this,boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred,buffer));
}

 //发送回调函数
void  MeterServer::SendHandle(const boost::system::error_code& error ,size_t bytes_transferred,char * buf)
{
      if(!error)
      {
              delete buf;
      }
      else
      {
         //异常处理
      }
}
