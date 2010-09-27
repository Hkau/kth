/* Generated file, do not edit */
#ifdef SOKOBAN_TEST
#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/ErrorPrinter.h>

int main() {
 return CxxTest::ErrorPrinter().run();
}
#include "unittest.h"

static UnitTest suite_UnitTest;

static CxxTest::List Tests_UnitTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_UnitTest( "unittest.h", 24, "UnitTest", suite_UnitTest, Tests_UnitTest );

static class TestDescription_UnitTest_testQueue : public CxxTest::RealTestDescription {
public:
 TestDescription_UnitTest_testQueue() : CxxTest::RealTestDescription( Tests_UnitTest, suiteDescription_UnitTest, 27, "testQueue" ) {}
 void runTest() { suite_UnitTest.testQueue(); }
} testDescription_UnitTest_testQueue;

static class TestDescription_UnitTest_testQueueVisited : public CxxTest::RealTestDescription {
public:
 TestDescription_UnitTest_testQueueVisited() : CxxTest::RealTestDescription( Tests_UnitTest, suiteDescription_UnitTest, 46, "testQueueVisited" ) {}
 void runTest() { suite_UnitTest.testQueueVisited(); }
} testDescription_UnitTest_testQueueVisited;

static class TestDescription_UnitTest_testLevelFromServer : public CxxTest::RealTestDescription {
public:
 TestDescription_UnitTest_testLevelFromServer() : CxxTest::RealTestDescription( Tests_UnitTest, suiteDescription_UnitTest, 84, "testLevelFromServer" ) {}
 void runTest() { suite_UnitTest.testLevelFromServer(); }
} testDescription_UnitTest_testLevelFromServer;

#include <cxxtest/Root.cpp>
#endif
