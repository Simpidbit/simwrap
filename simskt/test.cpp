// 一个简单的复读程序：先发送"Hello"，之后收到的消息全部发送回去，并显示在屏幕上
#include <iostream>
#include "simskt.h"

using namespace std;
using namespace simpid;


int main()
{
    Server s;
    s.bind("0.0.0.0", 12348);
    s.listen(32);
    
    Client cli = s.accept();
    cli.send("Hello");
    while(1) {
        string msg = cli.recv(1024);
        if (msg.size() == 0) break;
        cout << msg << flush;
        //cli.send(msg);
    }

    /*
    Client cli;
    cli.connect("127.0.0.1", 12348);
    cli.send("Hello\n");
    */
    
    return 0;
}
