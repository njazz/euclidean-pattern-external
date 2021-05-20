
#include "MaxPdObject.hpp"
#include "EuclideanPattern.hpp"

#include <map>

// -----

t_class* euclidean_pattern_class = nullptr;

struct t_euclidean_pattern {
    t_object obj;

    t_comp_outlet* out1 = nullptr;
};

// ---

static void euclidean_pattern_make_fractions(t_euclidean_pattern* x, t_symbol* s, COMP_T_ARGC argc, t_atom* argv)
{
    GUARD_ATOM_COUNT(2)
    GUARD_ATOM_TYPE(0, COMP_LONG)
    GUARD_ATOM_TYPE(1, COMP_LONG)

    auto p = Eu::GetPattern(argv[0].a_w.COMP_W_LONG, argv[1].a_w.COMP_W_LONG);
    auto fr = Eu::ToFractions(p);

    auto atomList = std::vector<MaxPdObject::Atom>();
    for (const auto& e : fr) {
        atomList.push_back(MaxPdObject::Atom(float(e)));
    }
    outlet_list(x->out1, gensym("list"), atomList.size(), (t_atom*)atomList.data());
}

static void euclidean_pattern_make_pattern(t_euclidean_pattern* x, t_symbol* s, COMP_T_ARGC argc, t_atom* argv)
{
    GUARD_ATOM_COUNT(2)
    GUARD_ATOM_TYPE(0, COMP_LONG)
    GUARD_ATOM_TYPE(1, COMP_LONG)

    auto p = Eu::GetPattern(argv[0].a_w.COMP_W_LONG, argv[1].a_w.COMP_W_LONG);

    auto atomList = std::vector<MaxPdObject::Atom>();
    for (const auto& e : p) {
        atomList.push_back(MaxPdObject::Atom(long(e)));
    }
    outlet_list(x->out1, gensym("list"), atomList.size(), (t_atom*)atomList.data());
}

// ---

static void* euclidean_pattern_new(t_symbol* s, COMP_T_ARGC argc, t_atom* argv)
{
    t_euclidean_pattern* x = (t_euclidean_pattern*)COMP_OBJECT_NEW(euclidean_pattern_class);

    x->out1 = comp_outlet_new((t_object*)x, "int");

// Max outlet order fix
#ifdef BUILD_MAX_OBJECT
    x->out1 = (t_comp_outlet*)outlet_nth((t_object*)x, 0);
#endif

    return x;
}

static void euclidean_pattern_free(t_euclidean_pattern* x)
{
    comp_outlet_delete(x->out1);
}

extern "C" {

void euclidean_pattern_setup(void)
{
    euclidean_pattern_class = comp_class_new("euclidean_pattern", (t_comp_newmethod)euclidean_pattern_new, (t_comp_method)euclidean_pattern_free, sizeof(t_euclidean_pattern));

    comp_class_add_method(euclidean_pattern_class, (t_comp_method)&euclidean_pattern_make_pattern, "make_pattern");
    comp_class_add_method(euclidean_pattern_class, (t_comp_method)&euclidean_pattern_make_fractions, "make_fractions");

    comp_class_register(euclidean_pattern_class);
}

void ext_main(void* r)
{
    euclidean_pattern_setup();
}
}
