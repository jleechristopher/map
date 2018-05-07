#ifndef __MY_MAP_TESTS__
#define __MY_MAP_TESTS__

#include <test_helpers.h>
#include <chrono>

class Timer
{
private:
    using clock_t = std::chrono::high_resolution_clock;
    using second_t = std::chrono::duration<double, std::ratio<1> >;

    std::chrono::time_point<clock_t> m_start; 

public:
    Timer() : m_start(clock_t::now())
    {
    }

    void reset()
    {
        m_start = clock_t::now();
    }

    double elapsed() const
    {
        return std::chrono::duration_cast<second_t>(clock_t::now() - m_start).count();
    }

    void printElapsed() const
    {
        auto timeTaken = std::chrono::duration_cast<second_t>(clock_t::now() - m_start).count();
        std::cout << "Time elapsed: " << timeTaken << "\n";
    }
};

class MapTests final : public UnitTests
{
protected:
    void RunTests() final
    {
        ADD_TEST(MapTests::singleElementTest);
        ADD_TEST(MapTests::fiveElementTest);
        ADD_TEST(MapTests::useCharKeys);
        ADD_TEST(MapTests::insertMany);
        ADD_TEST(MapTests::remappingTest);
        ADD_TEST(MapTests::timingTest);
        ADD_TEST(MapTests::useFloatKeys);
    }

private:
	static bool singleElementTest();
    static bool fiveElementTest();
    static bool useCharKeys();
    static bool insertMany();
    static bool remappingTest();
    static bool timingTest();
    static bool useFloatKeys();

    static Test_Registrar<MapTests> registrar;
};

#endif