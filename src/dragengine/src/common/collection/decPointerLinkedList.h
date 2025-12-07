/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DECPOINTERLINKEDLIST_H_
#define _DECPOINTERLINKEDLIST_H_

#include "../../dragengine_export.h"


/**
 * \brief Double linked list of pointers.
 * 
 * All pointers except NULL are allowed. Pointers can not be included multiple times in the list.
 * 
 * To use a pointer with the linked list add the cListEntry class instance as member to the class
 * to be stored in the linked list like this:
 * 
 * \code{.cpp}
 * #include <dragengine/common/collection/decPointerLinkedList.h>
 * 
 * class MyClass{
 * public:
 * decPointerLinkedList::cListEntry MyList;
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
 */
class DE_DLL_EXPORT decPointerLinkedList{
public:
	/**
	 * \brief List entry.
	 * \warning Use only on stack! Never use it with new allocator!
	 */
	class DE_DLL_EXPORT cListEntry{
	private:
		void *pOwner;
		cListEntry *pPrev;
		cListEntry *pNext;
		decPointerLinkedList *pList;
		
		
		
	public:
		/** \name Constructors and Destructors */
		/*@{*/
		/**
		 * \brief Create list entry.
		 * \warning Use only on stack! Never use it with new allocator!
		 */
		cListEntry(void *owner);
		
		/**
		 * \brief Clean up list entry.
		 */
		~cListEntry();
		/*@}*/
		
		
		
		/** \name Management */
		/*@{*/
		/** \brief List entry owner. */
		inline void *GetOwner() const{return pOwner;}
		
		/** \brief Previous linked list entry or NULL. */
		inline cListEntry *GetPrevious() const{return pPrev;}
		
		/** \brief Next linked list entry or NULL. */
		inline cListEntry *GetNext() const{return pNext;}
		
		/** \brief Linked list or NULL if not in a list. */
		inline decPointerLinkedList *GetList() const{return pList;}
		/*@}*/
		
		
		
		/**
		 * \name Internal use.
		 * \warning For internal use only! Never call on your own!
		 */
		/*@{*/
		/**
		 * \warning For internal use only! Never call on your own!
		 */
		void SetPrevious(cListEntry *previous);
		void SetNext(cListEntry *next);
		void SetList(decPointerLinkedList *list);
		void Clear();
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
	 * decPointerLinkedList::cIterator iter( list );
	 * while( iter.Next() ){  // also possible while( iter++ )
	 * // entry can be accesses using different methods
	 * void * owner1 = iter.Entry() ...
	 * void * owner2 = iter->GetOwner() ...
	 * }
	 * \endcode
	 */
	class DE_DLL_EXPORT cIterator{
	private:
		decPointerLinkedList &pList;
		void *pEntry;
		cListEntry *pNextEntry;
		cListEntry *pLastEntry;
		
		
		
	public:
		/** \name Constructors and Destructors */
		/*@{*/
		/**
		 * \brief Iterator over entire list.
		 */
		cIterator(decPointerLinkedList &list);
		
		/**
		 * \brief Iterator from entry to end of list.
		 */
		cIterator(cListEntry *firstEntry);
		
		/**
		 * \brief Iterator from entry to another entry.
		 * \warning If last entry is not located after first entry result can be undefined.
		 */
		cIterator(cListEntry *firstEntry, cListEntry *lastEntry);
		/*@}*/
		
		
		
		/** \name Management */
		/*@{*/
		/**
		 * \brief Owner of entry the iterator is located at.
		 * \throws EInvalidParam Iteration finished.
		 */
		void *Entry() const;
		
		/**
		 * \brief Advance to next entry if iteration has not finished.
		 * 
		 * If next entry is past the last entry or past the end of list iteration finishes.
		 * 
		 * \returns true if iterator is located at the next entry or false if iteration finished.
		 */
		bool Advance();
		
		/**
		 * \brief Iteration finished.
		 * \returns false if iterator is located at a valid entry or true if iteration finished.
		 */
		bool HasFinished() const;
		
		/**
		 * \brief Iteration has not finished.
		 * \returns true if iterator is located at a valid entry or false if iteration finished.
		 */
		bool HasNotFinished() const;
		/*@}*/
		
		
		
		/** \name Operators */
		/*@{*/
		/**
		 * \brief Entry the iterator is located at.
		 * \throws EInvalidParam Iteration finished.
		 */
		operator void*() const;
		
		/**
		 * \brief Owner of entry the iterator is located at.
		 * \throws EInvalidParam Iteration finished.
		 */
		void *operator->() const;
		
		/**
		 * \brief Advance to next entry if iteration has not finished.
		 * 
		 * If next entry is past the last entry or past the end of list iteration finishes.
		 * 
		 * \returns true if iterator is located at the next entry or false if iteration finished.
		 */
		bool operator++();
		
		/**
		 * \brief Advance by multiple entries.
		 * 
		 * Same as calling Advance() multiple times in a row.
		 */
		bool operator+=(int distance);
		
		/**
		 * \brief Iteration finished.
		 * \returns false if iterator is located at a valid entry or true if iteration finished.
		 */
		bool operator!() const;
		
		/**
		 * \brief Iteration has not finished.
		 * \returns true if iterator is located at a valid entry or false if iteration finished.
		 */
		operator bool() const;
		/*@}*/
	};
	
	
	
private:
	cListEntry *pRoot;
	cListEntry *pTail;
	int pCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new linked list. */
	decPointerLinkedList();
	
	/** \brief Clean up linked list. */
	~decPointerLinkedList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of objects. */
	inline int GetCount() const{return pCount;}
	
	/** \brief Root entry at start of list or NULL if list is empty. */
	inline cListEntry *GetRoot() const{return pRoot;}
	
	/** \brief Tail entry at end of list or NULL if list is empty. */
	inline cListEntry *GetTail() const{return pTail;}
	
	/**
	 * \brief Pointer at index.
	 * 
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	cListEntry *GetAt(int index) const;
	
	/** \brief Index of pointer or -1 if not found. */
	int IndexOf(cListEntry *entry) const;
	
	/** \brief Determine if pointer exists in the list. */
	bool Has(cListEntry *entry) const;
	
	/** \brief Add pointer. */
	void Add(cListEntry *entry);
	
	/**
	 * \brief Insert pointer before entry.
	 * \throws deeInvalidParam \em beforeEntry is invalid.
	 */
	void InsertBefore(cListEntry *entry, cListEntry *before);
	
	/**
	 * \brief Insert pointer after entry.
	 * \throws deeInvalidParam \em beforeEntry is invalid.
	 */
	void InsertAfter(cListEntry *entry, cListEntry *after);
	
	/** \brief Remove pointer. */
	void Remove(cListEntry *entry);
	
	/** \brief Remove all pointers. */
	void RemoveAll();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/**
	 * \brief Pointer at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	cListEntry *operator[](int position) const;
	/*@}*/
};

#endif
