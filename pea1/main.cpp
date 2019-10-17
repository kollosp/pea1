#include <iostream>
#include <Menu.h>

using namespace std;

void clear(){
    system("clear");
}

void pause(){

}


int main()
{
    srand(time(NULL));
    Menu m;
    m.run();



    return 0;
}
