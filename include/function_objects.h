//https://stackoverflow.com/a/14741161/1261153
//#include "cpp_fix.h"
//#include "delete_new.h"


#pragma once
namespace function_private
{
template<typename Result,typename ...Args>
struct abstract_function
{
    virtual Result operator()(Args... args)=0;
    virtual abstract_function *clone() const =0;
    virtual ~abstract_function() = default;
};




template<typename Func,typename Result,typename ...Args>
class concrete_function: public abstract_function<Result,Args...>
{
    Func f;
public:
    concrete_function(const Func &x)
        : f(x)
    {}
    Result operator()(Args... args) override
    {
        return f(args...);
    }
    concrete_function *clone() const override
    {
        return new concrete_function{f};
    }
};

template<typename Func>
struct func_filter
{
    typedef Func type;
};
template<typename Result,typename ...Args>
struct func_filter<Result(Args...)>
{
    typedef Result (*type)(Args...);
};

}

template<typename signature>
class FunctionObject;

template<typename Result,typename ...Args>
class FunctionObject<Result(Args...)>
{
    function_private::abstract_function<Result,Args...> *f;
public:
    FunctionObject()
        : f(nullptr)
    {}
    template<typename Func> FunctionObject(const Func &x)
        : f(new function_private::concrete_function<typename function_private::func_filter<Func>::type,Result,Args...>(x))
    {}
    FunctionObject(const FunctionObject &rhs)
        : f(rhs.f ? rhs.f->clone() : nullptr)
    {}
    FunctionObject &operator=(const FunctionObject &rhs)
    {
        if( (&rhs != this ) && (rhs.f) )
        {
            auto *temp = rhs.f->clone();
            delete f;
            f = temp;
        }
        return *this;
    }
    template<typename Func> FunctionObject &operator=(const Func &x)
    {
        auto *temp = new function_private::concrete_function<typename function_private::func_filter<Func>::type,Result,Args...>(x);
        delete f;
        f = temp;
        return *this;
    }
    Result operator()(Args... args)
    {
        if(f)
            return (*f)(args...);
        else
            return Result();
    }
    ~FunctionObject()
    {
        delete f;
    }
};

