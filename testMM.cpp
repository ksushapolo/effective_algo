#include <iostream>

#include "mm.h"
#include <string>

const int ELEMENTS_COUNT = 10;
const int BLOCK_SIZE = 10;

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

typedef lab618::CMemoryManager<TestStruct> MM;

static std::string makeRandomString(int minL = 1, int maxL = 10) {
	const char alphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz�����Ũ��������������������������������������������������������";
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

void TestMMFunction() {
	MM mm(BLOCK_SIZE);
	TestStruct* my_array[ELEMENTS_COUNT];
	for (int i = 0; i < ELEMENTS_COUNT; ++i) {
		my_array[i] = mm.newObject();
	}

	for (int i = 0; i < ELEMENTS_COUNT; ++i) {
		mm.deleteObject(my_array[i]);
	}
}

int main() {
	TestMMFunction();
	return 0;
}