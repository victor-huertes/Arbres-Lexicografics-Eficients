#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <iostream>
#include <string>
#include <functional>
#include <vector>
#include <chrono>

// Colors per als missatges de test
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"

// Macros per als asserts
#define ASSERT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            std::cout << COLOR_RED << "✗ FAIL" << COLOR_RESET \
                      << " at " << __FILE__ << ":" << __LINE__ \
                      << " - Expected true, got false" << std::endl; \
            TestFramework::getInstance().recordFailure(); \
            return false; \
        } \
    } while(0)

#define ASSERT_FALSE(condition) \
    do { \
        if (condition) { \
            std::cout << COLOR_RED << "✗ FAIL" << COLOR_RESET \
                      << " at " << __FILE__ << ":" << __LINE__ \
                      << " - Expected false, got true" << std::endl; \
            TestFramework::getInstance().recordFailure(); \
            return false; \
        } \
    } while(0)

#define ASSERT_EQ(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            std::cout << COLOR_RED << "✗ FAIL" << COLOR_RESET \
                      << " at " << __FILE__ << ":" << __LINE__ \
                      << " - Expected: " << (expected) \
                      << ", Got: " << (actual) << std::endl; \
            TestFramework::getInstance().recordFailure(); \
            return false; \
        } \
    } while(0)

#define TEST(test_name) \
    bool test_name(); \
    bool test_##test_name##_runner() { \
        std::cout << COLOR_CYAN << "Running " << #test_name << "..." << COLOR_RESET << std::endl; \
        auto start = std::chrono::high_resolution_clock::now(); \
        bool result = test_name(); \
        auto end = std::chrono::high_resolution_clock::now(); \
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start); \
        if (result) { \
            std::cout << COLOR_GREEN << "✓ PASS" << COLOR_RESET \
                      << " (" << duration.count() << "μs)" << std::endl; \
            TestFramework::getInstance().recordSuccess(); \
        } else { \
            TestFramework::getInstance().recordFailure(); \
        } \
        std::cout << std::endl; \
        return result; \
    } \
    bool test_name()

#define RUN_TEST(test_name) \
    TestFramework::getInstance().addTest(#test_name, test_##test_name##_runner)

/**
 * @brief Framework simple per executar tests
 */
class TestFramework {
private:
    std::vector<std::pair<std::string, std::function<bool()>>> tests;
    int passed = 0;
    int failed = 0;
    
    TestFramework() = default;

public:
    static TestFramework& getInstance() {
        static TestFramework instance;
        return instance;
    }
    
    void addTest(const std::string& name, std::function<bool()> test) {
        tests.emplace_back(name, test);
    }
    
    void recordSuccess() { passed++; }
    void recordFailure() { failed++; }
    
    int runAllTests() {
        std::cout << COLOR_MAGENTA << "===========================================" << COLOR_RESET << std::endl;
        std::cout << COLOR_MAGENTA << "           TRIE TEST SUITE" << COLOR_RESET << std::endl;
        std::cout << COLOR_MAGENTA << "===========================================" << COLOR_RESET << std::endl;
        std::cout << std::endl;
        
        auto start_time = std::chrono::high_resolution_clock::now();
        
        for (const auto& test : tests) {
            test.second();
        }
        
        auto end_time = std::chrono::high_resolution_clock::now();
        auto total_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        
        std::cout << COLOR_MAGENTA << "===========================================" << COLOR_RESET << std::endl;
        std::cout << COLOR_MAGENTA << "              TEST SUMMARY" << COLOR_RESET << std::endl;
        std::cout << COLOR_MAGENTA << "===========================================" << COLOR_RESET << std::endl;
        
        std::cout << "Total tests: " << COLOR_BLUE << (passed + failed) << COLOR_RESET << std::endl;
        std::cout << "Passed: " << COLOR_GREEN << passed << COLOR_RESET << std::endl;
        std::cout << "Failed: " << COLOR_RED << failed << COLOR_RESET << std::endl;
        std::cout << "Success rate: " << COLOR_YELLOW 
                  << (passed * 100.0 / (passed + failed)) << "%" << COLOR_RESET << std::endl;
        std::cout << "Total time: " << COLOR_CYAN << total_duration.count() << "ms" << COLOR_RESET << std::endl;
        
        if (failed == 0) {
            std::cout << std::endl << COLOR_GREEN << "🎉 ALL TESTS PASSED!" << COLOR_RESET << std::endl;
        } else {
            std::cout << std::endl << COLOR_RED << "❌ SOME TESTS FAILED!" << COLOR_RESET << std::endl;
        }
        
        return failed == 0 ? 0 : 1;
    }
};

#endif // TEST_FRAMEWORK_H