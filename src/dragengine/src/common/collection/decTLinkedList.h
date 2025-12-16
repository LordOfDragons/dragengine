/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _DECTLINKEDLIST_H_
#define _DECTLINKEDLIST_H_

#include <iterator>
#include <cstddef>

#include "../exceptions.h"
#include "../../deTObjectReference.h"
#include "../../threading/deTThreadSafeObjectReference.h"


/**
 * \brief Double linked list template class.
 * 
 * All elements except default constructed values are allowed. Elements can not be included multiple times in the list.
 * 
 * To use an element with the linked list add the cListEntry class instance as member to the class
 * to be stored in the linked list like this:
 * 
 * \code{.cpp}
 * #include <dragengine/common/collection/decTLinkedList.h>
 * 
 * class MyClass{
 * public:
 * decTLinkedList<MyClass>::cListEntry MyList;
 * ...
 * }
 * 
 * MyClass::MyClass() : MyList(this){
 * ...
 * }
 * \endcode
 * 
 * The constructor call links the cListEntry to the owner class and allows to retrieve the owner
 * class put in the list from a list entry.
 * 
 * Template parameter TG allows to define reference holder type, for example
 * deTObjectReference<MyClass>. If not set it defaults to raw pointer MyClass*.
 */
template<typename T, typename TR = T*>
class decTLinkedList{
public:
	/**
	 * \brief List entry.
	 * \warning Use only on stack! Never use it with new allocator!
	 */
	class Element{
	private:
		T *pOwner;
		TR pReference;
		Element *pPrev, *pNext;
		decTLinkedList<T,TR> *pList;
		
		
	public:
		/** \name Constructors and Destructors */
		/*@{*/
		/**
		 * \brief Create list entry.
		 * \warning Use only on stack! Never use it with new allocator!
		 */
		explicit Element(T *owner) : pOwner(owner), pPrev(nullptr), pNext(nullptr), pList(nullptr){
			DEASSERT_NOTNULL(owner)
		}
		
		/**
		 * \brief Clean up list entry.
		 */
		~Element(){
			if(pList){
				try{
					pList->Remove(this);
				}catch(...){}
			}
		}
		/*@}*/
		
		
		/** \name Management */
		/*@{*/
		/** \brief List entry owner. */
		inline T *GetOwner() const{ return pOwner; }
		
		/** \brief Previous linked list entry or nullptr. */
		inline Element *GetPrevious() const{ return pPrev; }
		
		/** \brief Next linked list entry or nullptr. */
		inline Element *GetNext() const{ return pNext; }
		
		/** \brief Linked list or nullptr if not in a list. */
		inline decTLinkedList<T,TR> *GetList() const{ return pList; }
		/*@}*/
		
		friend class decTLinkedList<T,TR>;
	};
	
	
	/**
	 * \brief Forward moving iterator.
	 * 
	 * Iterator stores last entry to iterator during constructor. It is safe to add and insert
	 * entries while the iterator is in use. Removing entries is safe if the entry is located
	 * before the current iterator position. Other removing operations are potentially
	 * dangerous. The iterator ensures entries added after the last entry while iterator is
	 * in use will not be iteratored.
	 * 
	 * After constructing it is required to call Next() to advance the iterator to the first
	 * entry. A typical iteration looks like this:
	 * 
	 * \code{.cpp}
	 * decTLinkedList<MyClass>::cIterator iter( list );
	 * while( iter.Next() ){  // also possible while( iter++ )
	 * // entry can be accesses using different methods
	 * MyClass &owner1 = iter.Entry() ...
	 * MyClass &owner2 = iter->GetOwner() ...
	 * }
	 * \endcode
	 */
	class ElementIterator{
	private:
		decTLinkedList<T,TR> &pList;
		T *pEntry;
		Element *pNextEntry, *pLastEntry;
		
		
	public:
		/** \name Constructors and Destructors */
		/*@{*/
		/**
		 * \brief Iterator over entire list.
		 */
		explicit ElementIterator(decTLinkedList<T,TR> &list) :
		pList(list),
		pEntry(nullptr),
		pNextEntry(list.pRoot),
		pLastEntry(list.pTail){
		}
		
		/**
		 * \brief Iterator from entry to end of list.
		 */
		explicit ElementIterator(Element *firstEntry) :
		pList(*firstEntry->GetList()),
		pEntry(nullptr),
		pNextEntry(firstEntry),
		pLastEntry(pList.pTail){
		}
		
		/**
		 * \brief Iterator from entry to another entry.
		 * \warning If last entry is not located after first entry result can be undefined.
		 */
		ElementIterator(Element *firstEntry, Element *lastEntry) :
		pList(*firstEntry->GetList()),
		pEntry(nullptr),
		pNextEntry(firstEntry),
		pLastEntry(lastEntry){
		}
		/*@}*/
		
		
		/** \name Management */
		/*@{*/
		/**
		 * \brief Entry the iterator is located at.
		 * \throws EInvalidParam Iteration finished.
		 */
		T &Entry() const{
			DEASSERT_NOTNULL(pEntry)
			return *pEntry;
		}
		
		/**
		 * \brief Advance to next entry if iteration has not finished.
		 * 
		 * If next entry is past the last entry or past the end of list iteration finishes.
		 * 
		 * \returns true if iterator is located at the next entry or false if iteration finished.
		 */
		bool Advance(){
			if(pNextEntry){
				pEntry = pNextEntry->GetOwner();
				if(pNextEntry == pLastEntry){
					pNextEntry = nullptr;
					
				}else{
					pNextEntry = pNextEntry->GetNext();
				}
				return true;
				
			}else{
				pEntry = nullptr;
				return false;
			}
		}
		
		/**
		 * \brief Iteration finished.
		 * \returns false if iterator is located at a valid entry or true if iteration finished.
		 */
		bool HasFinished() const{
			return pEntry == nullptr;
		}
		
		/**
		 * \brief Iteration has not finished.
		 * \returns true if iterator is located at a valid entry or false if iteration finished.
		 */
		bool HasNotFinished() const{
			return pEntry != nullptr;
		}
		/*@}*/
		
		
		/** \name Operators */
		/*@{*/
		/**
		 * \brief Entry the iterator is located at.
		 * \throws EInvalidParam Iteration finished.
		 */
		operator T*() const{
			DEASSERT_NOTNULL(pEntry)
			return pEntry;
		}
		
		/**
		 * \brief Entry the iterator is located at.
		 * \throws EInvalidParam Iteration finished.
		 */
		operator T&() const{
			DEASSERT_NOTNULL(pEntry)
			return *pEntry;
		}
		
		/**
		 * \brief Entry the iterator is located at.
		 * \throws EInvalidParam Iteration finished.
		 */
		T *operator->() const{
			DEASSERT_NOTNULL(pEntry)
			return pEntry;
		}
		
		/**
		 * \brief Advance to next entry if iteration has not finished.
		 * 
		 * If next entry is past the last entry or past the end of list iteration finishes.
		 * 
		 * \returns true if iterator is located at the next entry or false if iteration finished.
		 */
		bool operator++(){
			return Advance();
		}
		
		/**
		 * \brief Advance by multiple entries.
		 * 
		 * Same as calling Advance() multiple times in a row.
		 */
		bool operator+=(int distance){
			while(distance-- > 0){
				if(!Advance()){
					return false;
				}
			}
			return true;
		}
		
		/**
		 * \brief Iteration finished.
		 * \returns false if iterator is located at a valid entry or true if iteration finished.
		 */
		bool operator!() const{
			return pEntry == nullptr;
		}
		
		/**
		 * \brief Iteration has not finished.
		 * \returns true if iterator is located at a valid entry or false if iteration finished.
		 */
		operator bool() const{
			return pEntry != nullptr;
		}
		
		/**
		 * \brief Copy operator.
		 */
		ElementIterator &operator=(const ElementIterator &o){
			if(this != &o){
				pEntry = o.pEntry;
				pNextEntry = o.pNextEntry;
				pLastEntry = o.pLastEntry;
			}
			return *this;
		}
		/*@}*/
	};
	
	
private:
	Element *pRoot, *pTail;
	int pCount;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new linked list. */
	decTLinkedList() : pRoot(nullptr), pTail(nullptr), pCount(0){
	}
	
	/** \brief Clean up linked list. */
	~decTLinkedList(){
		try{
			RemoveAll();
		}catch(...){}
	}
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of elements. */
	inline int GetCount() const{ return pCount; }
	
	/** \brief List is empty. */
	inline bool IsEmpty() const{ return pCount == 0; }
	
	/** \brief List is not empty. */
	inline bool IsNotEmpty() const{ return pCount > 0; }
	
	/** \brief Root entry at start of list or nullptr if list is empty. */
	inline Element *GetRoot() const{ return pRoot; }
	
	/** \brief Tail entry at end of list or nullptr if list is empty. */
	inline Element *GetTail() const{ return pTail; }
	
	/**
	 * \brief Element at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	Element *GetAt(int index) const{
		DEASSERT_TRUE(index >= 0)
		DEASSERT_TRUE(index < pCount)
		
		Element *entry = pRoot;
		while(index-- > 0){
			entry = entry->pNext;
		}
		return entry;
	}
	
	/** \brief Index of element or -1 if not found. */
	int IndexOf(const Element *entry) const{
		const Element *iter = pRoot;
		int index = 0;
		
		while(iter){
			if(iter == entry){
				return index;
			}
			iter = iter->pNext;
			index++;
		}
		
		return -1;
	}
	
	/** \brief Determine if element exists in the list. */
	bool Has(const Element *entry) const{
		return entry && entry->GetList() == this;
	}
	
	/** \brief Add element. */
	void Add(Element *entry){
		DEASSERT_NOTNULL(entry)
		DEASSERT_NULL(entry->GetList())
		
		entry->pReference = entry->pOwner;
		entry->pPrev = pTail;
		entry->pNext = nullptr;
		
		if(pTail){
			pTail->pNext = entry;
			
		}else{
			pRoot = entry;
		}
		
		pTail = entry;
		entry->pList = this;
		pCount++;
	}
	
	/**
	 * \brief Insert element before entry.
	 * \throws deeInvalidParam \em beforeEntry is invalid.
	 */
	void InsertBefore(Element *entry, Element *before){
		DEASSERT_NOTNULL(entry)
		DEASSERT_NULL(entry->GetList())
		DEASSERT_NOTNULL(before)
		DEASSERT_TRUE(before->GetList() == this)
		
		entry->pReference = entry->pOwner;
		entry->pPrev = before->pPrev;
		entry->pNext = before;
		
		if(before->pPrev){
			before->pPrev->pNext = entry;
			
		}else{
			pRoot = entry;
		}
		
		before->pPrev = entry;
		entry->pList = this;
		pCount++;
	}
	
	/**
	 * \brief Insert element after entry.
	 * \throws deeInvalidParam \em afterEntry is invalid.
	 */
	void InsertAfter(Element *entry, Element *after){
		DEASSERT_NOTNULL(entry)
		DEASSERT_NULL(entry->GetList())
		DEASSERT_NOTNULL(after)
		DEASSERT_TRUE(after->GetList() == this)
		
		entry->pReference = entry->pOwner;
		entry->pPrev = after;
		entry->pNext = after->pNext;
		
		if(after->pNext){
			after->pNext->pPrev = entry;
			
		}else{
			pTail = entry;
		}
		
		after->pNext = entry;
		entry->pList = this;
		pCount++;
	}
	
	/** \brief Remove element. */
	void Remove(Element *entry){
		DEASSERT_NOTNULL(entry)
		DEASSERT_TRUE(entry->GetList() == this)
		
		if(entry->pPrev){
			entry->pPrev->pNext = entry->pNext;
			
		}else{
			pRoot = entry->pNext;
		}
		
		if(entry->pNext){
			entry->pNext->pPrev = entry->pPrev;
			
		}else{
			pTail = entry->pPrev;
		}
		
		entry->pPrev = nullptr;
		entry->pNext = nullptr;
		entry->pList = nullptr;
		
		// this is required or strange bugs can happen. Element is supposed to be a class member
		// of type T. TR is potentially a smart pointer holding owner alive. if the reference is
		// cleared owner is potentially destroyed while inside the smart pointer assignment
		// operator. this causes Element destructor to be called which in turn has the smart
		// pointer we are currently clearing. this causes double FreeReference() to be called
		// which causes an exception thrown and ultimately a segfault. by guarding the reference
		// before clearing the Element is in a proper cleared state before the reference goes
		// out of scope
		const TR guard(entry->pReference);
		entry->pReference = {};
		
		pCount--;
	}
	
	/** \brief Remove all elements. */
	void RemoveAll(){
		while(pRoot){
			Remove(pRoot);
		}
	}
	
	
	/**
	 * \brief Visit elements.
	 * \param[in] visitor Visitor callable invoked as visitor(T*).
	 */
	template<typename Visitor>
	void Visit(Visitor &visitor) const {
		Element *entry = pRoot;
		while(entry){
			visitor(entry->pOwner);
			entry = entry->pNext;
		}
	}
	
	template<typename Visitor>
	void Visit(Visitor &&visitor) const{
		Visit<Visitor>(visitor);
	}
	
	
	/**
	 * \brief Find element.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T*).
	 * \param[out] found Found element if true is returned.
	 */
	template<typename Evaluator>
	bool Find(Evaluator &evaluator, T* &found) const{
		Element *entry = pRoot;
		while(entry){
			if(evaluator(entry->pOwner)){
				found = entry->pOwner;
				return true;
			}
			entry = entry->pNext;
		}
		found = nullptr;
		return false;
	}
	
	template<typename Evaluator>
	bool Find(Evaluator &&evaluator, T* &found) const{
		return Find<Evaluator>(evaluator, found);
	}
	
	
	/**
	 * \brief Find element or nullptr if absent.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T*).
	 * \param[in] defaultValue Default value returned if not found.
	 */
	template<typename Evaluator>
	T *FindOrNull(Evaluator &evaluator, T *defaultValue = nullptr) const{
		T *found = nullptr;
		return Find<Evaluator>(evaluator, found) ? found : defaultValue;
	}
	
	template<typename Evaluator>
	T *FindOrNull(Evaluator &&evaluator, T *defaultValue = nullptr) const{
		return FindOrNull<Evaluator>(evaluator, defaultValue);
	}
	
	
	/**
	 * \brief Remove elements matching condition.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T*).
	 */
	template<typename Evaluator>
	void RemoveIf(Evaluator &evaluator){
		Element *entry = pRoot;
		while(entry){
			Element *nextEntry = entry->pNext;
			if(evaluator(entry->pOwner)){
				Remove(entry);
			}
			entry = nextEntry;
		}
	}
	
	template<typename Evaluator>
	void RemoveIf(Evaluator &&evaluator){
		RemoveIf<Evaluator>(evaluator);
	}
	/*@}*/
	
	
	/** \name Operators */
	/*@{*/
	/**
	 * \brief Element at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	Element *operator[](int position) const{
		return GetAt(position);
	}
	/*@}*/
	
	
	/** \name Standard library iterators. */
	/*@{*/
	class const_iterator{
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = const T;
		using difference_type = std::ptrdiff_t;
		using pointer = const T*;
		using reference = const T&;
		
	private:
		// current element (nullptr means end)
		Element *pCurrent;
		// owning list (may be nullptr for default constructed iterator)
		const decTLinkedList<T,TR> *pList;
		
	public:
		const_iterator() : pCurrent(nullptr), pList(nullptr) {}
		
		explicit const_iterator(const decTLinkedList<T,TR> &list, Element *current) : pCurrent(current), pList(&list){}
		
		const_iterator(const const_iterator &o) = default;
		const_iterator& operator=(const const_iterator &o) = default;
		~const_iterator() = default;
		
		reference operator*() const{
			DEASSERT_NOTNULL(pCurrent)
			return *pCurrent->GetOwner();
		}
		
		pointer operator->() const{
			DEASSERT_NOTNULL(pCurrent)
			return pCurrent->GetOwner();
		}
		
		const_iterator& operator++(){
			if(pCurrent){
				pCurrent = pCurrent->GetNext();
			}
			// if pCurrent becomes nullptr, this is end()
			return *this;
		}
		
		const_iterator operator++(int){
			const_iterator tmp(*this);
			++(*this);
			return tmp;
		}
		
		const_iterator& operator--(){
			// if currently end() (pCurrent == nullptr) and we have a list, move to tail
			if(!pCurrent){
				if(pList){
					pCurrent = pList->GetTail();
				}
			}else{
				pCurrent = pCurrent->GetPrevious();
			}
			return *this;
		}
		
		const_iterator operator--(int){
			const_iterator tmp(*this);
			--(*this);
			return tmp;
		}
		
		bool operator==(const const_iterator &o) const{
			return pCurrent == o.pCurrent;
		}
		
		bool operator!=(const const_iterator &o) const{
			return !(*this == o);
		}
	};
	
	using cbegin_t = const_iterator;
	using cend_t = const_iterator;
	
	inline const_iterator begin() const{
		return const_iterator(*this, pRoot);
	}
	inline const_iterator cbegin() const{
		return const_iterator(*this, pRoot);
	}
	
	inline const_iterator end() const{
		return const_iterator(*this, nullptr);
	}
	inline const_iterator cend() const{
		return const_iterator(*this, nullptr);
	}
	/*@}*/
};



/**
 * \brief Object linked list template class.
 * 
 * This template uses deTObjectReference.
 */
template<typename T>
using decTObjectLinkedList = decTLinkedList<T, deTObjectReference<T>>;

/**
 * \brief Thread safe object linked list template class.
 * 
 * This template uses deTThreadSafeObjectReference to get thread safe assignment of
 * object references. This does not make the linked list itself thread safe though.
 */
template<typename T>
using decTThreadSafeObjectLinkedList = decTLinkedList<T, deTThreadSafeObjectReference<T>>;

#endif
