// 一个简单的复读程序：先发送"Hello"，之后收到的消息全部发送回去，并显示在屏幕上
#include <iostream>
#include "simskt.h"

using namespace std;
using namespace simpid;


class Base {
public:
    Base() { cout << "Base()" << endl; }
    ~Base() {}

    virtual void _init() = 0;
};

class Derived : public Base {
public:
    void _init() override {
        cout << "Hello _init()" << endl;
    }

    Derived() {
        this->_init();
    }
};

int main()
{
    Derived d;
    Socket s;
    s.bind("0.0.0.0", 8888);
    s.listen(32);
    
    Client cli = s.accept();
    cli.send("Hello");
    while(1) {
        string msg = cli.recvall();
        if (msg.size() == 0) break;
        cout << msg << flush;
        cli.send(msg);
    }
    
    return 0;
}
