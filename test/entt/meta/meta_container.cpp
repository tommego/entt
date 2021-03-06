#include <gtest/gtest.h>
#include <entt/core/hashed_string.hpp>
#include <entt/meta/container.hpp>
#include <entt/meta/factory.hpp>
#include <entt/meta/meta.hpp>
#include <entt/meta/resolve.hpp>

struct MetaContainer: ::testing::Test {
    static void SetUpTestCase() {
        entt::meta<double>().conv<int>();
        entt::meta<int>().conv<char>();
    }
};

TEST_F(MetaContainer, EmptySequenceContainer) {
    entt::meta_sequence_container container{};

    ASSERT_FALSE(container);

    entt::meta_any any{std::vector<int>{}};
    container = any.as_sequence_container();

    ASSERT_TRUE(container);
}

TEST_F(MetaContainer, StdVector) {
    std::vector<int> vec{};
    entt::meta_any any{std::ref(vec)};

    auto view = any.as_sequence_container();

    ASSERT_TRUE(view);
    ASSERT_EQ(view.value_type(), entt::resolve<int>());
    ASSERT_EQ(view.size(), 0u);

    ASSERT_TRUE(view.resize(3u));
    ASSERT_EQ(view.size(), 3u);

    view[0].cast<int>() = 2;
    view[1].cast<int>() = 3;
    view[2].cast<int>() = 4;

    auto first = view.begin();
    const auto last = view.end();

    ASSERT_FALSE(first == last);
    ASSERT_TRUE(first != last);

    ASSERT_NE(first, last);
    ASSERT_EQ((*(first++)).cast<int>(), 2);
    ASSERT_EQ((*(++first)).cast<int>(), 4);
    ASSERT_NE(first++, last);
    ASSERT_EQ(first, last);

    ASSERT_TRUE(first == last);
    ASSERT_FALSE(first != last);

    ASSERT_EQ(view[1u].cast<int>(), 3);

    auto it = view.begin();
    auto ret = view.insert(it, 0);

    ASSERT_TRUE(ret.second);
    ASSERT_FALSE(view.insert(ret.first, 'c').second);
    ASSERT_TRUE(view.insert(++ret.first, 1.).second);

    ASSERT_EQ(view.size(), 5u);
    ASSERT_EQ((*view.begin()).cast<int>(), 0);
    ASSERT_EQ((*++view.begin()).cast<int>(), 1);

    it = view.begin();
    ret = view.erase(it);

    ASSERT_TRUE(ret.second);
    ASSERT_EQ(view.size(), 4u);
    ASSERT_EQ((*ret.first).cast<int>(), 1);

    ASSERT_TRUE(view.clear());
    ASSERT_EQ(view.size(), 0u);
}

TEST_F(MetaContainer, StdArray) {
    std::array<int, 3> arr{};
    entt::meta_any any{std::ref(arr)};

    auto view = any.as_sequence_container();

    ASSERT_TRUE(view);
    ASSERT_EQ(view.value_type(), entt::resolve<int>());
    ASSERT_EQ(view.size(), 3u);

    ASSERT_FALSE(view.resize(5u));
    ASSERT_EQ(view.size(), 3u);

    view[0].cast<int>() = 2;
    view[1].cast<int>() = 3;
    view[2].cast<int>() = 4;

    auto first = view.begin();
    const auto last = view.end();

    ASSERT_FALSE(first == last);
    ASSERT_TRUE(first != last);

    ASSERT_NE(first, last);
    ASSERT_EQ((*(first++)).cast<int>(), 2);
    ASSERT_EQ((*(++first)).cast<int>(), 4);
    ASSERT_NE(first++, last);
    ASSERT_EQ(first, last);

    ASSERT_TRUE(first == last);
    ASSERT_FALSE(first != last);

    ASSERT_EQ(view[1u].cast<int>(), 3);

    auto it = view.begin();
    auto ret = view.insert(it, 0);

    ASSERT_FALSE(ret.second);
    ASSERT_FALSE(view.insert(it, 'c').second);
    ASSERT_FALSE(view.insert(++it, 1).second);

    ASSERT_EQ(view.size(), 3u);
    ASSERT_EQ((*view.begin()).cast<int>(), 2);
    ASSERT_EQ((*++view.begin()).cast<int>(), 3);

    it = view.begin();
    ret = view.erase(it);

    ASSERT_FALSE(ret.second);
    ASSERT_EQ(view.size(), 3u);
    ASSERT_EQ((*it).cast<int>(), 2);

    ASSERT_FALSE(view.clear());
    ASSERT_EQ(view.size(), 3u);
}

TEST_F(MetaContainer, EmptyAssociativeContainer) {
    entt::meta_associative_container container{};

    ASSERT_FALSE(container);

    entt::meta_any any{std::map<int, char>{}};
    container = any.as_associative_container();

    ASSERT_TRUE(container);
}

TEST_F(MetaContainer, StdMap) {
    std::map<int, char> map{{2, 'c'}, {3, 'd'}, {4, 'e'}};
    entt::meta_any any{std::ref(map)};

    auto view = any.as_associative_container();

    ASSERT_TRUE(view);
    ASSERT_FALSE(view.key_only());
    ASSERT_EQ(view.key_type(), entt::resolve<int>());
    ASSERT_EQ(view.mapped_type(), entt::resolve<char>());
    ASSERT_EQ(view.value_type(), (entt::resolve<std::pair<const int, char>>()));
    ASSERT_EQ(view.size(), 3u);

    auto first = view.begin();
    const auto last = view.end();

    ASSERT_FALSE(first == last);
    ASSERT_TRUE(first != last);

    ASSERT_NE(first, last);
    ASSERT_EQ((*(first++)).first.cast<int>(), 2);
    ASSERT_EQ((*(++first)).second.cast<char>(), 'e');
    ASSERT_NE(first++, last);
    ASSERT_EQ(first, last);

    ASSERT_TRUE(first == last);
    ASSERT_FALSE(first != last);

    ASSERT_EQ((*view.find(3)).second.cast<char>(), 'd');

    ASSERT_FALSE(view.insert('a', 'a'));
    ASSERT_FALSE(view.insert(1, 1.));

    ASSERT_TRUE(view.insert(0, 'a'));
    ASSERT_TRUE(view.insert(1., static_cast<int>('b')));

    ASSERT_EQ(view.size(), 5u);
    ASSERT_EQ((*view.find(0)).second.cast<char>(), 'a');
    ASSERT_EQ((*view.find(1)).second.cast<char>(), 'b');

    ASSERT_TRUE(view.erase(0));
    ASSERT_EQ(view.size(), 4u);
    ASSERT_EQ(view.find(0), view.end());

    (*view.find(1)).second.cast<char>() = 'f';

    ASSERT_EQ((*view.find(1)).second.cast<char>(), 'f');

    ASSERT_TRUE(view.clear());
    ASSERT_EQ(view.size(), 0u);
}

TEST_F(MetaContainer, StdSet) {
    std::set<int> set{2, 3, 4};
    entt::meta_any any{std::ref(set)};

    auto view = any.as_associative_container();

    ASSERT_TRUE(view);
    ASSERT_TRUE(view.key_only());
    ASSERT_EQ(view.key_type(), entt::resolve<int>());
    ASSERT_EQ(view.mapped_type(), entt::meta_type{});
    ASSERT_EQ(view.value_type(), entt::resolve<int>());
    ASSERT_EQ(view.size(), 3u);

    auto first = view.begin();
    const auto last = view.end();

    ASSERT_FALSE(first == last);
    ASSERT_TRUE(first != last);

    ASSERT_NE(first, last);
    ASSERT_FALSE((*first).second);
    ASSERT_EQ((*(first++)).first.cast<int>(), 2);
    ASSERT_EQ((*(++first)).first.cast<int>(), 4);
    ASSERT_NE(first++, last);
    ASSERT_EQ(first, last);

    ASSERT_TRUE(first == last);
    ASSERT_FALSE(first != last);

    ASSERT_EQ((*view.find(3)).first.cast<int>(), 3);

    ASSERT_FALSE(view.insert('0'));

    ASSERT_TRUE(view.insert(0));
    ASSERT_TRUE(view.insert(1));

    ASSERT_EQ(view.size(), 5u);
    ASSERT_EQ((*view.find(0)).first.cast<int>(), 0);
    ASSERT_EQ((*view.find(1)).first.cast<int>(), 1);

    ASSERT_TRUE(view.erase(0));
    ASSERT_EQ(view.size(), 4u);
    ASSERT_EQ(view.find(0), view.end());

    (*view.find(1)).first.cast<int>() = 42;

    ASSERT_EQ((*view.find(1)).first.cast<int>(), 1);

    ASSERT_TRUE(view.clear());
    ASSERT_EQ(view.size(), 0u);
}
