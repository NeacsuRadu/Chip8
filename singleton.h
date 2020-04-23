#ifndef SINGLETON_H
#define SINGLETON_H
template <class T>
class singleton
{
public:
    static T& get_instance()
    {
        if (!instance)
        {
            instance = new T();
        }
        
        return *(instance);
    }

    static void relese_instance()
    {
        delete instance;
        instance = nullptr;
    }
protected:
    singleton() = default;
private:
    static T * instance;
};
#endif
