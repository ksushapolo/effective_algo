#include <iostream>

#include "hash.h"
#include <string>
#include <vector>

const int ELEMENTS_COUNT = 10;

struct TestStruct {
	std::string key;
	std::string value1;
	std::string value2;
	TestStruct& operator=(TestStruct& src) {
		if (this == &src) {
			return *this;
		}
		key = src.key;
		value1 = src.value1;
		value2 = src.value2;
		return *this;
	}
};

unsigned int HashFunc(const TestStruct* pElement) {
	std::string key = pElement->key;
	unsigned int hash = 0;
	unsigned int power = 1;
	const int base = 47;
	for (size_t i = 0; i < key.length(); ++i) {
		hash += (key[i] - 'a' + 1) * power;
		power *= base;
	}
	return hash;
}

int Compare(const TestStruct* first, const TestStruct* second) {
	std::string first_key = first->key;
	std::string second_key = second->key;
	if (first_key == second_key) {
		return 0;
	}
	if (first_key < second_key) {
		return 1;
	}
	return -1;
}

typedef lab618::CHash<TestStruct, HashFunc, Compare> CHash;

static std::string makeRandomString(int minL = 1, int maxL = 10) {
	const char alphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	const int alphabet_length = sizeof(alphabet);
	int length = rand() % maxL + minL;
	std::string s;
	s.reserve(length);
	for (int i = 0; i < length; ++i) {
		char temp = alphabet[rand() % (alphabet_length)];
		s += temp;
	}
	return s;
}

static void generate(TestStruct *pts) {
	pts->key = makeRandomString();
	pts->value1 = makeRandomString();
	pts->value2 = makeRandomString();
}

void TestHashFunction() {
	const int BLOCK_SIZE = 10;
	TestStruct* my_array = new TestStruct[ELEMENTS_COUNT];
	CHash hash(47, BLOCK_SIZE);

	for (int i = 0; i < ELEMENTS_COUNT; ++i) {
		generate(&my_array[i]);
		hash.add(&my_array[i]);
	}

	hash.update(&my_array[0]);

	for (int i = 0; i < ELEMENTS_COUNT; ++i) {
		hash.remove(my_array[i]);
	}

	delete[] my_array;
}

int main() {
	TestHashFunction();
	return 0;
}