#include <bluetooth/bluetooth.h>

#include <iostream>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
//#include <bluetooth/l2cap.h>  // may be later will add
#include <bluetooth/rfcomm.h>

using namespace std;

class CON{

    public:
        int log = 0;
        struct sockaddr_rc addr = {0};
        int sock = 0 ;
        int channel = 1;

        CON(string adress){
            sock = socket(AF_BLUETOOTH , SOCK_SEQPACKET , BTPROTO_RFCOMM);

            addr.rc_family = AF_BLUETOOTH;
            addr.rc_channel = (uint8_t)channel;
            str2ba(adress.c_str() , &addr.rc_bdaddr);
            log = 1;
            
            if(connect(sock , (struct sockaddr *)&addr ,sizeof(addr) ) < 0){
                return ;
            }

        }
        int send(string message){
            return ((int)::send(sock , message.c_str() , strlen(message.c_str()) , 0));
        }
        string read(){
            string ans = "";
            int a = 0x0;
            char buff[1024] = {0};
            while((a = recv(sock , buff , 1024 , 0)) > 0){
                for(int ind = 0; ind <a; ind++ ){
                    ans=  ans + buff[ind];
                }
            }
            return ans;
        }


};
