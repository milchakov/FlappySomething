#pragma once

#ifndef singleton_h
#define singleton_h

template <class T>
class Singleton
{
public:

    static T & Instance()
    {
        static T instance;
        return instance;
    }

protected:

    Singleton() {}
    ~Singleton() {}

    Singleton(const Singleton &) = delete;
    Singleton(Singleton &&) = delete;
    Singleton & operator= (const Singleton &) = delete;
    Singleton & operator= (Singleton &&) = delete; 
};

#endif //singleton_h