#ifndef SORT_HEAD_H_2021_02_25
#define SORT_HEAD_H_2021_02_25

namespace templates {
	/** Определение типа функции сравнения элементов
	\return
	\li > 0  - pElem1 меньше pElem2
	\li 0   - элементы pElem1 и pElem2 равны
	\li < 0 - pElem1 больше pElem2
	*/

	typedef int (CompareSortType)(const void *pElem1, const void *pElem2);

	void mergeTwoParts(void **ppArray_1, int length_1, void **ppArray_2, int length_2,
		CompareSortType pCompareFunc, void **result) {

		void **temp_result = new void*[length_1 + length_2];

		int iter_1 = 0;
		int iter_2 = 0;
		int ppIter = 0;

		while (iter_1 < length_1 && iter_2 < length_2) {
			int comp_result = pCompareFunc(ppArray_1[iter_1], ppArray_2[iter_2]);
			if (comp_result > 0) {
				temp_result[ppIter] = ppArray_1[iter_1];
				++iter_1;
			}
			else {
				temp_result[ppIter] = ppArray_2[iter_2];
				++iter_2;
			}
			++ppIter;
		}

		while (iter_1 < length_1) {
			temp_result[ppIter] = ppArray_1[iter_1];
			++iter_1;
			++ppIter;
		}

		while (iter_2 < length_2) {
			temp_result[ppIter] = ppArray_2[iter_2];
			++iter_2;
			++ppIter;
		}

		for (int i = 0; i < length_1 + length_2; ++i) {
			result[i] = temp_result[i];
		}

		delete[] temp_result;
	}

	void recursiveStep(void **ppArray, int length, CompareSortType pCompareFunc, void **result) {
		if (length == 1) {
			result[0] = ppArray[0];
			return;
		}
		recursiveStep(ppArray, length / 2, pCompareFunc, result);
		recursiveStep(ppArray + length / 2, length - length / 2, pCompareFunc, result + length / 2);
		mergeTwoParts(result, length / 2, result + length / 2, length - length / 2, pCompareFunc, result);
	}

	void mergeSort(void **ppArray, int length, CompareSortType pCompareFunc) {
		void **result = new void*[length];
		recursiveStep(ppArray, length, pCompareFunc, result);
		for (int i = 0; i < length; ++i) {
			ppArray[i] = result[i];
		}
		delete[] result;
	}

	template <class T>
	void mergeSort(T **ppArray, int length, int (pCompareFunc)(const T *pElem1, const T *pElem2))
	{
		mergeSort(reinterpret_cast<void**>(ppArray), length, (CompareSortType*)pCompareFunc);
	}

}; // namespace templates

#endif // #define SORT_HEAD_H_2021_02_25

