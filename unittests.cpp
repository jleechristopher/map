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
	VERIFY_TRUE(map.set(3, 'd'));
	VERIFY_EQ(map.get(1), 'a');
	VERIFY_EQ(map.get(10), 'a');
	VERIFY_EQ(map.get(100), 'b');
	VERIFY_EQ(map.get(2), 'c');
	VERIFY_EQ(map.get(3), 'd');

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
