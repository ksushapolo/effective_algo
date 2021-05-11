#include "sort.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

const int ELEMENTS_COUNT = 10;

struct TestStruct {
	std::string key = "";
	std::string value1 = "";
	std::string value2 = "";

	TestStruct() = default;

	bool operator==(const TestStruct other) {
		if (key == other.key && value1 == other.value1 && value2 == other.value2) {
			return true;
		}
		return false;
	}
};

static std::string makeRandomString(int minL = 1, int maxL = 10) {
	const char alphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	//ÀÁÂÃÄÅ¨ÆÇÈÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞßàáâãäå¸æçèêëìíîïğñòóôõö÷øùúûüışÿ
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

bool stdCompareFunc(const TestStruct& first, const TestStruct& second) {
	return first.key < second.key;
}


int pCompareFunc(const TestStruct* first, const TestStruct* second) {
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


void TestSortFunction() {

	std::vector<TestStruct> my_array;
	std::vector<TestStruct> my_array_copy;
	for (int i = 0; i < ELEMENTS_COUNT; ++i) {
		TestStruct ts;
		generate(&ts);
		my_array.push_back(ts);
		my_array_copy.push_back(ts);
	}
	std::sort(my_array_copy.begin(), my_array_copy.end(), stdCompareFunc);
	TestStruct** upd_to_ms = new TestStruct*[ELEMENTS_COUNT];
	for (int i = 0; i < ELEMENTS_COUNT; ++i) {
		upd_to_ms[i] = &(my_array[i]);
	}
	templates::mergeSort(upd_to_ms, ELEMENTS_COUNT, pCompareFunc);
	bool is_the_same = true;
	for (int i = 0; i < ELEMENTS_COUNT; ++i) {
		if (*upd_to_ms[i] == my_array_copy[i]) {
			is_the_same = true;
		} else {
			is_the_same = false;
			break;
		}
	}
	_ASSERT(is_the_same && "MergeSort doesn't work(");
}

int main() {
	TestSortFunction();
	system("pause");
	return 0;
}
