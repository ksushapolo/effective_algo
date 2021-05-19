#include "timer.h"
#include "sort.h"
#include "avltree.h"
#include "hash.h"
#include "mm.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>


struct TestStruct {
	std::string key1;
	std::string key2;
	std::string value;
	TestStruct& operator=(TestStruct& src) {
		if (this == &src) {
			return *this;
		}
		key1 = src.key1;
		key2 = src.key2;
		value = src.value;
		return *this;
	}
};


static std::string makeRandomString(int minL = 7, int maxL = 20) {
	const char alphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";//ÀÁÂÃÄÅ¨ÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞßàáâãäå¸æçèéêëìíîïğñòóôõö÷øùúûüışÿ";
	const int alphabet_length = sizeof(alphabet);
	int length = rand() % maxL + minL;
	std::string s;
	s.reserve(length);
	for (int i = 0; i < length; ++i) {
		char temp = alphabet[rand() % (alphabet_length)];
		s += temp;
	}
	while (s == "") {
		s = makeRandomString();
	}
	return s;
}

static void generate(TestStruct *pts) {
	pts->key1 = makeRandomString();
	pts->key2 = makeRandomString();
	pts->value = makeRandomString();
}

unsigned int HashFunc(const TestStruct* pElement) {
	std::string key = pElement->key1 + pElement->key2;
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
	std::string first_key1 = first->key1;
	std::string second_key1 = second->key1;
	std::string first_key2 = first->key2;
	std::string second_key2 = second->key2;
	if (first_key1 == second_key1) {
		if (first_key2 == second_key2) {
			return 0;
		}
		if (first_key2 < second_key2) {
			return 1;
		}
		return -1;
	}
	if (first_key1 < second_key1) {
		return 1;
	}
	return -1;
}

TestStruct* binSearch(TestStruct** array_of_pointers, const TestStruct& element, const size_t& length) {
	int left = 0;
	int right = length - 1;
	int search_idx = -1; 
	while (left <= right) {
		int mid = (left + right) / 2;
		int comp_result = Compare(array_of_pointers[mid], &element);
		if (0 == comp_result) {
			search_idx = mid;
			break;
		}
		if (comp_result < 0) {
			right = mid - 1;
		} else {
			left = mid + 1;
		}
	}
	if (-1 == search_idx) {
		return nullptr;
	}
	return array_of_pointers[search_idx];
}

typedef lab618::CHash<TestStruct, HashFunc, Compare> CHash;
typedef lab618::CAVLTree<TestStruct, Compare> CAVL;

void timerSort(CHash& hash, CAVL& tree, TestStruct* array_of_elements, TestStruct** array_of_pointers, const size_t& N,
			   std::vector<double>& sort_time, std::vector<double>& add_hash_time, std::vector<double>& add_tree_time) {

	Timer timer_sort;
	templates::mergeSort(array_of_pointers, N, Compare);
	sort_time.push_back(timer_sort.elapsed());

	Timer timer_hash;
	for (int i = 0; i < N; ++i) {
		hash.add(&array_of_elements[i]);
	}
	add_hash_time.push_back(timer_hash.elapsed());

	Timer timer_tree;
	for (int i = 0; i < N; ++i) {
		tree.add(&array_of_elements[i]);
	}
	add_tree_time.push_back(timer_tree.elapsed());
}

void timerFind(CHash& hash, CAVL& tree, TestStruct* array_of_elements, TestStruct** array_of_pointers, const size_t& N,
	std::vector<double>& find_time, std::vector<double>& find_hash_time, std::vector<double>& find_tree_time) {

	Timer timer_find;
	for (int i = 0; i < N; ++i) {
		binSearch(array_of_pointers, array_of_elements[i], N - 1);
	}
	find_time.push_back(timer_find.elapsed());

	Timer timer_hash;
	for (int i = 0; i < N; ++i) {
		hash.find(array_of_elements[i]);
	}
	find_hash_time.push_back(timer_hash.elapsed());

	Timer timer_tree;
	for (int i = 0; i < N; ++i) {
		tree.find(array_of_elements[i]);
	}
	find_tree_time.push_back(timer_tree.elapsed());
}

void timerDelete(CHash& hash, CAVL& tree, TestStruct* array_of_elements, const size_t& N,
	std::vector<double>& delete_time, std::vector<double>& delete_hash_time, std::vector<double>& delete_tree_time) {

	Timer timer_delete;
	delete[] array_of_elements;
	delete_time.push_back(timer_delete.elapsed());

	Timer timer_hash;
	hash.clear();
	delete_hash_time.push_back(timer_hash.elapsed());

	Timer timer_tree;
	tree.clear();
	delete_tree_time.push_back(timer_tree.elapsed());
}

void timerRemove(CHash& hash, CAVL& tree, TestStruct* array_of_elements, const size_t& N,
	             std::vector<double>& remove_hash_time, std::vector<double>& remove_tree_time) {

	Timer timer_hash;
	for (int i = 0; i < N; ++i) {
		hash.remove(array_of_elements[i]);
	}
	remove_hash_time.push_back(timer_hash.elapsed());

	Timer timer_tree;
	for (int i = 0; i < N; ++i) {
		tree.remove(array_of_elements[i]);
	}
	remove_tree_time.push_back(timer_tree.elapsed());
}


int main() {
	setlocale(LC_ALL, "Russian");
	const int BLOCK_SIZE = 10;
	std::vector<double> sort_time;
	std::vector<double> add_hash_time;
	std::vector<double> add_tree_time;
	std::vector<double> find_time;
	std::vector<double> find_hash_time;
	std::vector<double> find_tree_time;
	std::vector<double> delete_time;
	std::vector<double> delete_hash_time;
	std::vector<double> delete_tree_time;
	std::vector<double> remove_hash_time;
	std::vector<double> remove_tree_time;

	std::vector<double> find_time_2;
	std::vector<double> find_hash_time_2;
	std::vector<double> find_tree_time_2;

	std::vector<double> trash;

	int min = 100;
	int max = 10000;
	int shift = 990;

	for (int N = min; N <= max; N += shift) {
		std::cout << N << std::endl;
		CHash hash(47, BLOCK_SIZE);
		CAVL tree(BLOCK_SIZE);
		TestStruct* array_of_elements = new TestStruct[N];
		TestStruct** array_of_pointers = new TestStruct*[N];
		for (int i = 0; i < N; ++i) {
			generate(&array_of_elements[i]);
		}

		for (int i = 0; i < N; ++i) {
			array_of_pointers[i] = &(array_of_elements[i]);
		}

		TestStruct* array_of_elements_2 = new TestStruct[2 * N];
		TestStruct** array_of_pointers_2 = new TestStruct*[2 * N];
		for (int i = 0; i < 2 * N; ++i) {
			generate(&array_of_elements_2[i]);
		}

		for (int i = 0; i < 2 * N; ++i) {
			array_of_pointers_2[i] = &(array_of_elements_2[i]);
		}

		timerSort(hash, tree, array_of_elements, array_of_pointers, N, sort_time, add_hash_time, add_tree_time);
		timerFind(hash, tree, array_of_elements, array_of_pointers, N, find_time, find_hash_time, find_tree_time);
		timerDelete(hash, tree, array_of_elements, N, delete_time, delete_hash_time, delete_tree_time);

		CHash hash1(47, BLOCK_SIZE);
		CAVL tree1(BLOCK_SIZE);
		TestStruct* array_of_elements1 = new TestStruct[N];
		TestStruct** array_of_pointers1 = new TestStruct*[N];
		for (int i = 0; i < N; ++i) {
			generate(&array_of_elements1[i]);
		}

		for (int i = 0; i < N; ++i) {
			array_of_pointers1[i] = &(array_of_elements1[i]);
		}
		timerSort(hash1, tree1, array_of_elements1, array_of_pointers1, N, trash, trash, trash);
		timerRemove(hash1, tree1, array_of_elements1, N, remove_hash_time, remove_tree_time);


		CHash hash_2(47, BLOCK_SIZE);
		CAVL tree_2(BLOCK_SIZE);
		timerSort(hash_2, tree_2, array_of_elements_2, array_of_pointers_2, N, trash, trash, trash);
		timerFind(hash_2, tree_2, array_of_elements_2, array_of_pointers_2, N, find_time_2, find_hash_time_2, find_tree_time_2);
		timerDelete(hash_2, tree_2, array_of_elements_2, N, trash, trash, trash);

		delete[] array_of_elements1;
		delete[] array_of_pointers1;
		delete[] array_of_pointers;
		delete[] array_of_pointers_2;
	}

	int num_of_points = (max - min) / shift;

	std::ofstream out;
	out.open("sort.txt", std::fstream::out);
	for (int i = 0; i < num_of_points; ++i) {
		out << sort_time[i] << ",";
	}
	for (int i = 0; i < num_of_points; ++i) {
		out << add_hash_time[i] << ",";
	}
	for (int i = 0; i < num_of_points; ++i) {
		out << add_tree_time[i] << ",";
	}
	out.close();

	out.open("find.txt", std::fstream::out);
	for (int i = 0; i < num_of_points; ++i) {
		out << find_time[i] << ",";
	}
	for (int i = 0; i < num_of_points; ++i) {
		out << find_hash_time[i] << ",";
	}
	for (int i = 0; i < num_of_points; ++i) {
		out << find_tree_time[i] << ",";
	}
	out.close();

	out.open("delete.txt", std::fstream::out);
	for (int i = 0; i < num_of_points; ++i) {
		out << delete_time[i] << ",";
	}
	for (int i = 0; i < num_of_points; ++i) {
		out << delete_hash_time[i] << ",";
	}
	for (int i = 0; i < num_of_points; ++i) {
		out << delete_tree_time[i] << ",";
	}
	out.close();

	out.open("remove.txt", std::fstream::out);
	for (int i = 0; i < num_of_points; ++i) {
		out << remove_hash_time[i] << ",";
	}
	for (int i = 0; i < num_of_points; ++i) {
		out << remove_tree_time[i] << ",";
	}
	out.close();

	out.open("find2.txt", std::fstream::out);
	for (int i = 0; i < num_of_points; ++i) {
		out << find_time_2[i] << ",";
	}
	for (int i = 0; i < num_of_points; ++i) {
		out << find_hash_time_2[i] << ",";
	}
	for (int i = 0; i < num_of_points; ++i) {
		out << find_tree_time_2[i] << ",";
	}
	out.close();
	return 0;
}