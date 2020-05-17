#include <gtest/gtest.h>

#include "defs.h"

TEST (test_misc, clear) {
    std::string input1 = " trash";
    const std::string expected1 = " trash";

    defs::clear(input1);

    ASSERT_TRUE(expected1 == input1);

    std::string input2 = "  trash";
    const std::string expected2 = " trash";

    defs::clear(input2);

    ASSERT_TRUE(expected2 == input2);

    std::string input3 = "trash ";
    const std::string expected3 = "trash ";

    defs::clear(input3);

    ASSERT_TRUE(expected3 == input3);

    std::string input4 = "trash  ";
    const std::string expected4 = "trash ";

    defs::clear(input4);

    ASSERT_TRUE(expected4 == input4);

    std::string input5 = " trash   trummor ";
    const std::string expected5 = " trash trummor ";

    defs::clear(input5);

    ASSERT_TRUE(expected5 == input5);
}

TEST (test_misc, split) {
    const std::string unsplitted = " trash  trumor";

    auto splitted = defs::split(unsplitted);

    ASSERT_TRUE(splitted[0] == "trash");
    ASSERT_TRUE(splitted[1] == "trumor");
}

TEST (test_misc, is_integer) {
    const std::string trash = "trash1996";

    ASSERT_FALSE (defs::is_integer(trash));

    const std::string positive = "1990";

    ASSERT_TRUE (defs::is_integer(positive));

    const std::string negative = "-1990";

    ASSERT_TRUE (defs::is_integer(negative));

    const std::string floated = "1990.0";

    ASSERT_FALSE (defs::is_integer(floated));
}
