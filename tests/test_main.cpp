#include <iostream>

#include "package_manager.h"
#include "reporter.h"

#define BOOST_TEST_MODULE test_main

#include <boost/test/unit_test.hpp>
#include <boost/test/execution_monitor.hpp> 
#include <boost/test/output_test_stream.hpp>

BOOST_AUTO_TEST_SUITE(test_suite_main)

BOOST_AUTO_TEST_CASE(console_handler_output)
{
    ConsoleHandler ch;

    //---------------------------------------------------
    std::ostringstream oss;
    std::chrono::system_clock::time_point time = std::chrono::system_clock::now();
    std::string commands = "bulk: cmd1, cmd2\n";

    auto package = Package{ commands, time.time_since_epoch() };

    ch.output(package, oss);

    BOOST_CHECK_EQUAL(oss.str(), commands);

    //---------------------------------------------------
    commands = "bulk: cmd10, cmd22, cmd137\n";
    package = Package{ commands, time.time_since_epoch() };
    oss.str("");

    ch.output(package, oss);

    BOOST_CHECK_EQUAL(oss.str(), commands);

    //---------------------------------------------------
    commands = "bulk: cmd158, cmd144, cmd2, cmd157, cmd8888\n";
    package = Package{ commands, time.time_since_epoch() };
    oss.str("");

    ch.output(package, oss);

    assert(oss.str() == commands);

    BOOST_CHECK_EQUAL(oss.str(), commands);
}

BOOST_AUTO_TEST_CASE(console_handler_output)
{
    FileHandler fh;

    //---------------------------------------------------
    std::ostringstream oss;
    std::chrono::system_clock::time_point time = std::chrono::system_clock::now();
    std::string commands = "bulk: cmd1, cmd2\n";

    auto package = Package{ commands, time.time_since_epoch() };

    fh.output(package, oss);

    std::string fileName = "bulk" + std::to_string(time.time_since_epoch().count()) + ".log";
    BOOST_CHECK_EQUAL(oss.str(), fileName);

    //---------------------------------------------------
    commands = "bulk: cmd13, cmd55, cmd2228\n";
    time = std::chrono::system_clock::now();
    package = Package{ commands, time.time_since_epoch() };
    oss.str("");

    fh.output(package, oss);

    fileName = "bulk" + std::to_string(time.time_since_epoch().count()) + ".log";
    BOOST_CHECK_EQUAL(oss.str(), fileName);
}

BOOST_AUTO_TEST_SUITE_END()