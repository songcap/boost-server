#ifndef   __MASKHANDLE_H__
#define  __MASKHANDLE_H__

#include <string.h>
#include <ctime>
#include <iostream>
#include <boost/asio.hpp>
#include<boost/asio/ip/address.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio/basic_stream_socket.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

#include "Meter_protocl.h"
#include "MySqlAPI.h"

#define   INSERT 1
#define   CREATE_TABLE 2

using namespace std;
using namespace boost::asio;

using  boost::asio::ip::tcp;
using  boost::asio::ip::address;
using boost::system::error_code;

/*
其实在哪里进行读取都无所谓，只要保证这个buffer是安全的就可以
主要就是需要run里面一直循环，线程池里面放它就可以了，线程池可以使用boost线程池
异步的话就是触发一个指令之后如果有指令就放在队列里面执行。
readhandle函数里面记得再一次调用read函数就好。
*/
boost::mutex mutex;
class MaskHandle
{
    public:
          MaskHandle(boost::shared_ptr< boost::asio::ip::tcp::socket > psocket, MySQLInterface * SqlTool);
          ~MaskHandle();
         void  ReadService();
        char * getBuf();
        boost::shared_ptr< boost::asio::ip::tcp::socket > getSocket();
        void  ReadHandle(const boost::system::error_code& error,size_t bytes_transferred);

    private:
        char  buf_[100];
        Meter_Message mes;
        struct tm * pnow;

        MySQLInterface * SqlTool;
    private:

        boost::shared_ptr< boost::asio::ip::tcp::socket > socket_;
        //读取回调函数const boost::system::error_code& error, size_t bytes_transferred
        void updateMes();

        void  MeterGetTime();
        string MysqlInstruct(int mode);
};


#endif   /*__MASKHANDLE_H__*/
