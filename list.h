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
				m_pBegin = nullptr;
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
				return (it.m_pBegin != m_pBegin) || (it.m_pCurrent != m_pCurrent);
			}

			void operator++() {
				if (m_pCurrent != m_pBegin) {
					m_pCurrent = m_pCurrent->pnext;
				} else {
					m_pBegin = nullptr;
				}
			}

			T& getData() {
				if (m_pCurrent == m_pBegin) {
					throw "List is empty!";
				}
				return m_pCurrent->data;
			}

			T& operator* () {
				if (m_pCurrent == m_pBegin) {
					throw "List is empty!";
				}
				return m_pCurrent->data;
			}

			leaf* getLeaf() {
				return m_pCurrent;
			}

			void setLeaf(leaf* p) {
				m_pBegin = nullptr;
				m_pCurrent = p;
			}

			void setLeafPreBegin(leaf* p) {
				m_pBegin = p;
				m_pCurrent = p;
			}

			bool isValid() {
				return (m_pBegin == nullptr) && (m_pCurrent != nullptr);
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
			leaf* new_leaf = new leaf(data, nullptr);
			if (nullptr == m_pBegin) {
				m_pBegin = new_leaf;
				m_pEnd = new_leaf;
			} else {
				m_pEnd->pnext = new_leaf;
				m_pEnd = new_leaf;
			}
		}

		void pushFront(T& data) {
			leaf* new_leaf = new leaf(data, nullptr);
			if (nullptr == m_pBegin) {
				m_pBegin = new_leaf;
				m_pEnd = new_leaf;
			} else {
				new_leaf->pnext = m_pBegin;
				m_pBegin = new_leaf;
			}
		}

		T popFront() {
			if (m_pBegin) {
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
			leaf* curr = it.getLeaf();
			if (curr == m_pBegin) {
				leaf* temp = m_pBegin;
				if (getSize() == 1) {
					m_pBegin = nullptr;
					m_pEnd = nullptr;
				} else {
					m_pBegin = m_pBegin->pnext;
				}
				it.setLeafPreBegin(m_pBegin);
				delete temp;
			} else {
				leaf* temp = new leaf();
				temp = m_pBegin;
				while (temp->pnext != curr) {
					temp = temp->pnext;
				}
				it.setLeaf(temp);
				if (curr == m_pEnd) {
					m_pEnd = temp;
				}
				temp->pnext = curr->pnext;
				delete curr;
			}
		}

		int getSize() {
			int size = 0;
			for (CIterator it = begin(); it.isValid(); ++it) {
				++size;
			}
			return size;
		}

		void clear() {
			if (nullptr == m_pBegin) {
				return;
			}
			while (nullptr != m_pBegin) {
				popFront();
			}
			m_pBegin = nullptr;
			m_pEnd = nullptr;
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
				m_pBegin = nullptr;
				m_pEnd = nullptr;
				m_pCurrent = p;
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
				return !(it.m_pBegin == m_pBegin && it.m_pEnd == m_pEnd && it.m_pCurrent == m_pCurrent);
			}

			void operator++() {
				if (m_pBegin == m_pCurrent) {
					m_pBegin = nullptr;
				} else {
					m_pCurrent = m_pCurrent->pnext;
				}
			}

			void operator--() {
				if (m_pEnd == m_pCurrent) {
					m_pEnd = nullptr;
				}
				else {
					m_pCurrent = m_pCurrent->pprev;
				}
			}

			T& getData() {
				if (m_pCurrent != m_pBegin && m_pCurrent != m_pEnd) {
					return m_pCurrent->data;
				} else {
					throw "List is empty!";
				}
			}

			T& operator* () {
				if (m_pCurrent != m_pBegin && m_pCurrent != m_pEnd) {
					return m_pCurrent->data;
				}
				else {
					throw "List is empty!";
				}
			}

			leaf* getLeaf() {
				return m_pCurrent;
			}

			// применяется в erase и eraseAndNext
			void setLeaf(leaf* p) {
				m_pBegin = nullptr;
				m_pEnd = nullptr;
				m_pCurrent = p;
			}

			// применяется в erase и eraseAndNext
			void setLeafPreBegin(leaf* p) {
				m_pBegin = p;
				m_pEnd = nullptr;
				m_pCurrent = p;
			}

			// применяется в erase и eraseAndNext
			void setLeafPostEnd(leaf* p) {
				m_pBegin = nullptr;
				m_pEnd = p;
				m_pCurrent = p;
			}

			bool isValid() {
				return (m_pCurrent != nullptr && m_pBegin == nullptr && m_pEnd == nullptr);
			}

		private:
			//храним голову списка, если мы находимся перед началом
			leaf* m_pBegin = nullptr;
			// храним текущее положение
			leaf* m_pCurrent = nullptr;
			//храним конец списка, если мы находимся после конца
			leaf* m_pEnd = nullptr;
		};

	public:

		CDualLinkedList() = default;

		virtual ~CDualLinkedList() {
			clear();
		};

		void pushBack(T& data) {
			leaf* new_leaf = new leaf(data, nullptr, nullptr);
			if (nullptr == m_pBegin) {
				m_pBegin = new_leaf;
				m_pEnd = new_leaf;
			} else {
				new_leaf->pprev = m_pEnd;
				m_pEnd->pnext = new_leaf;
				m_pEnd = new_leaf;
			}
		}

		T popBack() {
			if (m_pEnd) {
				T value = m_pEnd->data;
				if (m_pEnd != m_pBegin) {
					leaf* temp = m_pEnd;
					m_pEnd = m_pEnd->pprev;
					m_pEnd->pnext = nullptr;
					delete temp;
				} else {
					delete m_pEnd;
					m_pBegin = nullptr;
					m_pEnd = nullptr;
				}
				return value;
			} else {
				throw "List is empty!";
			}
		}

		void pushFront(T& data) {
			leaf* new_leaf = new leaf(data, nullptr, nullptr);
			if (m_pBegin) {
				m_pBegin->pprev = new_leaf;
				new_leaf->pnext = m_pBegin;
				m_pBegin = new_leaf;
			} else {
				m_pBegin = new_leaf;
				m_pEnd = new_leaf;
			}
		}

		T popFront() {
			if (m_pBegin) {
				T value = m_pBegin->data;
				if (m_pEnd != m_pBegin) {
					leaf* temp = m_pBegin;
					m_pBegin = m_pBegin->pnext;
					m_pBegin->pprev = nullptr;
					delete temp;
				}
				else {
					delete m_pBegin;
					m_pBegin = nullptr;
					m_pEnd = nullptr;
				}
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
			leaf* curr = it.getLeaf();
			if (curr == m_pBegin) {
				leaf* temp = m_pBegin;
				if (getSize() == 1) {
					m_pBegin = nullptr;
					m_pEnd = nullptr;
				} else {
					m_pBegin = m_pBegin->pnext;
					m_pBegin->pprev = nullptr;
				}
				it.setLeafPreBegin(m_pBegin);
				delete temp;
			} else {
				--it;
				leaf* temp = it.getLeaf();
				temp->pnext = curr->pnext;
				if (curr == m_pEnd) {
					m_pEnd = curr->pprev;
					m_pEnd->pnext = nullptr;
				} else {
					leaf* curr_next = curr->pnext;
					curr_next->pprev = temp;
				}
				delete curr;
			}
		}

		// изменяет состояние итератора. выставляет следующую позицию.
		void eraseAndNext(CIterator& it) {
			if (!it.isValid()) {
				return;
			}
			leaf* curr = it.getLeaf();
			if (curr == m_pEnd) {
				leaf* temp = m_pEnd;
				if (getSize() == 1) {
					m_pBegin = nullptr;
					m_pEnd = nullptr;
				} else {
					m_pEnd = m_pEnd->pprev;
					m_pEnd->pnext = nullptr;
				}
				it.setLeafPostEnd(m_pEnd);
				delete temp;
			} else {
				++it;
				leaf* temp = it.getLeaf();
				temp->pprev = curr->pprev;
				if (curr == m_pBegin) {
					m_pBegin = curr->pnext;
					m_pBegin->pprev = nullptr;
				}
				else {
					leaf* curr_prev = curr->pprev;
					curr_prev->pnext = temp;
				}
				delete curr;
			}
		}

		int getSize() {
			int size = 0;
			for (CIterator it = begin(); it.isValid(); ++it) {
				++size;
			}
			return size;
		}

		void clear() {
			if (nullptr == m_pBegin) {
				return;
			}
			while (nullptr != m_pBegin) {
				popFront();

			}
			m_pBegin = nullptr;
			m_pEnd = nullptr;
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
