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
    srand(time(NULL));
    Menu m;


    if(argc >= 2){
        m.initFromFile(argv[1]);
        //m.dynamic();
    }

    m.run();



    return 0;
}
