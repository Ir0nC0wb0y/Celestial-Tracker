#include "pch.h"
#include <iostream>
#include "HelloWorld.h"

void sumInternal(int, int, int&);

Geek geek{};

Geek::Geek()
{
    _var = 0;
    return;
}

void Geek::update()
{
    _var++;
    return;
}

int Geek::getVar() const
{
    return _var;
}

void Geek::initVar()
{
    _var = 0;
    return;
}

void helloWorld()
{
    std::cout << "Hello, World!" << std::endl;
    return;
}

void sumInternal(int a, int b, int& c)
{
    c = a + b;
    return;
}

int sum(int a, int b)
{
    int c{};
    sumInternal(a, b, c);
    return c;
}

int sum2(int32_t in[2])
{
    return in[0] + in[1];
}

void initialize()
{
    geek.initVar();
    return;
}

int update()
{
    geek.update();
    return geek.getVar();
}