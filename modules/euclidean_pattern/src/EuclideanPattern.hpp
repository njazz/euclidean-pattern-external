#pragma once

#include <array>
#include <map>
#include <vector>

#include <math.h>

namespace Eu {

using Pattern = std::vector<unsigned int>;
using Fractions = std::vector<float>;

///> converts pattern to string
static inline const std::string PrintPattern(const Pattern& p)
{
    std::string ret;

    ret += ("[ ");
    auto idx = 0;
    for (auto& e : p) {
        if ((idx % 4 == 0) && (idx != 0))
            ret += ("| __ ");

        ret += (e > 0 ? "|#" : "| ");
        idx++;
    }
    ret += ("| ]\n");
    return ret;
}

///> generates list with n elements filled with value
static inline Pattern MakeList(const size_t& size, const unsigned int& value)
{
    Pattern ret;

    for (size_t i = 0; i < size; i++)
        ret.push_back(value);

    return ret;
};

///> main function
static inline std::array<Pattern, 2> ProcessLists(Pattern& a, Pattern& b, int v = 1)
{
    auto min_size = ((a.size() / v) < b.size()) ? (a.size() / v) : b.size();

    Pattern retA;
    Pattern retB;

    auto p_a = 0;
    auto p_b = 0;

    for (int i = 0; i < min_size; i++) {
        for (int j = 0; j < v; j++) {
            retA.push_back(a[i * v + (j)]);
            p_a++;
        }
        retA.push_back(b[i]);
        p_b++;
    };

    for (auto i = p_a; i < a.size(); i++)
        retA.push_back(a[i]);
    for (auto i = p_b; i < b.size(); i++)
        retB.push_back(b[i]);

    return std::array<Pattern, 2>({ { retA, retB } });
}

///> solution using Bresengham line algorithm
/// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
/// https://en.wikipedia.org/wiki/Euclidean_algorithm
static inline Pattern GetPattern(const size_t& a, const size_t& b){
    Pattern ret;

    float prev = -1;

    auto len = a+b;

    if (a==0) return {};

    for (int i=0;i<len;i++){
        float current = floorf(float(i)/len*a);
        ret.push_back((current != prev));
            prev = current;
    }

    return ret;
}

///> converts pattern to fractions value
const Fractions ToFractions(const Pattern& v)
{
    Fractions ret;
    int idx = 0;
    for (auto e : v) {
        float fract = float(idx) / v.size();

        if (e > 0)
            ret.push_back(fract);

        idx++;
    }
    return ret;
}

}
