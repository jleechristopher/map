#include "unittests.h"
#include "map.h"
#include <iostream>

Test_Registrar<MapTests> MapTests::registrar;

bool MapTests::singleElementTest() 
{
	MyMap<int, char> map;
	VERIFY_TRUE(map.set(1, 'a'));
	VERIFY_EQ(map.get(1), 'a');
	VERIFY_TRUE(map.set(1, 'b'));
	VERIFY_NOT_EQ(map.get(1), 'a');
	VERIFY_EQ(map.get(1), 'b');

	return true;
}

// forces remapping, verifies data is valid and unchanged after remapping
bool MapTests::fiveElementTest() 
{
	MyMap<int, char> map;
	VERIFY_TRUE(map.set(1, 'a'));
	VERIFY_TRUE(map.set(10, 'a'));
	VERIFY_TRUE(map.set(100, 'b'));
	VERIFY_EQ(map.get(1), map.get(10));
	VERIFY_EQ(map.get(100), 'b');
	VERIFY_TRUE(map.set(2, 'c'));
	VERIFY_TRUE(map.set(-1, 'd'));
	VERIFY_EQ(map.get(1), 'a');
	VERIFY_EQ(map.get(10), 'a');
	VERIFY_EQ(map.get(100), 'b');
	VERIFY_EQ(map.get(2), 'c');
	VERIFY_EQ(map.get(-1), 'd');

	return true;
}

bool MapTests::useCharKeys() 
{
	MyMap<char, int> map;
	VERIFY_TRUE(map.set('a', 1));
	VERIFY_EQ(map.get('a'), 1);
	VERIFY_TRUE(map.set('a', 3));
	VERIFY_NOT_EQ(map.get('a'), 1);
	VERIFY_EQ(map.get('a'), 3);

	return true;
}

bool MapTests::insertMany()
{
	MyMap<int, int> map;

	int numElements = 10000;
	for (int i = 0; i < numElements; ++i)
	{
		map.set(i, i);
	}

	int count = 0;
	for (int i = 0; i < numElements; ++i)
	{
		if (map.get(i) == i)
		{
			++count;
		}
	}
	VERIFY_EQ(count, numElements);

	return true; 
}

// forces multiple remappings with a single set()
bool MapTests::remappingTest()
{
	MyMap<int, int> map;
	VERIFY_TRUE(map.set(16, 1));
	VERIFY_TRUE(map.set(32, 1));
	VERIFY_TRUE(map.set(128, 1));
	VERIFY_EQ(map.get(16), 1);
	VERIFY_EQ(map.get(32), 1);
	VERIFY_EQ(map.get(128), 1);

	return true; 
}

// test O(1) access time
bool MapTests::timingTest()
{
	int numIterations = 1000;
	int numBins = 45; 
	double singleElementAvg = 0.0;
	Timer t;

	MyMap<int, int> map;
	VERIFY_TRUE(map.set(0, 1));
	for (int ii = 0; ii < numIterations; ++ii)
	{
		t.reset();
		map.get(0);
		singleElementAvg += t.elapsed();
	}
	singleElementAvg = singleElementAvg / numIterations;

	for (int ii = 1; ii < numIterations; ++ii)
	{
		map.set(ii, 1);
	}

	int count = 0;
	// since the worst case is checking every bin to get a value, time taken should be at most 
	// singleElementAvg * numBins. Since there should be some variance, we use numBins + 1
	for (int ii = 0; ii < numIterations; ++ii)
	{
		t.reset();
		map.get(ii);
		if (t.elapsed() <= (singleElementAvg * (numBins + 1)))
		{
			++count;
		}
	}

	VERIFY_EQ(numIterations, count);

	return true; 
}

bool MapTests::useFloatKeys()
{
	MyMap<float, int> map;
	float current = 1e-38;
	int ordersMagnitude = 77;
	for (int ii = 1; ii <= ordersMagnitude; ++ii)
	{
		map.set(current, ii);
		current = current * 10;
	}

	current = 1e-38;
	int count = 0; 
	for (int ii = 1; ii <= ordersMagnitude; ++ii)
	{
		if (map.get(current) == ii)
		{
			++count;
		}
		current = current * 10;
	}
	VERIFY_EQ(ordersMagnitude, count);

	return true;
}
