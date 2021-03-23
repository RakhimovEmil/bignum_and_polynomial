all: smoke

smoke_test_1: smoke_test.cpp polynomial_test.cpp bignum.hpp
	$(CXX) -g -Wall -Wextra -std=c++17 -o smoke_test smoke_test.cpp

smoke_test_2: polynomial_test.cpp bignum.hpp
	$(CXX) -g -Wall -Wextra -std=c++17 -o polynomial_test polynomial_test.cpp

smoke: smoke_test_1 smoke_test_2
	./smoke_test
	./polynomial_test
