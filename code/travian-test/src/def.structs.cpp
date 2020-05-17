#include <cstdlib>
#include <gtest/gtest.h>

#include "defs.structs.h"

TEST (test_vector, creation) {
    const int size = 5;
    int value = 10;

    defs::ivector<size> vec(value);

    for (int i = 0; i < size; ++i) {
        ASSERT_EQ (vec[i], value);
    }
}

TEST (test_vector, copy) {
    const int size = 5;
    int value = 10;

    defs::ivector<size> vec(value);
    defs::ivector<size> vec_copy;

    for (int i = 0; i < size; ++i) {
        ASSERT_EQ (vec[i], value);
        ASSERT_EQ (vec_copy[i], 0);
    }

    vec_copy = vec;

    for (int i = 0; i < size; ++i) {
        ASSERT_EQ (vec[i], value);
        ASSERT_EQ (vec_copy[i], value);
    }
}

TEST (test_vector, less_equal) {
    const int size = 3;

    defs::ivector<size> left;
    defs::ivector<size> right;

    left[0] = 0;
    left[1] = 1;
    left[2] = 2;

    right[0] = 1;
    right[1] = 2;
    right[2] = 3;

    ASSERT_EQ (left <= right, true);

    right[0] = 0;
    right[1] = 1;
    right[2] = 2;

    ASSERT_EQ (left <= right, true);

    right[0] = 0;
    right[1] = 1;
    right[2] = 1;

    ASSERT_EQ (left <= right, false);

}

TEST (test_vector, subtraction) {
    const int size = 3;

    int left_value = std::rand() % 255;
    int right_value = std::rand() % 255;

    defs::ivector<size> left(left_value);
    defs::ivector<size> right(right_value);

    defs::ivector<size> substracted = left - right;


    for (int i = 0; i < size; ++i) {
        ASSERT_EQ (substracted[i], left_value - right_value);
    }
}

TEST (test_vector, maximum) {
    const int size = 3;

    defs::ivector<size> vec;

    vec[0] = 1;
    vec[1] = 2;
    vec[2] = 3;

    ASSERT_EQ(vec.max(), std::make_pair(2u, 3));

    vec[0] = 3;
    vec[1] = 2;
    vec[2] = 1;

    ASSERT_EQ(vec.max(), std::make_pair(0u, 3));

    vec[0] = 1;
    vec[1] = 3;
    vec[2] = 3;

    ASSERT_EQ(vec.max(), std::make_pair(1u, 3));
}

TEST (test_vector, comparison) {
    const int size = 3;

    int value = std::rand() % 255;

    defs::ivector<size> left(value);
    defs::ivector<size> right(value);

    ASSERT_EQ(left == right, true);

    right[0] = -1;

    ASSERT_EQ(left == right, false);

}
