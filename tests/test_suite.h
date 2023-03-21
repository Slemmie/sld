// usage:
// create a cluster of tests: TEST_REGISTER_CLUSTER(<CLUSTER_NAME>)
// create a test case: TEST_CASE(<CLUSTER_NAME>, <TEST_NAME>, "<DESCRIPTION>") { ... }
// create an assertion that returns false if it fails: TEST_ASSERT(<CONDITION>)
//
// note: the test case function must return a bool (inbuilt ways to do this are: TEST_PASS and TEST_FAIL)
//
// in one source file, use (implements main()): TEST_SUITE_MAIN

#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <functional>
#include <vector>

#define _TEST_PASS_FORMAT "\033[1;32mPASS\033[0m"
#define _TEST_FAIL_FORMAT "\033[1;31mFAIL\033[0m"

class _TEST_SUITE {
public:
	static size_t _test_count;
	static size_t _passed_count;
	static std::vector <std::function <bool ()>> CLUSTER_FUNCTIONS;
	static bool run() {
		std::cout << "RUNNING ALL TESTS:\n" << std::endl;
		bool _all_passed = true;
		_passed_count = 0;
		std::chrono::time_point <std::chrono::high_resolution_clock> _start_time = std::chrono::system_clock::now();
		for (auto& _cluster_function : CLUSTER_FUNCTIONS) {
			_all_passed &= _cluster_function();
			std::cout << std::endl;
		}
		std::chrono::time_point <std::chrono::high_resolution_clock> _end_time = std::chrono::system_clock::now();
		size_t _us = std::chrono::duration_cast <std::chrono::microseconds> (_end_time - _start_time).count();
		std::stringstream _sstream;
		_sstream << std::fixed << std::setprecision(3) << (long double) _us / 1000.0L;
		std::cout << "[ " << (_all_passed ? _TEST_PASS_FORMAT : _TEST_FAIL_FORMAT) << " ] ALL TESTS { "
		<< _TEST_PASS_FORMAT << ": " << _passed_count << ", "
		<< _TEST_FAIL_FORMAT << ": " << (_test_count - _passed_count ? "\033[31m" : "\033[0m") << _test_count - _passed_count << "\033[0m, "
		<< "\033[1mTOTAL\033[0m: " << _test_count << " } completed in " << _sstream.str() << " ms" << std::endl;
		return _all_passed;
	}
};

#ifdef TEST_IMPLEMENTATION
size_t _TEST_SUITE::_test_count = 0;
size_t _TEST_SUITE::_passed_count = 0;
std::vector <std::function <bool ()>> _TEST_SUITE::CLUSTER_FUNCTIONS;
int main() {
	return _TEST_SUITE::run();
}
#endif

#define _TEST_REGISTER(_CLUSTER_NAME, _FUNCTION) \
	class _TEST_REGISTER##_CLUSTER_NAME { \
	public: \
		_TEST_REGISTER##_CLUSTER_NAME() { \
			_TEST_SUITE::CLUSTER_FUNCTIONS.push_back([] () -> bool { \
				std::cout << #_CLUSTER_NAME << ":" << std::endl; \
				std::function <bool ()> _function = _FUNCTION; \
				bool _passed = _function(); \
				std::cout << "[ " << (_passed ? _TEST_PASS_FORMAT : _TEST_FAIL_FORMAT) << " ] " << #_CLUSTER_NAME << std::endl; \
				return _passed; \
			}); \
		}; \
	}; \
	static _TEST_REGISTER##_CLUSTER_NAME _TEST_REGISTER##_CLUSTER_NAME##_instance = _TEST_REGISTER##_CLUSTER_NAME();

#define TEST_REGISTER_CLUSTER(_CLUSTER_NAME) \
	static std::vector <std::function <bool ()>> _CLUSTER_NAME##_test_cases; \
	class _CLUSTER_NAME { \
	public: \
		static void _add_test_case(std::function <bool ()> _test_case) { \
			_CLUSTER_NAME##_test_cases.push_back(_test_case); \
		} \
	}; \
	_TEST_REGISTER(_CLUSTER_NAME, [] () -> bool { \
		bool _all_passed = true; \
		for (auto& _test_case : _CLUSTER_NAME##_test_cases) { \
			_all_passed &= _test_case(); \
		} \
		return _all_passed; \
	});

#define TEST_CASE(_CLUSTER_NAME, _TEST_NAME, _DESCRIPTION) \
	namespace _CLUSTER_NAME##_TEST_NAME { \
		bool _TEST_NAME(); \
		class TEST_CASE##_CLUSTER_NAME##_TEST_NAME { \
		public: \
			TEST_CASE##_CLUSTER_NAME##_TEST_NAME() { \
				_CLUSTER_NAME::_add_test_case([] () -> bool { \
					_TEST_SUITE::_test_count++; \
					std::string _message = "  [ ???? ] " + std::string(#_TEST_NAME) + (std::string(_DESCRIPTION).empty() ? "" : " '" + std::string(_DESCRIPTION) + "'"); \
					std::cout << _message << std::flush; \
					bool _passed = _TEST_NAME(); \
					_TEST_SUITE::_passed_count += _passed; \
					std::cout << "\r" << _message.replace(4, 4, _passed ? _TEST_PASS_FORMAT : _TEST_FAIL_FORMAT) << std::endl; \
					return _passed; \
				}); \
			} \
		}; \
		static TEST_CASE##_CLUSTER_NAME##_TEST_NAME TEST_CASE##_CLUSTER_NAME##_TEST_NAME##_instance = TEST_CASE##_CLUSTER_NAME##_TEST_NAME(); \
	} \
	bool _CLUSTER_NAME##_TEST_NAME::_TEST_NAME()

#define TEST_PASS return true
#define TEST_FAIL return false

#define TEST_ASSERT(_EXPR) { if (!(_EXPR)) { TEST_FAIL; } }
