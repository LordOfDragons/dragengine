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

#ifndef _DECPOINTERLIST_H_
#define _DECPOINTERLIST_H_

#include "decCollectionInterfaces.h"


/**
 * \brief List of pointers.
 * 
 * All pointers including NULL are allowed. Pointers can be included
 * multiple times in the list.
 */
class DE_DLL_EXPORT decPointerList{
private:
	void **pPointers;
	int pPointerCount;
	int pPointerSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new list. */
	decPointerList();
	
	/**
	 * \brief Create a new list with an initial capacity.
	 * \throws deeInvalidParam \em capacity is less than 0.
	 */
	decPointerList( int capacity );
	
	/** \brief Create a copy of a list. */
	decPointerList( const decPointerList &list );
	
	/** \brief Clean up the list. */
	~decPointerList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of objects. */
	inline int GetCount() const{ return pPointerCount; }
	
	/**
	 * \brief Pointer at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void *GetAt( int position ) const;
	
	/**
	 * \brief Set pointer at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void SetAt( int position, void *pointer );
	
	/** \brief Index of the first occurance of a pointer or -1 if not found. */
	int IndexOf( void *pointer ) const;
	
	/**
	 * \brief Index of the first occurance of a pointer or -1 if not found.
	 * \throws deeInvalidParam \em start is less than 0 or larger than GetCount()-1.
	 */
	int IndexOf( void *pointer, int start ) const;
	
	/** \brief Determine if pointer exists in the list. */
	bool Has( void *pointer ) const;
	
	/** \brief Number of times pointer exists in the list. */
	int CountOccurance( void *pointer ) const;
	
	/** \brief Add pointer. */
	void Add( void *pointer );
	
	/**
	 * \brief Insert pointer.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void Insert( void *pointer, int index );
	
	/**
	 * \brief Move pointer.
	 * \throws deeInvalidParam \em pointer is not part of the list.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void Move( void *pointer, int index );
	
	/**
	 * \brief Move pointer.
	 * \throws deeInvalidParam \em from is less than 0 or larger than GetCount()-1.
	 * \throws deeInvalidParam \em to is less than 0 or larger than GetCount()-1.
	 */
	void Move( int from, int to );
	
	/**
	 * \brief Remove pointer from index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void RemoveFrom( int position );
	
	/** \brief Remove all pointers. */
	void RemoveAll();
	
	/** \brief Determine if this list is equal to another list. */
	bool Equals( const decPointerList &list ) const;
	
	/**
	 * \brief New list with the values from the beginning of this list.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	decPointerList GetHead( int count ) const;
	
	/**
	 * \brief Set list to values from the beginning of this list.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void GetHead( decPointerList &list, int count ) const;
	
	/**
	 * \brief New list with values from the end of this list.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	decPointerList GetTail( int count ) const;
	
	/**
	 * \brief Set list to values from the end of this list.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void GetTail( decPointerList &list, int count ) const;
	
	/**
	 * \brief New list with values from the middle of this list.
	 * 
	 * \em from and \em to are clamped to the last index in the list if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 */
	decPointerList GetMiddle( int from, int to ) const;
	
	/**
	 * \brief Set list to values from the middle of this list.
	 * 
	 * \em from and \em to are clamped to the last index in the list if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 */
	void GetMiddle( decPointerList &list, int from, int to ) const;
	
	/**
	 * \brief New list with values from the middle of this list using a step size.
	 * 
	 * \em from and \em to are clamped to the last index in the list if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 * \throws deeInvalidParam \em step is less than 1.
	 */
	decPointerList GetSliced( int from, int to, int step ) const;
	
	/**
	 * \brief Set list to values from the middle of this list using a step size.
	 * 
	 * \em from and \em to are clamped to the last index in the list if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 * \throws deeInvalidParam \em step is less than 1.
	 */
	void GetSliced( decPointerList &list, int from, int to, int step ) const;
	
	
	
	/**
	 * \brief Visit pointers.
	 * \param[in] visitor Visitor.
	 * \param[in] from First index to visit. Negative counts from end of list.
	 * \param[in] to One past last index to visit. Negative counts from end of list.
	 * \param[in] step Step size. Can be negative but not 0.
	 */
	void Visit(decPointerVisitor &visitor, int from, int to = -1, int step = 1) const;
	
	inline void Visit(decPointerVisitor &visitor) const{ Visit(visitor, 0, pPointerCount); }
	
	/**
	 * \brief Find pointer.
	 * \param[in] evaluator Evaluator.
	 * \param[out] found Found pointer if true is returned.
	 * \param[in] from First index to visit. Negative counts from end of list.
	 * \param[in] to One past last index to visit. Negative counts from end of list.
	 * \param[in] step Step size. Can be negative but not 0.
	 */
	bool Find(decPointerEvaluator &evaluator, void *&found,
		int from = 0, int to = -1, int step = 1) const;
	
	inline bool Find(decPointerEvaluator &evaluator, void *&found) const{
		return Find(evaluator, found, 0, pPointerCount);
	}
	
	/**
	 * \brief Collect pointer into a new list.
	 * \param[in] evaluator Evaluator.
	 * \param[in] from First index to visit. Negative counts from end of list.
	 * \param[in] to One past last index to visit. Negative counts from end of list.
	 * \param[in] step Step size. Can be negative but not 0.
	 */
	decPointerList Collect(decPointerEvaluator &evaluator, int from, int to = -1, int step = 1) const;
	
	inline decPointerList Collect(decPointerEvaluator &evaluator) const{
		return Collect(evaluator, 0, pPointerCount);
	}
	
	/**
	 * \brief Remove pointers matching condition.
	 * \param[in] evaluator Evaluator.
	 * \param[in] from First index to visit. Negative counts from end of list.
	 * \param[in] to One past last index to visit. Negative counts from end of list.
	 * \param[in] step Step size. Can be negative but not 0.
	 */
	void RemoveIf(decPointerEvaluator &evaluator, int from, int to = -1, int step = 1);
	
	inline void RemoveIf(decPointerEvaluator &evaluator){ RemoveIf(evaluator, 0, pPointerCount); }
	
	/** \brief Sort pointers in place. */
	void Sort(decPointerComparator &comparator);
	
	/** \brief Sort pointers as new list. */
	decPointerList GetSorted(decPointerComparator &comparator) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Determine if this list is equal to another list. */
	bool operator==( const decPointerList &list ) const;
	
	/** \brief New list containing all values of this list followed by all values of another list. */
	decPointerList operator+( const decPointerList &list ) const;
	
	/**
	 * \brief Pointer at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void *operator[]( int position ) const;
	
	/**
	 * \brief Set pointer at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void *&operator[]( int position );
	
	/** \brief Copy list to this list. */
	decPointerList &operator=( const decPointerList &list );
	
	/** \brief Append values of list to this list. */
	decPointerList &operator+=( const decPointerList &list );
	/*@}*/
	
	
private:
	void pSort(decPointerComparator &comparator, int left, int right);
};

#endif
