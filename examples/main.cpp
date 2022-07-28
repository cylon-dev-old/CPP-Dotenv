#include <iostream>
#include "dotenv.hpp"

using namespace std;
using namespace dotenv;

int main() {
    auto env = dotenv::load_dotenv("./test.env");
    
    cout << "My name is " << env["NAME"];
    
    return 0;
}
