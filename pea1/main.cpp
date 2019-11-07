#include <iostream>
#include <Menu.h>

using namespace std;

void clear(){
    system("clear");
}

void pause(){

}


int main(int argc, char* argv[])
{
//    cout<<Algorythms::log2(1)<<endl;
//    cout<<Algorythms::log2(2)<<endl;
//    cout<<Algorythms::log2(4)<<endl;
//    cout<<Algorythms::log2(8)<<endl;
//    cout<<Algorythms::log2(16)<<endl;
//    cout<<Algorythms::log2(32)<<endl;

    srand(time(NULL));
    Menu m;


    if(argc >= 2){
        m.initFromFile(argv[1]);
        m.dynamic();
        //m.test();
        //return 0;
    }

    m.run();



    return 0;
}
