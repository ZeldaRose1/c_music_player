#pragma once

#include "catch_amalgamated.hpp"
#include "catch_amalgamated.cpp"

int add(int a, int b){
    return a + b;
}

TEST_CASE( "addition is computed", "[addition]"){
    REQUIRE( add(1, 2) == 3 );
    REQUIRE( add(2, 2) == 4 );
    REQUIRE( add(3, 7) == 10 );
    REQUIRE( add(5, -5) == 0 );
}