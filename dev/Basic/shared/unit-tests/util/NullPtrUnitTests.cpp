//Copyright (c) 2013 Singapore-MIT Alliance for Research and Technology
//Licensed under the terms of the MIT License, as described in the file:
//   license.txt   (http://opensource.org/licenses/MIT)

#include <cmath>
#include <limits>

#include "util/LangHelpers.hpp"

#include "NullPtrUnitTests.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION(unit_tests::NullPtrUnitTests);

void unit_tests::NullPtrUnitTests::test_Equality()
{
    void* x = nullptr;
    if (x) {
        CPPUNIT_FAIL("Nullptr equality test failed.");
    }
}


void unit_tests::NullPtrUnitTests::test_NEquality()
{
    void* x = nullptr;
    if (!x) {} else {
        CPPUNIT_FAIL("Nullptr negative equality test failed.");
    }
}
