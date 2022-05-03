#pragma once

#define HELLOWORLD_EXPORTS

#ifdef HELLOWORLD_EXPORTS
#define HELLOWORLD_API __declspec(dllexport)
#else
#define HELLOWORLD_API __declspec(dllimport)
#endif

class Geek
{
public:
    Geek();

    void update();
    int getVar() const;
    void initVar();

private:
    int _var{};
};

extern "C" HELLOWORLD_API void helloWorld();

extern "C" HELLOWORLD_API int sum(int, int);

extern "C" HELLOWORLD_API int sum2(int32_t[2]);

extern "C" HELLOWORLD_API float sum3(float, float);

extern "C" HELLOWORLD_API void initialize();

extern "C" HELLOWORLD_API int update();