//#define CATCH_CONFIG_MAIN
//#include <catch2/catch.hpp>

#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>

int main(int argc, char* argv[])
{
    return Catch::Session().run( argc, argv );
}
