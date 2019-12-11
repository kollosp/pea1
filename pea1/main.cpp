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

        if(argc >= 3){
            std::string str(argv[2]);
            if(str == "tabu"){
                m.tabuSearchTest();
            }
            else if(str == "sim"){
                m.simAnnealingTest();
            }

            return 0;
        }
    }

    m.run();



    return 0;
}
