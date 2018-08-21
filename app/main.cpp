#include "main.hpp"
#include "argument.hpp"
#include <iostream>

using namespace std;
int main(int argc, char** argv) {
    //起動モードの取得
    Argument arg(argc, argv);
    cout << arg.ToString();
    cout << arg.HasOption('a');
    return 0;
}