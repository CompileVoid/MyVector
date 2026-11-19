#include <gtest/gtest.h>
#include <numeric>
#include <sstream>
#include <string>
#include <utility>
#include "MyVector.h"

struct Tracker {
    static inline int alive_count = 0;
    int value{0};

    Tracker() : value(0) { ++alive_count; }
    explicit Tracker(int v) : value(v) { ++alive_count; }
    Tracker(const Tracker& other) : value(other.value) { ++alive_count; }
    Tracker(Tracker&& other) noexcept : value(other.value) {
        other.value = -1;
        ++alive_count;
    }
    Tracker& operator=(const Tracker& other) {
        value = other.value;
        return *this;
    }
    Tracker& operator=(Tracker&& other) noexcept {
        value = other.value;
        other.value = -1;
        return *this;
    }
    ~Tracker() { --alive_count; }
};

TEST(MyVectorConstructorTest, DefaultConstructor) {
    MyVector<int> vec;
    EXPECT_TRUE(vec.isEmpty());
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 0);
    EXPECT_EQ(vec.begin(), nullptr);
    EXPECT_EQ(vec.end(), nullptr);
}

TEST(MyVectorConstructorTest, SizeConstructorZero) {
    MyVector<int> vec(0);
    EXPECT_TRUE(vec.isEmpty());
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 0);
}

TEST(MyVectorConstructorTest, SizeConstructorDefaultInit) {
    MyVector<int> vec(5);
    EXPECT_FALSE(vec.isEmpty());
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec.capacity(), 10);
    for (std::size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i], 0);
    }
}

TEST(MyVectorConstructorTest, SizeAndValueConstructorZero) {
    MyVector<int> vec(0, 99);
    EXPECT_TRUE(vec.isEmpty());
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 0);
}

TEST(MyVectorConstructorTest, SizeAndValueConstructor) {
    MyVector<std::string> vec(4, "test");
    EXPECT_EQ(vec.size(), 4);
    EXPECT_EQ(vec.capacity(), 8);
    for (std::size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i], "test");
    }
}

TEST(MyVectorConstructorTest, InitializerListConstructor) {
    MyVector<int> vec = {10, 20, 30, 40};
    EXPECT_EQ(vec.size(), 4);
    EXPECT_EQ(vec.capacity(), 8);
    EXPECT_EQ(vec.at(0), 10);
    EXPECT_EQ(vec.at(1), 20);
    EXPECT_EQ(vec.at(2), 30);
    EXPECT_EQ(vec.at(3), 40);
}

TEST(MyVectorConstructorTest, InitializerListStrings) {
    MyVector<std::string> vec{"one", "two", "three"};
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec.capacity(), 6);
    EXPECT_EQ(vec.at(0), "one");
    EXPECT_EQ(vec.at(1), "two");
    EXPECT_EQ(vec.at(2), "three");
}

TEST(MyVectorConstructorTest, InitializerListEmpty) {
    MyVector<int> vec = {};
    EXPECT_TRUE(vec.isEmpty());
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 0);
    EXPECT_EQ(vec.begin(), nullptr);
}

TEST(MyVectorCopyMoveTest, CopyConstructorEmpty) {
    MyVector<int> vec1;
    MyVector<int> vec2(vec1);
    EXPECT_TRUE(vec2.isEmpty());
    EXPECT_EQ(vec2.capacity(), 0);
}

TEST(MyVectorCopyMoveTest, CopyConstructorDeepCopy) {
    MyVector<int> vec1(3, 10);
    MyVector<int> vec2(vec1);
    EXPECT_EQ(vec2.size(), 3);
    EXPECT_EQ(vec2.capacity(), 6);
    vec2.at(0) = 999;
    EXPECT_EQ(vec1.at(0), 10);
    EXPECT_EQ(vec2.at(0), 999);
}

TEST(MyVectorCopyMoveTest, CopyAssignmentSelf) {
    MyVector<int> vec(3, 7);
    vec = vec;
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec.at(0), 7);
}

TEST(MyVectorCopyMoveTest, CopyAssignmentNonEmptyToEmpty) {
    MyVector<int> vec1(2, 5);
    MyVector<int> vec2;
    vec2 = vec1;
    EXPECT_EQ(vec2.size(), 2);
    EXPECT_EQ(vec2.at(0), 5);
    EXPECT_EQ(vec2.at(1), 5);
}

TEST(MyVectorCopyMoveTest, CopyAssignmentEmptyToNonEmpty) {
    MyVector<int> vec1;
    MyVector<int> vec2(3, 8);
    vec2 = vec1;
    EXPECT_TRUE(vec2.isEmpty());
    EXPECT_EQ(vec2.capacity(), 0);
}

TEST(MyVectorCopyMoveTest, MoveConstructor) {
    MyVector<std::string> vec1;
    vec1.pushBack("alpha");
    vec1.pushBack("beta");

    MyVector<std::string> vec2(std::move(vec1));
    EXPECT_EQ(vec2.size(), 2);
    EXPECT_EQ(vec2.at(0), "alpha");
    EXPECT_EQ(vec2.at(1), "beta");
    EXPECT_TRUE(vec1.isEmpty());
    EXPECT_EQ(vec1.capacity(), 0);
    EXPECT_EQ(vec1.begin(), nullptr);
}

TEST(MyVectorCopyMoveTest, MoveAssignmentSelf) {
    MyVector<int> vec(2, 42);
    vec = std::move(vec);
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec.at(0), 42);
}

TEST(MyVectorCopyMoveTest, MoveAssignment) {
    MyVector<int> vec1(3, 11);
    MyVector<int> vec2(5, 22);
    vec2 = std::move(vec1);

    EXPECT_EQ(vec2.size(), 3);
    EXPECT_EQ(vec2.at(0), 11);
    EXPECT_TRUE(vec1.isEmpty());
    EXPECT_EQ(vec1.capacity(), 0);
}

TEST(MyVectorAccessTest, SubscriptOperatorReadWrite) {
    MyVector<int> vec(3, 0);
    for (std::size_t i = 0; i < vec.size(); ++i) {
        vec[i] = static_cast<int>((i + 1) * 10);
    }
    for (std::size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i], static_cast<int>((i + 1) * 10));
    }
}

TEST(MyVectorAccessTest, ConstSubscriptAndAt) {
    MyVector<int> vec(2, 5);
    vec.at(1) = 15;
    const MyVector<int>& cvec = vec;
    for (std::size_t i = 0; i < cvec.size(); ++i) {
        EXPECT_EQ(cvec[i], cvec.at(i));
    }
    EXPECT_EQ(cvec.at(0), 5);
    EXPECT_EQ(cvec.at(1), 15);
    EXPECT_THROW((void)cvec.at(2), std::out_of_range);
}

TEST(MyVectorAccessTest, AtBoundsChecking) {
    MyVector<int> vec;
    EXPECT_THROW(vec.at(0), std::out_of_range);

    vec.pushBack(100);
    EXPECT_EQ(vec.at(0), 100);
    vec.at(0) = 200;
    EXPECT_EQ(vec.at(0), 200);
    EXPECT_THROW(vec.at(1), std::out_of_range);
}

TEST(MyVectorIteratorTest, RangeBasedForAndAlgorithms) {
    MyVector<int> vec;
    for (int i = 1; i <= 5; ++i) {
        vec.pushBack(i);
    }

    int sum = std::accumulate(vec.begin(), vec.end(), 0);
    EXPECT_EQ(sum, 15);

    for (auto& x : vec) {
        x *= 2;
    }
    EXPECT_EQ(vec.at(0), 2);
    EXPECT_EQ(vec.at(4), 10);
}

TEST(MyVectorIteratorTest, ConstIterators) {
    MyVector<int> vec(3, 4);
    const MyVector<int>& cvec = vec;
    int count = 0;
    for (auto it = cvec.begin(); it != cvec.end(); ++it) {
        EXPECT_EQ(*it, 4);
        ++count;
    }
    EXPECT_EQ(count, 3);
}

TEST(MyVectorCapacityTest, ReserveIncreasesCapacity) {
    MyVector<int> vec;
    vec.pushBack(1);
    vec.pushBack(2);
    vec.reserve(50);

    EXPECT_EQ(vec.capacity(), 50);
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec.at(0), 1);
    EXPECT_EQ(vec.at(1), 2);
}

TEST(MyVectorCapacityTest, ReserveSmallerOrEqualDoesNothing) {
    MyVector<int> vec(5, 1);
    std::size_t initial_cap = vec.capacity();
    vec.reserve(initial_cap);
    EXPECT_EQ(vec.capacity(), initial_cap);
    vec.reserve(2);
    EXPECT_EQ(vec.capacity(), initial_cap);
}

TEST(MyVectorModifierTest, PushBackCopyAndMove) {
    MyVector<std::string> vec;
    std::string lvalue = "lvalue_string";
    vec.pushBack(lvalue);
    vec.pushBack(std::string("rvalue_string"));

    EXPECT_EQ(lvalue, "lvalue_string");
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec.at(0), "lvalue_string");
    EXPECT_EQ(vec.at(1), "rvalue_string");
}

TEST(MyVectorModifierTest, PushBackReallocationGrowth) {
    MyVector<int> vec;
    for (int i = 0; i < 100; ++i) {
        vec.pushBack(i);
        EXPECT_GE(vec.capacity(), vec.size());
    }
    EXPECT_EQ(vec.size(), 100);
    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(vec[i], i);
    }
}

TEST(MyVectorModifierTest, PopBack) {
    MyVector<int> vec;
    EXPECT_THROW(vec.popBack(), std::out_of_range);

    vec.pushBack(10);
    vec.pushBack(20);
    vec.popBack();
    EXPECT_EQ(vec.size(), 1);
    EXPECT_EQ(vec.at(0), 10);

    vec.popBack();
    EXPECT_TRUE(vec.isEmpty());
    EXPECT_THROW(vec.popBack(), std::out_of_range);
}

TEST(MyVectorModifierTest, Clear) {
    MyVector<int> vec(5, 3);
    vec.clear();
    EXPECT_TRUE(vec.isEmpty());
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 0);
    EXPECT_EQ(vec.begin(), nullptr);

    vec.pushBack(42);
    EXPECT_EQ(vec.size(), 1);
    EXPECT_EQ(vec.at(0), 42);
}

TEST(MyVectorModifierTest, ResizeDefaultValue) {
    MyVector<int> vec(2, 7);
    vec.resize(5);
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec.at(0), 7);
    EXPECT_EQ(vec.at(1), 7);
    EXPECT_EQ(vec.at(2), 0);
    EXPECT_EQ(vec.at(3), 0);
    EXPECT_EQ(vec.at(4), 0);

    vec.resize(1);
    EXPECT_EQ(vec.size(), 1);
    EXPECT_EQ(vec.at(0), 7);

    vec.resize(1);
    EXPECT_EQ(vec.size(), 1);
}

TEST(MyVectorModifierTest, ResizeCustomValue) {
    MyVector<int> vec(1, 3);
    vec.resize(4, 9);
    EXPECT_EQ(vec.size(), 4);
    EXPECT_EQ(vec.at(0), 3);
    EXPECT_EQ(vec.at(1), 9);
    EXPECT_EQ(vec.at(2), 9);
    EXPECT_EQ(vec.at(3), 9);

    vec.resize(0, 9);
    EXPECT_TRUE(vec.isEmpty());
}

TEST(MyVectorStreamTest, OutputOperator) {
    MyVector<int> vec = {1, 2, 3};
    std::ostringstream oss;
    oss << vec;
    EXPECT_EQ(oss.str(), "1 2 3 \n");
}

TEST(MyVectorStreamTest, OutputOperatorEmpty) {
    MyVector<int> vec;
    std::ostringstream oss;
    oss << vec;
    EXPECT_EQ(oss.str(), "\n");
}

TEST(MyVectorLifecycleTest, ObjectDestructionCount) {
    EXPECT_EQ(Tracker::alive_count, 0);
    {
        MyVector<Tracker> vec = {Tracker(1), Tracker(2)};
        vec.pushBack(Tracker(3));
        vec.reserve(10);
        MyVector<Tracker> copy(vec);
        MyVector<Tracker> moved(std::move(copy));
    }
    EXPECT_EQ(Tracker::alive_count, 0);
}
