/*
 * Copyright Aleksey Verkholat 2018
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt
*/

#include <iostream>
#include <fstream>
#include <set>

#include "useful/useful.hpp"
#include "useful/out_overloads.hpp"
#include "useful/in_overloads.hpp"

#define check_types_equal(t1, t2) static_assert(std::is_same_v<t1, t2>)
#define check_expr_type(expression, type) static_assert(std::is_same_v<decltype(expression), type>)

using namespace std;

using namespace uf::out_overloads;
using namespace uf::in_overloads;



int main()
{
    cout << uf::strip("123 4"s, [](char c){return c == '4' || c == ' ' || c == '1';}) << endl;
    cout << uf::strip_sides("123 4"s, [](char c){return c == '1' || c == '2' || c == '4' || c == ' ';}) << endl;
    stringstream ss;
    ss << "123, 124\n125, 126\n       ";
    auto r = uf::read_csv(ss);
    return 0;
}
