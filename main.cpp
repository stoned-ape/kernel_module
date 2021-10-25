#include <thread>
#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>

using namespace std;

long g=0;

//this object provides an interface for a mutex that exists in the kernel
//all communication with the mutex is done via the read system call
//system calls can mean whatever we want them to mean for our drivers
//because we're the one implementing them
//first we call open() to get a file descriptor that points to our mutex
//driver in the /dev directory
//the mutex is persistant so we must use read() to reset it
struct umtx{
    int fd;
    umtx(){
        fd=open("/dev/mtx",O_RDONLY);
        assert(fd!=-1);
        assert(read(fd,nullptr,2)==2); //reset mutex
    }
    void lock(){
        while(read(fd,nullptr,1)==0) this_thread::yield(); //lock request
    }
    void unlock(){
        long a=69;
        long *c=&a;
        cout<<c<<endl;
        read(fd,c,0); //unlock request
        
    }
};

umtx m;


void tf(){
    //100,000,000 garantees a race condition
    for(long i=0;i<1;i++){
        m.lock();
        g++;
        m.unlock();

    }
}

int main(){
    thread t1(tf),t2(tf);
    t2.join();
    t1.join();
    cout<<g<<endl;
}
