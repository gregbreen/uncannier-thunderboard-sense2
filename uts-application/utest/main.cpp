///-----------------------------------------------------------------------------
///
/// @file main.cpp
///
/// @brief Test runner for the Thunderboard test suite
///
/// @copyright Copyright (c) Uncannier Software 2018
///
///-----------------------------------------------------------------------------

#include <iostream>
#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestRegistry.h>
#include <CppUTestExt/MockSupportPlugin.h>

int main( int argc, char** argv )
{
    std::cout << "Thunderboard Test" << std::endl;

    // Install a mock plugin to save us a lot of boilerplate in the test cases
    MockSupportPlugin mockPlugin;
    TestRegistry::getCurrentRegistry()->installPlugin( &mockPlugin );

    return RUN_ALL_TESTS( argc, argv );
}
