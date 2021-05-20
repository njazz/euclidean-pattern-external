// simple compatibility layer to build both both Max and Pd objects

#pragma once

#include "stdio.h"

#ifdef BUILD_PD_OBJECT
extern "C" {
#include "m_pd.h"

using t_comp_outlet = t_outlet;

using t_comp_newmethod = t_newmethod;
using t_comp_method = t_method;

inline t_comp_outlet* comp_outlet_new(t_object* x, const char* s) { return (t_comp_outlet*)::outlet_new(x, gensym(s)); }
inline void comp_outlet_delete(t_comp_outlet* x) { ::outlet_free(x); }

inline void comp_class_add_method(t_class* c, t_comp_method m, const char* n) { class_addmethod(c, m, gensym(n), A_GIMME); }
inline t_class* comp_class_new(const char* name, t_comp_newmethod n, t_comp_method f, size_t s)
{
    return class_new(gensym(name), n, f, s, 0, A_GIMME, 0);
}

inline void comp_class_register(t_class* c)
{
}

#define COMP_OBJECT_NEW pd_new
#define COMP_SYMBOL A_SYMBOL
#define COMP_W_SYMBOL w_symbol
#define COMP_LONG A_FLOAT
#define COMP_W_LONG w_float

#define COMP_NULL A_NULL;

#define COMP_T_ARGC int

#define outlet_int outlet_float

#define COMP_EXPORT EXTERN

///> atom type enum class:
#define COMP_ATOM_TYPE int
}
#endif

// ------------------------------------------------------------------------------------------------

#ifdef BUILD_MAX_OBJECT

// fix for osx w/o Carbon:
#define __FILES__
typedef void FSRef;

extern "C" {

#include "ext.h"

using t_comp_outlet = outlet;

using t_comp_newmethod = method;
using t_comp_method = method;

inline t_comp_outlet* comp_outlet_new(void* x, const char* s) { return (t_comp_outlet*)::outlet_new(x, s); }
inline void comp_outlet_delete(void* x) { ::outlet_delete(x); }

inline void comp_class_add_method(t_class* c, t_comp_method m, const char* n) { class_addmethod(c, m, n, A_GIMME, 0); }
inline t_class* comp_class_new(const char* name, t_comp_newmethod n, t_comp_method f, size_t s)
{
    return class_new(name, n, f, s, 0L, A_GIMME, 0);
};

inline void comp_class_register(t_class* c)
{
    class_register(CLASS_BOX, c);
}

#define COMP_OBJECT_NEW object_alloc
#define COMP_SYMBOL A_SYM
#define COMP_W_SYMBOL w_sym
#define COMP_LONG A_LONG
#define COMP_W_LONG w_long

#define COMP_NULL A_NOTHING;

#define COMP_T_ARGC long

#define COMP_EXPORT C74_EXPORT

///> atom type enum class:
#define COMP_ATOM_TYPE e_max_atomtypes

};

#endif

// ------------------------------------------------------------------------------------------------

// utility macros:

#define GUARD_ATOM_COUNT(__x)                     \
    if (argc < __x) {                             \
        error("bad arguments (count: %i)", argc); \
        return;                                   \
    }

#define GUARD_ATOM_TYPE(idx, t)               \
    if (argv[idx].a_type != t) {              \
        error("bad argument (%i) type", idx); \
        return;                               \
    }

#define GUARD_ATOM_TYPE_SCALAR(idx)           \
    if ((argv[idx].a_type != COMP_SYMBOL)     \
        && (argv[idx].a_type != COMP_LONG)    \
        && (argv[idx].a_type != A_FLOAT)) {   \
        error("bad argument (%i) type", idx); \
        return;                               \
    }

// utility classes:

#include <string>

namespace MaxPdObject {

class Atom {
    t_atom _obj;

public:
    Atom() { }

    Atom& operator=(const float& v)
    {
        _obj.a_type = A_FLOAT;
        _obj.a_w.w_float = v;
        return *this;
    }
    Atom& operator=(const std::string& str)
    {
        _obj.a_type = COMP_SYMBOL;
        _obj.a_w.COMP_W_SYMBOL = gensym(str.c_str());
        return *this;
    };
    Atom& operator=(const long& v)
    {
        _obj.a_type = COMP_LONG;
        _obj.a_w.COMP_W_LONG = v;
        return *this;
    }
    Atom& operator=(const std::nullptr_t&)
    {
        _obj.a_type = COMP_NULL;
        return *this;
    }

    //
    Atom& operator=(const Atom& src)
    {
        if (&src == this)
            return *this;
        _obj = src._obj;
        return *this;
    }
    Atom& operator=(const t_atom& src)
    {
        _obj = src;
        return *this;
    }

    explicit Atom(const Atom& v) { operator=(v); }
    explicit Atom(const t_atom& v) { operator=(v); }

    explicit Atom(const float& v) { operator=(v); }
    explicit Atom(const std::string& v) { operator=(v); }
    explicit Atom(const long& v) { operator=(v); }
    explicit Atom(const std::nullptr_t& v) { operator=(v); }

    // strict:
    operator float() const noexcept(false)
    {
        if (!isFloat())
            throw("Atom: bad type requested");
        return _obj.a_w.w_float;
    }
    operator long() const noexcept(false)
    {
        if (!isInt())
            throw("Atom: bad type requested");
        return _obj.a_w.COMP_W_LONG;
    };
    operator std::string() const noexcept(false)
    {
        if (!isString())
            throw("Atom: bad type requested");
        return std::string(_obj.a_w.COMP_W_SYMBOL->s_name);
    }

    //
    bool isInt() const { return _obj.a_type == COMP_LONG; }
    bool isFloat() const { return _obj.a_type == A_FLOAT; }
    bool isIntValue() const
    {
        if (isInt())
            return true;
        if (!isFloat())
            return false;
        auto floatValue = float(*this);
        return (floatValue - int(floatValue)) == 0;
    }
    bool isString() const
    {
        return _obj.a_type == COMP_SYMBOL;
    }

    //  less strict:
    long asInt() const
    {
        if (isInt())
            return long(*this);
        if (isFloat())
            return long(float(*this));
        if (isString())
            try {
                return std::stol(std::string(*this));
            } catch (std::exception&) {
            }
        throw("Atom: bad type requested");
    }

    float asFloat() const
    {
        if (isInt())
            return float(long(*this));
        if (isFloat())
            return float(*this);
        if (isString())
            try {
                return std::stof(std::string(*this));
            } catch (std::exception&) {
            }
        throw("Atom: bad type requested");
    }

    std::string asString() const
    {
        if (isInt())
            return std::to_string(long(*this));
        if (isFloat())
            return std::to_string(float(*this));
        if (isString())
            return std::string(*this);

        throw("Atom: bad type requested");
    }

    t_atom& get() { return _obj; }
};

static inline void __verifyAtomObject() { static_assert(sizeof(Atom) == sizeof(t_atom), "Atom object support error"); }

}
