#ifndef __MY_MAP_TESTS__
#define __MY_MAP_TESTS__

#include <test_helpers.h>

class MapTests final : public UnitTests
{
protected:
    void RunTests() final
    {
        ADD_TEST(MapTests::singleElementTest);
        ADD_TEST(MapTests::fiveElementTest);
        ADD_TEST(MapTests::useCharKeys);
    }

private:
	static bool singleElementTest();
    static bool fiveElementTest();
    static bool useCharKeys();

    static Test_Registrar<MapTests> registrar;
};

#endif