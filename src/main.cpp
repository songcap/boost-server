#include "server_main.h"

int main(int argc, char **argv)
{
     if(argc < 3)
     {
         cout << "please enter serverip   port"  << endl;
         return 0;
     }
     else
     {
             //,String  bindIp,int port, string SQLIP
             boost::asio::io_service io_svr;
             int port = atoi(argv[2]);
             std::string serverip(argv[1]);
             MeterServer * Server = new MeterServer(io_svr,serverip,port);
             Server->StartService();
             return 0;
     }

}
