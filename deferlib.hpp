#ifndef DRA_DEFER_LIB
#define DRA_DEFER_LIB

namespace dra_defer_lib
{

template<typename Fn>
struct defer_s
{
    defer_s(Fn const &fn)
    :_fn{fn}
    {}

    ~defer_s()
    {
        _fn();
    }

    Fn _fn;
};

template<typename Fn>
static inline defer_s<Fn> defer_f(Fn const &fn)
{
    return defer_s<Fn>(fn);
}

struct defer_aux_s
{};

template<typename Fn>
static inline defer_s<Fn> operator<<(defer_aux_s const &defer_aux_obj, Fn const &fn)
{
    return defer_f<Fn>(fn);
}

}

#define DRA_DEFER_LIB_CAT_INTERNAL(x, y) x##y
#define DRA_DEFER_LIB_CAT(x, y) DRA_DEFER_LIB_CAT_INTERNAL(x, y)

#ifdef __COUNTER__
#define DRA_DEFER_LIB_COUNTER __COUNTER__
#else
#define DRA_DEFER_LIB_COUNTER __LINE__
#endif

#define DRA_DEFER_LIB_UNIQUE_IDENTIFIER(name) DRA_DEFER_LIB_CAT(name, DRA_DEFER_LIB_COUNTER)

#define defer auto DRA_DEFER_LIB_UNIQUE_IDENTIFIER(defer_instance_) = dra_defer_lib::defer_aux_s{} << [&]()

#endif // DRA_DEFER_LIB
