#ifndef TEMPLATES_LIST_2021_02_11
#define TEMPLATES_LIST_2021_02_11

#include <algorithm>

namespace lab618 {
	template<class T>
	class CSingleLinkedList {
	private:
		struct leaf {
			T data;
			leaf* pnext;

			leaf() = default;

			leaf(T& _data, leaf* _pnext) {
				data = _data;
				pnext = _pnext;
			}
		};
	public:
		class CIterator {
		public:
			CIterator() {
				m_pBegin = nullptr;
				m_pCurrent = nullptr;
			}

			CIterator(leaf* p) {
				m_pBegin = p;
				m_pCurrent = p;
			}

			CIterator(const CIterator &src) {
				m_pBegin = src.m_pBegin;
				m_pCurrent = src.m_pCurrent;
			}

			~CIterator() {}

			CIterator& operator = (const CIterator&  src) {
				if (this == &src) {
					return *this;
				}
				m_pBegin = src.m_pBegin;
				m_pCurrent = src.m_pCurrent;
				return *this;
			}

			bool operator != (const CIterator&  it) const {
				return it.m_pBegin == m_pBegin && it.m_pCurrent == m_pCurrent;
			}

			void operator++() {
				if (isValid()) {
					m_pCurrent = m_pCurrent->pnext;
				} else {
					throw "Out of range!";
				}
			}

			T& getData() {
				return m_pCurrent->data;
			}

			T& operator* () {
				return m_pCurrent->data; // чем отличается от getData?
			}

			leaf* getLeaf() {
				return m_pCurrent;
			}

			void setLeaf(leaf* p) {
				if (nullptr == m_pBegin) {
					m_pBegin = p;
				}
				m_pCurrent = p;
			}

			void setLeafPreBegin(leaf* p) {
				m_pBegin = p;
				m_pCurrent = p;
			}

			bool isValid() {
				return m_pCurrent != nullptr;
			}

		private:
			//храним голову списка, если мы находимся перед началом
			leaf* m_pBegin = nullptr;
			// храним текущее положение
			leaf* m_pCurrent = nullptr;
		};

	public:

		CSingleLinkedList() = default;

		virtual ~CSingleLinkedList() {
			clear();
		}

		void pushBack(T& data) {
			if (getSize() == 0) {
				m_pBegin = new leaf(data, nullptr);
				m_pEnd = new leaf();
				m_pBegin->pnext = m_pEnd;
			} else {
				leaf* temp = new leaf();
				m_pEnd->data = data;
				m_pEnd->pnext = temp;
				m_pEnd = temp;
			}
		}

		void pushFront(T& data) {
			leaf* new_leaf = new leaf(data, nullptr);
			if (getSize() == 0) {
				m_pBegin = new_leaf;
				m_pEnd = new leaf();
				m_pBegin->pnext = m_pEnd;
			} else {
				new_leaf->pnext = m_pBegin;
				m_pBegin = new_leaf;
			}
		}

		T popFront() {
			if (getSize() > 0) {
				T value = m_pBegin->data;
				leaf* temp = m_pBegin;
				m_pBegin = m_pBegin->pnext;
				delete temp;
				return value;
			} else {
				throw "List is empty!";
			}
		}

		// изменяет состояние итератора. выставляет предыдущую позицию.
		void erase(CIterator& it) {
			if (!it.isValid()) {
				return;
			}
			if (it.getLeaf() == m_pBegin) {
				popFront();
				if (getSize() > 0) {
					it.setLeafPreBegin(m_pBegin);
				}
			} else {
				leaf* curr = it.getLeaf();
				leaf* temp = new leaf();
				temp = m_pBegin;
				while (temp->pnext != curr) {
					temp = temp->pnext;
				}
				it.setLeafPreBegin(temp);
				temp->pnext = curr->pnext;
				delete curr;
			}
		}

		int getSize() {
			leaf* curr = m_pBegin;
			int size = 0;
			while (nullptr != curr) {
				++size;
				curr = curr->pnext;
			}
			return std::max(0, --size);
		}

		void clear() {
			while (nullptr != m_pBegin) {
				popFront();
			}
		}

		CIterator begin() {
			return CIterator(m_pBegin);
		}

	private:
		leaf* m_pBegin = nullptr;
		leaf* m_pEnd = nullptr;
	};

	template<class T>
	class CDualLinkedList {
	private:
		struct leaf {
			T data;
			leaf * pnext, *pprev;
			leaf() = default;
			leaf(T& _data, leaf * _pprev, leaf * _pnext) {
				data = _data;
				pprev = _pprev;
				pnext = _pnext;
			}
		};
	public:
		class CIterator {
		public:
			CIterator() {
				m_pBegin = nullptr;
				m_pCurrent = nullptr;
				m_pEnd = nullptr;
			}

			CIterator(leaf *p) {
				m_pBegin = p;
				m_pCurrent = p;
				m_pEnd = p->pnext;
			}

			CIterator(const CIterator &src) {
				m_pBegin = src.m_pBegin;
				m_pCurrent = src.m_pCurrent;
				m_pEnd = src.m_pEnd;
			}

			~CIterator() {}

			CIterator& operator = (const CIterator&  src) {
				if (this == &src) {
					return *this;
				}
				m_pBegin = src.m_pBegin;
				m_pCurrent = src.m_pCurrent;
				m_pEnd = src.m_pEnd;
				return *this;
			}

			bool operator != (const CIterator&  it) const {
				return it.m_pBegin == m_pBegin && it.m_pEnd == m_pEnd && it.m_pCurrent == m_pCurrent;
			}

			void operator++() {
				m_pCurrent = m_pCurrent->pnext;
			}

			void operator--() {
				m_pCurrent = m_pCurrent->pprev;
			}

			T& getData() {
				return m_pCurrent->data;
			}

			T& operator* () {
				return m_pCurrent->data;
			}

			leaf* getLeaf() {
				return m_pCurrent;
			}

			// применяется в erase и eraseAndNext
			void setLeaf(leaf* p) {
				if (nullptr == m_pBegin) {
					m_pBegin = p;
				}
				m_pCurrent = p;
				m_pEnd = p->pnext;
			}

			// применяется в erase и eraseAndNext
			void setLeafPreBegin(leaf* p) {
				//не особо понять смысл
			}

			// применяется в erase и eraseAndNext
			void setLeafPostEnd(leaf* p) {
				//аналогично
			}

			bool isValid() {
				return m_pCurrent != nullptr;
			}

		private:
			//храним голову списка, если мы находимся перед началом
			leaf* m_pBegin;
			// храним текущее положение
			leaf* m_pCurrent;
			//храним конец списка, если мы находимся после конца
			leaf* m_pEnd;
		};

	public:

		CDualLinkedList() = default;

		virtual ~CDualLinkedList() {
			clear();
		};

		void pushBack(T& data) {
			if (getSize() == 0) {
				m_pBegin = new leaf(data, nullptr, nullptr);
				m_pEnd = new leaf();
				m_pBegin->pnext = m_pEnd;
				m_pEnd->pprev = m_pBegin;
			} else {
				leaf* temp = new leaf();
				m_pEnd->data = data;
				m_pEnd->pnext = temp;
				temp->pprev = m_pEnd;
				m_pEnd = temp;
			}
		}

		T popBack() {
			T tmp;
			return tmp;
		}

		void pushFront(T& data) {
			leaf* new_leaf = new leaf(data, nullptr, nullptr);
			if (getSize() == 0) {
				m_pBegin = new_leaf;
				m_pEnd = new leaf();
				m_pBegin->pnext = m_pEnd;
				m_pEnd->pprev = m_pBegin;
			} else {
				new_leaf->pnext = m_pBegin;
				m_pBegin = new_leaf;
			}
		}

		T popFront() {
			if (getSize() > 0) {
				T value = m_pBegin->data;
				leaf* temp = m_pBegin;
				m_pBegin = m_pBegin->pnext;
				m_pBegin->pprev = nullptr;
				delete temp;
				return value;
			}
			else {
				throw "List is empty!";
			}
		}

		// изменяет состояние итератора. выставляет предыдущую позицию.
		void erase(CIterator& it) {
			if (!it.isValid()) {
				return;
			}
			if (it.getLeaf() == m_pBegin) {
				popFront();
			} else {
				leaf* curr = it.getLeaf();
				leaf* temp = new leaf();
				temp = m_pBegin;
				while (temp->pnext != curr) {
					temp = temp->pnext;
				}
				temp->pnext = curr->pnext;
				curr->pnext->pprev = temp;
				delete curr;
			}
		}

		// изменяет состояние итератора. выставляет следующую позицию.
		void eraseAndNext(CIterator& it) {
			//каково предназначение данного метода?
		}

		int getSize() {
			leaf* curr = m_pBegin;
			int size = 0;
			while (nullptr != curr) {
				++size;
				curr = curr->pnext;
			}
			return std::max(0, --size);
		}

		void clear() {
			while (nullptr != m_pBegin) {
				popFront();
			}
		}

		CIterator begin() {
			return CIterator(m_pBegin);
		}

		CIterator end() {
			return CIterator(m_pEnd);
		}

	private:
		leaf* m_pBegin = nullptr;
		leaf* m_pEnd = nullptr;
	};
};
#endif //#ifndef TEMPLATES_LIST_2021_02_11
