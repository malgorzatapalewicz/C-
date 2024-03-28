#include "escape.h"

int main(){
    std::srand( std::time(0));
    auto flappy = std::make_shared<Escape>();
    flappy->run();
    return 0;
}
