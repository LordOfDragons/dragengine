/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DECOBJECTLINKEDLIST_H_
#define _DECOBJECTLINKEDLIST_H_

#include "../../dragengine_export.h"

class deObject;


/**
 * \brief Double linked list of objects.
 * 
 * All objects except NULL are allowed. Objects can not be included multiple times in the list.
 * 
 * To use a object with the linked list add the cListEntry class instance as member to the class
 * to be stored in the linked list like this:
 * 
 * \code{.cpp}
 * #include <dragengine/common/collection/decObjectLinkedList.h>
 * 
 * class MyClass{
 * public:
 * decObjectLinkedList::cListEntry MyList;
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
 * cListEntry is keeping a weak owner object pointer. decObjectLinkedList keeps a strong
 * reference to all objects in the list.
 */
class DE_DLL_EXPORT decObjectLinkedList{
public:
	/**
	 * \brief List entry.
	 * \warning Use only on stack! Never use it with new allocator!
	 */
	class DE_DLL_EXPORT cListEntry{
	private:
		deObject *pOwner;
		cListEntry *pPrev;
		cListEntry *pNext;
		decObjectLinkedList *pList;
		
		
		
	public:
		/** \name Constructors and Destructors */
		/*@{*/
		/**
		 * \brief Create list entry.
		 * \warning Use only on stack! Never use it with new allocator!
		 */
		cListEntry( deObject *owner );
		
		/**
		 * \brief Clean up list entry.
		 */
		~cListEntry();
		/*@}*/
		
		
		
		/** \name Management */
		/*@{*/
		/** \brief List entry owner. */
		inline deObject *GetOwner() const{ return pOwner; }
		
		/** \brief Previous linked list entry or NULL. */
		inline cListEntry *GetPrevious() const{ return pPrev; }
		
		/** \brief Next linked list entry or NULL. */
		inline cListEntry *GetNext() const{ return pNext; }
		
		/** \brief Linked list or NULL if not in a list. */
		inline decObjectLinkedList *GetList() const{ return pList; }
		/*@}*/
		
		
		
		/**
		 * \name Internal use.
		 * \warning For internal use only! Never call on your own!
		 */
		/*@{*/
		/**
		 * \warning For internal use only! Never call on your own!
		 */
		void SetPrevious( cListEntry *previous );
		void SetNext( cListEntry *next );
		void SetList( decObjectLinkedList *list );
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
	 * decObjectLinkedList::cIterator iter( list );
	 * while( iter.Next() ){  // also possible while( iter++ )
	 * // entry can be accesses using different methods
	 * deObject &owner1 = iter.Entry() ...
	 * deObject &owner2 = iter->GetOwner() ...
	 * }
	 * \endcode
	 */
	class DE_DLL_EXPORT cIterator{
	private:
		decObjectLinkedList &pList;
		deObject *pEntry;
		cListEntry *pNextEntry;
		cListEntry *pLastEntry;
		
		
		
	public:
		/** \name Constructors and Destructors */
		/*@{*/
		/**
		 * \brief Iterator over entire list.
		 */
		cIterator( decObjectLinkedList &list );
		
		/**
		 * \brief Iterator from entry to end of list.
		 */
		cIterator( cListEntry *firstEntry );
		
		/**
		 * \brief Iterator from entry to another entry.
		 * \warning If last entry is not located after first entry result can be undefined.
		 */
		cIterator( cListEntry *firstEntry, cListEntry *lastEntry );
		/*@}*/
		
		
		
		/** \name Management */
		/*@{*/
		/**
		 * \brief Entry the iterator is located at.
		 * \throws EInvalidParam Iteration finished.
		 */
		deObject &Entry() const;
		
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
		operator deObject*() const;
		
		/**
		 * \brief Entry the iterator is located at.
		 * \throws EInvalidParam Iteration finished.
		 */
		operator deObject&() const;
		
		/**
		 * \brief Entry the iterator is located at.
		 * \throws EInvalidParam Iteration finished.
		 */
		deObject *operator->() const;
		
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
		bool operator+=( int distance );
		
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
	decObjectLinkedList();
	
	/** \brief Clean up linked list. */
	~decObjectLinkedList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of objects. */
	inline int GetCount() const{ return pCount; }
	
	/** \brief Root entry at start of list or NULL if list is empty. */
	inline cListEntry *GetRoot() const{ return pRoot; }
	
	/** \brief Tail entry at end of list or NULL if list is empty. */
	inline cListEntry *GetTail() const{ return pTail; }
	
	/**
	 * \brief Object at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	cListEntry *GetAt( int index ) const;
	
	/** \brief Index of object or -1 if not found. */
	int IndexOf( cListEntry *entry ) const;
	
	/** \brief Determine if object exists in the list. */
	bool Has( cListEntry *entry ) const;
	
	/** \brief Add object. */
	void Add( cListEntry *entry );
	
	/**
	 * \brief Insert object before entry.
	 * \throws deeInvalidParam \em beforeEntry is invalid.
	 */
	void InsertBefore( cListEntry *entry, cListEntry *before );
	
	/**
	 * \brief Insert object after entry.
	 * \throws deeInvalidParam \em beforeEntry is invalid.
	 */
	void InsertAfter( cListEntry *entry, cListEntry *after );
	
	/** \brief Remove object. */
	void Remove( cListEntry *entry );
	
	/** \brief Remove all objects. */
	void RemoveAll();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/**
	 * \brief Object at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	cListEntry *operator[]( int position ) const;
	/*@}*/
};

#endif
