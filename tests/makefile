all: c++20 c++17 c++14 c++11

CXX = g++
CXX_FLAGS = -Wshadow -Wall

CXX_20 = -std=c++2a
CXX_17 = -std=c++17
CXX_14 = -std=c++14
CXX_11 = -std=c++11

.PHONY: c++20
c++20:
	$(CXX) $(CXX_FLAGS) $(CXX_20) c++20/all_tests.cpp -o c++20-all

.PHONY: c++17
c++17:
	$(CXX) $(CXX_FLAGS) $(CXX_17) c++17/all_tests.cpp -o c++17-all

.PHONY: c++14
c++14:
	$(CXX) $(CXX_FLAGS) $(CXX_14) c++14/all_tests.cpp -o c++14-all

.PHONY: c++11
c++11:
	$(CXX) $(CXX_FLAGS) $(CXX_11) c++11/all_tests.cpp -o c++11-all
