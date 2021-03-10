#include <iostream>

#include "list.h"
#include <string>

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

typedef lab618::CSingleLinkedList<TestStruct> TestSingleList;
typedef lab618::CDualLinkedList<TestStruct> TestList;

static std::string makeRandomString(int minL = 7, int maxL = 14) {
	int length = rand() % maxL + minL;
	std::string s;
	s.reserve(length);
	// сделать случайную строку
	s += "tmp";
	return s;
}

static void generate(TestStruct *pts) {
	pts->key = makeRandomString();
	pts->value1 = makeRandomString();
	pts->value2 = makeRandomString();
}

void TestListFunction() {
	// тест односвязного списка

	TestSingleList single_list;
	for (int i = 0; i < ELEMENTS_COUNT; ++i) {
		TestStruct ts;
		generate(&ts);
		single_list.pushBack(ts);
	}

	_ASSERT(single_list.getSize() == ELEMENTS_COUNT);
	
	for (TestSingleList::CIterator it = single_list.begin(); it.isValid(); ++it) {
		it.getLeaf();
		TestStruct ts = *it;
		try {
			single_list.erase(it);
		}
		catch (...) {
			std::cout << "List is empty!";
		}
	}
	
	_ASSERT(single_list.getSize() == 0);

	/// тест двусвязного списка

	//TestList list;
	//for (int i = 0; i < ELEMENTS_COUNT; ++i) {
	//	TestStruct ts;
	//	generate(&ts);
	//	list.pushBack(ts);
	//}
	//
	//_ASSERT(list.getSize() == ELEMENTS_COUNT);
	//
	//for (TestList::CIterator it = list.begin(); it.isValid(); ++it) {
	//	it.getLeaf();
	//	TestStruct ts = *it;
	//	list.erase(it);
	//}
}

int main() {
	TestListFunction();
}
