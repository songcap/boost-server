#ifndef  __SERVER_MAIN_H
#define  __SERVER_MAIN_H
//当服务端连接出现异常该如何处理

#include <string.h>
#include <iostream>
#include <boost/asio.hpp>
#include<boost/asio/ip/address.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio/basic_stream_socket.hpp>
#include <boost/thread/thread.hpp>

#include <vector>
#include <iostream>


#include "Meter_protocl.h"
#include "MySqlAPI.h"
#include "MaskHandle.h"

using boost::system::error_code;
using namespace boost::asio;


using namespace std;

using  boost::asio::ip::tcp;
using  boost::asio::ip::address;

class  MeterServer
{
public:
            MeterServer(io_service & io_svr,string  bindIp,int port);
              ~MeterServer();
             void  StartAccept();
             void  StartSend(char * buffer,size_t buff_size);

             void  StartService(void) ;
private:

          std::vector< boost::shared_ptr<ip::tcp::socket>  > client_sockets;
          char  buf_[1024];
           //进行accept的对象
           ip::tcp::acceptor  myAccept;
           //io_service类
           io_service  &  io_service_;
          //暂时接收socket对象的类
          ip::tcp::socket * socket_;

          MySQLInterface * SqlTool;
          //连接回调函数
          void  AcceptHandle(boost::shared_ptr< ip::tcp::socket > pSocket,boost::system::error_code ec);

          //发送回调函数
          void  SendHandle(const boost::system::error_code& error ,size_t bytes_transferred,char * buf);
};

//当服务端连接出现异常该如何处理


#endif
