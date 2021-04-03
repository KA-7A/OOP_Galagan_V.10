//
// Created by Администратор on 03.04.2021.
//

#include <iostream>
#include "station.h"
#include <unistd.h>

int main()
{
    for (int i = 0; i < 30; i++)
    {
        S_Station tmp(i);
        sleep(1);
    }

}