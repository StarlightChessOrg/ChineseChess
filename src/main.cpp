#include "pipe.hpp"

int main(){
    //test::testNNUE();
    //test::testSearch();
    pipe *p = new pipe;
    p->work();
    if(p){
        delete p;
    }
    return 0;
}