#include <pthread.h>
#include <iostream>
using namespace std;
class A{
    public:
    A();
    pthread_t tid;            // thread ID
    static void *f(void *);   // thread start routine
    void g();                 // non-static member
};

A::A(){
    // "this" gets passed as argument to thread's routine (A::f)
    pthread_create(&tid, NULL, A::f, this);
}

void *A::f(void *p){
    A *a = (A *)p;    // cast *p to A class type
    a->g();           // access A's non-static member g()
    return NULL;      // do some work and return
}

void A::g(){
    std::cout << "wored";
    return;
}

int main(){
    A obj;
    return 0;
}
