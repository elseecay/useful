#include "testing.hpp"

#include "../useful/utils.hpp"
#include "../useful/span.hpp"

using namespace uf;

TEST(stf_one)
{
    {
        char c = 'a';
        assert_true(stf_one(c, 'a'));
        assert_true(stf_one(c, [](char c){ return c == 'a'; }));
        assert_true(stf_one(c, [](char){ return false; }, 'a'));
        assert_false(stf_one(c, [](char){ return false; }));
    }

    {
        int i = 5;
        assert_true(stf_one(i, 5));
        assert_true(stf_one(i, [](int i){ return i == 5; }));
        assert_true(stf_one(i, [](int){ return false; }, 5));
        assert_false(stf_one(i, [](int){ return false; }));
    }

    {
        std::string s = "12345";
        assert_true(stf_one(s, "12345"));
        assert_true(stf_one(s, [](const std::string& s){ return s == "12345"; }));
        assert_true(stf_one(s, [](const std::string&){ return false; }, [](const std::string& s){ return s == "12345"; }));
        assert_false(stf_one(s, [](const std::string&){ return false; }));
    }
}

TEST(stf_first_obj)
{
    {
        std::pair<int, double> x(1, 2.0);
        assert_true(stf_first_obj(1)(x));
        assert_false(stf_first_obj([](int x){ return x != 1; })(x));
    }
}

TEST(remove_associative)
{
    {
        std::map<std::string, int> m{{"123", 123}, {"111", 111}};
        remove_associative(m, stf_first_obj("123"));
        assert_false(m.count("123"));
        assert_true(m.size() == 1);
        assert_true(m["111"] == 111);
    }

    {
        std::map<std::string, int> m{{"123", 123}, {"111", 111}, {"222", 222}};
        remove_associative(m, stf_first_obj("123"));
        assert_false(m.count("123"));
        assert_true(m.size() == 2);
        assert_true(m["111"] == 111);
        assert_true(m["222"] == 222);
    }

    {
        std::map<std::string, int> m{{"123", 123}, {"124", 124}, {"111", 111}, {"222", 222}};
        remove_associative(m, stf_first_obj("123"), stf_first_obj([](const auto& key){ return key == "124"; }));
        assert_false(m.count("123"));
        assert_false(m.count("124"));
        assert_true(m.size() == 2);
        assert_true(m["111"] == 111);
        assert_true(m["222"] == 222);
    }

    {
        std::unordered_multimap<std::string, int> m{{"123", 123}, {"123", 123}, {"124", 124}, {"124", 124}, {"111", 111}, {"222", 222}};
        remove_associative(m, stf_first_obj("123"), stf_first_obj([](const auto& key){ return key == "124"; }));
        assert_false(m.count("123"));
        assert_false(m.count("124"));
        assert_true(m.size() == 2);
        assert_true(m.find("111")->second == 111);
        assert_true(m.find("222")->second == 222);
    }


    {
        std::unordered_multiset<int> s{1, 2, 3, 4, 4, 4, 4, 5, 5, 5};
        remove_associative(s, 4, [](const auto& key){ return key == 5; });
        assert_false(s.count(4));
        assert_false(s.count(5));
        assert_true(s.size() == 3);
        assert_true(s.count(1));
        assert_true(s.count(2));
        assert_true(s.count(3));
    }

    {
        std::map<std::string, int> m{{"123", 123}, {"111", 111}};
        remove_associative(m, stf_second_obj(123));
        assert_false(m.count("123"));
        assert_true(m.size() == 1);
        assert_true(m["111"] == 111);
    }

    {
        std::map<std::string, int> m{{"123", 123}, {"111", 111}, {"222", 222}};
        remove_associative(m, stf_second_obj(123));
        assert_false(m.count("123"));
        assert_true(m.size() == 2);
        assert_true(m["111"] == 111);
        assert_true(m["222"] == 222);
    }

    {
        std::map<std::string, int> m{{"123", 123}, {"124", 124}, {"111", 111}, {"222", 222}};
        remove_associative(m, stf_second_obj(123), stf_second_obj([](const auto& value){ return value == 124; }));
        assert_false(m.count("123"));
        assert_false(m.count("124"));
        assert_true(m.size() == 2);
        assert_true(m["111"] == 111);
        assert_true(m["222"] == 222);
    }

    {
        std::unordered_multimap<std::string, int> m{{"123", 123}, {"123", 123}, {"124", 124}, {"124", 124}, {"111", 111}, {"222", 222}};
        remove_associative(m, stf_second_obj(123), stf_second_obj([](const auto& value){ return value == 124; }));
        assert_false(m.count("123"));
        assert_false(m.count("124"));
        assert_true(m.size() == 2);
        assert_true(m.find("111")->second == 111);
        assert_true(m.find("222")->second == 222);
    }

    {
        std::unordered_multiset<int> s{1, 2, 3, 4, 4, 4, 4, 5, 5, 5};
        remove_associative(s, 4, [](const auto& value){ return value == 5; });
        assert_false(s.count(4));
        assert_false(s.count(5));
        assert_true(s.size() == 3);
        assert_true(s.count(1));
        assert_true(s.count(2));
        assert_true(s.count(3));
    }
}

TEST(remove_associative_copy)
{
    {
        std::map<std::string, int> m{{"123", 123}, {"111", 111}};
        auto result = remove_associative_copy(m, stf_first_obj("123"));
        assert_false(result.count("123"));
        assert_true(result.size() == 1);
        assert_true(result["111"] == 111);
    }

    {
        std::map<std::string, int> m{{"123", 123}, {"111", 111}, {"222", 222}};
        auto result = remove_associative_copy(m, stf_first_obj("123"));
        assert_false(result.count("123"));
        assert_true(result.size() == 2);
        assert_true(result["111"] == 111);
        assert_true(result["222"] == 222);
    }

    {
        std::map<std::string, int> m{{"123", 123}, {"124", 124}, {"111", 111}, {"222", 222}};
        auto result = remove_associative_copy(m, stf_first_obj("123"), stf_first_obj([](const auto& key){ return key == "124"; }));
        assert_false(result.count("123"));
        assert_false(result.count("124"));
        assert_true(result.size() == 2);
        assert_true(result["111"] == 111);
        assert_true(result["222"] == 222);
    }

    {
        std::unordered_multimap<std::string, int> m{{"123", 123}, {"123", 123}, {"124", 124}, {"124", 124}, {"111", 111}, {"222", 222}};
        auto result = remove_associative_copy(m, stf_first_obj("123"), stf_first_obj([](const auto& key){ return key == "124"; }));
        assert_false(result.count("123"));
        assert_false(result.count("124"));
        assert_true(result.size() == 2);
        assert_true(result.find("111")->second == 111);
        assert_true(result.find("222")->second == 222);
    }

    {
        std::map<std::string, int> m{{"123", 123}, {"111", 111}};
        auto result = remove_associative_copy(m, stf_second_obj(123));
        assert_false(result.count("123"));
        assert_true(result.size() == 1);
        assert_true(result["111"] == 111);
    }

    {
        std::map<std::string, int> m{{"123", 123}, {"111", 111}, {"222", 222}};
        auto result = remove_associative_copy(m, stf_second_obj(123));
        assert_false(result.count("123"));
        assert_true(result.size() == 2);
        assert_true(result["111"] == 111);
        assert_true(result["222"] == 222);
    }

    {
        std::map<std::string, int> m{{"123", 123}, {"124", 124}, {"111", 111}, {"222", 222}};
        auto result = remove_associative_copy(m, stf_second_obj(123), stf_second_obj([](const auto& value){ return value == 124; }));
        assert_false(result.count("123"));
        assert_false(result.count("124"));
        assert_true(result.size() == 2);
        assert_true(result["111"] == 111);
        assert_true(result["222"] == 222);
    }

    {
        std::unordered_multimap<std::string, int> m{{"123", 123}, {"123", 123}, {"124", 124}, {"124", 124}, {"111", 111}, {"222", 222}};
        auto result = remove_associative_copy(m, stf_second_obj(123), stf_second_obj([](const auto& value){ return value == 124; }));
        assert_false(result.count("123"));
        assert_false(result.count("124"));
        assert_true(result.size() == 2);
        assert_true(result.find("111")->second == 111);
        assert_true(result.find("222")->second == 222);
    }

    {
        std::unordered_multiset<int> s{1, 2, 3, 4, 4, 4, 4, 5, 5, 5};
        auto result = remove_associative_copy(s, 4, [](const auto& key){ return key == 5; });
        assert_false(result.count(4));
        assert_false(result.count(5));
        assert_true(result.size() == 3);
        assert_true(result.count(1));
        assert_true(result.count(2));
        assert_true(result.count(3));
    }
}

template<typename C>
void f(C&&)
{
    cout << mt::is_iterable_v<std::remove_reference_t<C>> << endl;
}

TEST(span)
{
    {
        std::vector<int> v{2, 5};
        span s(v);
        assert_eq(s.size(), 2);
        assert_eq(s[0], 2);
        assert_eq(s[1], 5);
    }

    {
        std::string str("12");
        span s(str.data(), 2);
        const span<const char> cs(str);
        assert_eq(cs.size(), 2);
        assert_eq(cs[0], '1');
        assert_eq(cs[1], '2');
        s[0] = 'e';
        assert_eq(cs[0], 'e');
    }

    {
        std::vector<double> v{1.35, 1.112, 5.55};
        span s(v);
        auto j = s.crbegin();
        for (i64 i = 2; j != s.crend(); --i, ++j)
            assert_eq(v[i], *j);
    }

    {
        int a1[5]{1, 2, 3, 4, 5};
        span s1(a1);
        assert_eq(s1.size(), 5);
        assert_eq(s1[3], 4);
    }

    {
        std::array<int, 5> a2{{1, 2, 3, 4, 5}};
        span s2(a2);
        assert_eq(s2.size(), 5);
        assert_eq(s2[3], 4);
    }

    {
        std::string s;
        const std::string& sref = s;
        span sp{s};
    }
}

