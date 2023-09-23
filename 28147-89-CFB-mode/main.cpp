#include <array>
#include <bitset>
#include <iostream>

#include "file.h"

#include "CFB.h"
#include "ECB.h"

int main(int argc, char** argv)
{
    test_reader();
    test_CFB();
    test_ECB();

    return 0;
}