#ifndef AVL_HEAD_H_2021_04_01
#define AVL_HEAD_H_2021_04_01

#include "mm.h"
#include <algorithm>

namespace lab618
{
	const int BLOCK_SIZE = 10;

	template <class T, int(*Compare)(const T *pElement, const T* pElement2) >
	class CAVLTree {
	private:
		struct leaf {
			T* pData = nullptr;
			leaf *pLeft = nullptr;
			leaf *pRight = nullptr;
			int balanceFactor = 0;

			leaf() = default;
		};

	public:
		class CException {
		public:
			CException()
			{}
		};

	public:

		CAVLTree() : m_pRoot(nullptr), m_Memory(CMemoryManager<leaf>(BLOCK_SIZE, true)) {}


		virtual ~CAVLTree() {
			clear();
		}

		void childrenBF(leaf* root, int& left_child_BF, int& right_child_BF) {
			if (root->pLeft != nullptr) {
				left_child_BF = root->pLeft->balanceFactor;
			}
			if (root->pRight != nullptr) {
				right_child_BF = root->pRight->balanceFactor;
			}
		}

		void updateBF(leaf* root, int left_height, int right_height) {
			root->balanceFactor = left_height - right_height;
		}

		leaf* balance(leaf* root) {
			int curr_BF = root->balanceFactor;
			int left_child_BF = 0;
			int right_child_BF = 0;
			if (root->pLeft != nullptr) {
				left_child_BF = root->pLeft->balanceFactor;
			}
			if (root->pRight != nullptr) {
				right_child_BF = root->pRight->balanceFactor;
			}
			if (curr_BF == -2) {
				if (right_child_BF > 0) {
					root->pRight = smallRotateRight(root->pRight);
				}
				return smallRotateLeft(root);
			}
			if (curr_BF == 2) {
				if (left_child_BF < 0) {
					root->pLeft = smallRotateLeft(root->pLeft);
				}
				return smallRotateRight(root);
			}
			return root;
		}

		leaf* smallRotateLeft(leaf* p) {
			leaf* q = p->pRight;
			p->pRight = q->pLeft;
			q->pLeft = p;
			int left_child_BF_q = 0; 
			int right_child_BF_q = left_child_BF_q - q->balanceFactor;
			int left_child_BF_p = std::max(left_child_BF_q, right_child_BF_q) + 1 + p->balanceFactor;
			updateBF(p, left_child_BF_p, left_child_BF_q);
			updateBF(q, std::max(left_child_BF_p, left_child_BF_q) + 1, right_child_BF_q);
			return q;
		}

		leaf* smallRotateRight(leaf* p) {
			leaf* q = p->pLeft;
			p->pLeft = q->pRight;
			q->pRight = p;
			int left_child_BF_q = 0;
			int right_child_BF_q = left_child_BF_q - q->balanceFactor;
			int right_child_BF_p = std::max(left_child_BF_q, right_child_BF_q) + 1 - p->balanceFactor;
			updateBF(p, right_child_BF_q, right_child_BF_p);
			updateBF(q, left_child_BF_q, std::max(right_child_BF_p, right_child_BF_q) + 1);
			return q;
		}

		leaf* recInsert(leaf* root, T* pElement, bool& is_success) {
			if (nullptr == root) {
				leaf* new_leaf = m_Memory.newObject();
				new_leaf->pData = pElement;
				is_success = true;
				return new_leaf;
			}
			int comp_result = Compare(root->pData, pElement);
			if (comp_result < 0) {
				root->pLeft = recInsert(root->pLeft, pElement, is_success);
				if (is_success) {
					++root->balanceFactor;
				}
			} else {
				root->pRight = recInsert(root->pRight, pElement, is_success);
				if (is_success) {
					--root->balanceFactor;
				}
			}
			root = balance(root);
			return root;
		}

		leaf* recFind(leaf* root, const T& element) {
			if (nullptr == root) {
				return nullptr;
			}
			int comp_result = Compare(root->pData, &element);
			if (comp_result > 0) {
				return recFind(root->pRight, element);
			}
			if (comp_result < 0) {
				return recFind(root->pLeft, element);
			}
			return root;
		}

		leaf* recFindMin(leaf* root) {
			if (nullptr == root->pLeft) {
				return root;
			}
			return recFindMin(root->pLeft);
		}

		leaf* recRemoveMin(leaf* root) {
			if (nullptr == root->pLeft) {
				return root->pRight;
			}
			root->pLeft = recRemoveMin(root->pLeft);
			root = balance(root);
			return root;
		}

		leaf* recRemove(leaf* root, const T& element, bool& is_success) {
			if (nullptr == root) {
				return nullptr;
			}
			int comp_result = Compare(root->pData, &element);
			if (comp_result > 0) {
				root->pRight = recRemove(root->pRight, element, is_success);
			}
			else if (comp_result < 0) {
				root->pLeft = recRemove(root->pLeft, element, is_success);
			} else {
				leaf* left_child = root->pLeft;
				leaf* right_child = root->pRight;
				m_Memory.deleteObject(root);
				is_success = true;
				if (nullptr == right_child) {
					return left_child;
				}
			
				leaf* min_leaf_in_right = recFindMin(right_child);
				min_leaf_in_right->pRight = recRemoveMin(right_child);
				min_leaf_in_right->pLeft = left_child;
				return balance(min_leaf_in_right);
			}
			return balance(root);
		}

		bool add(T* pElement) {
			if (nullptr == m_pRoot) {
				leaf* new_leaf = m_Memory.newObject();
				new_leaf->pData = pElement;
				m_pRoot = new leaf*(new_leaf);
				return true;
			}
			if (nullptr == find(*pElement)) {
				bool is_success = false;
				leaf* temp = recInsert(*m_pRoot, pElement, is_success);
				m_pRoot = new leaf*(temp);
				return is_success;
			}
			return false;
		}

		bool update(T* pElement) {
			if (nullptr == m_pRoot || nullptr == find(*pElement)) {
				bool is_success = false;
				leaf* temp = recInsert(*m_pRoot, pElement, is_success);
				m_pRoot = new leaf*(temp);
				return !is_success;
			}
			recFind(*m_pRoot, *pElement)->pData = pElement;
			return true;
		}

		T* find(const T& element) {
			if (nullptr == m_pRoot || nullptr == recFind(*m_pRoot, element)) {
				return nullptr;
			}
			return recFind(*m_pRoot, element)->pData;

		}

		bool remove(const T& element) {
			auto a = recFind(*m_pRoot, element);
			if (nullptr == m_pRoot || nullptr == recFind(*m_pRoot, element)) {
				return false;
			}
			if (recFind(*m_pRoot, element) == *m_pRoot && nullptr == (*m_pRoot)->pLeft && nullptr == (*m_pRoot)->pRight) {
					delete m_pRoot;
					m_pRoot = nullptr;
					return true;
			}
			bool is_success = false;
			leaf* temp = recRemove(*m_pRoot, element, is_success);
			m_pRoot = new leaf*(temp);
			return is_success;
		}

		void clear() {
			m_Memory.clear();
		}

	private:
		leaf** m_pRoot = nullptr;
		CMemoryManager<leaf> m_Memory;
	};
}; // namespace templates

#endif // #define AVL_HEAD_H_2021_04_01
