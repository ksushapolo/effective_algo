#ifndef MEMORY_MANAGER_HEAD_H_2021_02_18
#define MEMORY_MANAGER_HEAD_H_2021_02_18

namespace lab618 {
	template <class T>
	class CMemoryManager {
	private:
		struct block {
			// Массив данных блока
			T* pdata = nullptr;
			// Адрес следующего блока
			block *pnext = nullptr;
			// Первая свободная ячейка
			int firstFreeIndex = 0;
			// Число заполненных ячеек
			int usedCount = 0;

			block() = default;

			block(T* _pdata, block* _pnext, int _firstFreeIndex, int _usedCount) :
				pdata(_pdata),
				pnext(_pnext),
				firstFreeIndex(_firstFreeIndex),
				usedCount(_usedCount) {}
		};
	public:
		class CException {
		public:
			CException() = default;
		};

	public:

		CMemoryManager() = default;

		CMemoryManager(int _default_block_size, bool isDeleteElementsOnDestruct = false) : 
			m_isDeleteElementsOnDestruct(isDeleteElementsOnDestruct),
			m_blkSize(_default_block_size),
			m_pBlocks(nullptr),
			m_pCurrentBlk(nullptr) {}

		virtual ~CMemoryManager() {
			clear();
		}

		// Получить адрес нового элемента из менеджера
		T* newObject() {
			if (nullptr == m_pCurrentBlk) {
				m_pBlocks = m_pCurrentBlk = newBlock();
			}
			bool is_full_curr = (m_pCurrentBlk->firstFreeIndex == -1);
			if (is_full_curr) {
				block* curr = m_pBlocks;
				while (curr != nullptr) {
					bool is_full_curr = (curr->firstFreeIndex == -1);
					if (!is_full_curr) {
						break;
					}
					else {
						curr = curr->pnext;
					}
				}
				if (curr) {
					m_pCurrentBlk = curr;
				}
				else {
					block* empty_block = newBlock();
					m_pCurrentBlk->pnext = empty_block;
					m_pCurrentBlk = m_pCurrentBlk->pnext;
				}
			}
			T* free_pos = m_pCurrentBlk->pdata + m_pCurrentBlk->firstFreeIndex;
			m_pCurrentBlk->firstFreeIndex = *(reinterpret_cast<int*>(free_pos));
			m_pCurrentBlk->usedCount = m_pCurrentBlk->usedCount + 1;
			new(free_pos) T;
			return free_pos;

		}

		// Освободить элемент в менеджере
		bool deleteObject(T* p) {
			if (nullptr == m_pBlocks) {
				return false;
			}
			block* curr = m_pBlocks;
			while (curr != nullptr) {
				if (curr->pdata <= p && p <= curr->pdata + m_blkSize - 1) {
					break;
				}
				curr = curr->pnext;
			}

			if (nullptr == curr) {
				return false;
			}

			int pos = p - curr->pdata;
			p->~T();
			curr->firstFreeIndex = pos;
			curr->usedCount = curr->usedCount - 1;
			new(reinterpret_cast<int*>(p)) int(curr->firstFreeIndex);
			return true;
		}

		// Очистка данных, зависит от m_isDeleteElementsOnDestruct
		void clear() {
			if (nullptr == m_pBlocks) {
				return;
			}
			block* curr = m_pBlocks;
			if (m_isDeleteElementsOnDestruct) {
				m_pBlocks = nullptr;
				m_pCurrentBlk = nullptr;

				bool* is_taken = new bool[m_blkSize];

				while (curr != nullptr) {
					block* next = curr->pnext;
					deleteBlock(curr, is_taken);
					curr = next;
				}

				delete[] is_taken;
				return;
			}
			if (!allBlocksAreEmpty()) {
				throw CException();
			}
			
			while (curr != nullptr) {
				block* next = curr->pnext;
				T* data_array = curr->pdata;
				char* char_array = reinterpret_cast<char*>(data_array);
				delete[] char_array;
				delete curr;
				curr = next;
			}
		}
	private:

		bool allBlocksAreEmpty() {
			block* curr = m_pBlocks;
			while (curr != nullptr) {
				if (curr->usedCount == 0) {
					curr = curr->pnext;
				}
				else {
					return false;
				}
			}
			return true;
		}

		// Создать новый блок данных. применяется в newObject
		block* newBlock() {
			int num_of_chars = m_blkSize * T_size;
			char* char_array = new char[num_of_chars];
			T* data_array = reinterpret_cast<T*>(char_array);
			block* new_block = new block(data_array, nullptr, 0, 0);
			if (nullptr == new_block) {
				throw CException();
			}
			for (int i = 0; i < m_blkSize; ++i) {
				T* curr = data_array + i;
				int* int_curr = reinterpret_cast<int*> (curr);
				int free_pos = -1;
				if (i + 1 != m_blkSize) {
					free_pos = i + 1;
				}
				new(int_curr) int(free_pos);
			}
			return new_block;
		}

		// Освободить память блока данных. Применяется в clear
		void deleteBlock(block *p, bool *is_taken) {
			if (!m_isDeleteElementsOnDestruct) {
				T* data_array = p->pdata;
				char* char_array = reinterpret_cast<char*>(data_array);
				delete[] char_array;
				delete p;
				return;
			}

			for (int i = 0; i < m_blkSize; ++i) {
				is_taken[i] = true;
			}

			T* data_array = p->pdata;
			int free_ind = p->firstFreeIndex;
			while (free_ind != -1) {
				is_taken[free_ind] = false;
				free_ind = *(reinterpret_cast<int*>(data_array + free_ind));
			}

			for (int i = 0; i < m_blkSize; ++i) {
				if (is_taken[i]) {
					(data_array + i)->~T();
				}
			}
			char* char_array = reinterpret_cast<char*>(data_array);
			delete[] char_array;
			delete p;
		}

		int T_size = sizeof(T);
		// Размер блока
		int m_blkSize = 0;
		// Начало списка блоков
		block* m_pBlocks = nullptr;
		// Текущий блок
		block *m_pCurrentBlk = nullptr;
		// Удалять ли элементы при освобождении
		bool m_isDeleteElementsOnDestruct = false;
	};
}; 

#endif
