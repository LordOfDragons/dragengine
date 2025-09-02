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

#ifndef _DECPOINTERORDEREDSET_H_
#define _DECPOINTERORDEREDSET_H_

#include "decCollectionInterfaces.h"


/**
 * \brief Ordered set of pointers.
 * 
 * All pointers including NULL are allowed. Pointers can be included
 * only once in the set.
 */
class DE_DLL_EXPORT decPointerOrderedSet{
private:
	void **pPointers;
	int pPointerCount;
	int pPointerSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new set. */
	decPointerOrderedSet();
	
	/**
	 * \brief Create a new set with initial capacity.
	 * \throws deeInvalidParam \em capacity is less than 0.
	 */
	decPointerOrderedSet( int capacity );
	
	/** \brief Create copy of a set. */
	decPointerOrderedSet( const decPointerOrderedSet &set );
	
	/** \brief Clean up the set. */
	~decPointerOrderedSet();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of pointers. */
	inline int GetCount() const{ return pPointerCount; }
	
	/**
	 * \brief Pointer at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void *GetAt( int index ) const;
	
	/** \brief Index of the first occurance of an pointer or -1 if not found. */
	int IndexOf( void *pointer ) const;
	
	/** \brief Determine if pointer exists in the list. */
	bool Has( void *pointer ) const;
	
	/**
	 * \brief Add pointer.
	 * \throws deeInvalidParam \em pointer is present in the set.
	 */
	void Add( void *pointer );
	
	/** \brief Add pointer if absent from the set. */
	void AddIfAbsent( void *pointer );
	
	/**
	 * \brief Insert pointer.
	 * \throws deeInvalidParam \em pointer is present in the set.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void Insert( void *pointer, int index );
	
	/**
	 * \brief Move pointer.
	 * \throws deeInvalidParam \em from is less than 0 or larger than GetCount()-1.
	 * \throws deeInvalidParam \em to is less than 0 or larger than GetCount().
	 */
	void Move( void *pointer, int to );
	
	/**
	 * \brief Remove pointer.
	 * \throws deeInvalidParam \em pointer is is absent from the set.
	 */
	void Remove( void *pointer );
	
	/** \brief Remove pointer if present in the set. */
	void RemoveIfPresent( void *pointer );
	
	/**
	 * \brief Remove pointer from index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void RemoveFrom( int index );
	
	/** \brief Remove all pointers. */
	void RemoveAll();
	
	/** \brief Determine if this set is equal to another set. */
	bool Equals( const decPointerOrderedSet &set ) const;
	
	/**
	 * \brief New set with the pointers from the beginning of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	decPointerOrderedSet GetHead( int count ) const;
	
	/**
	 * \brief Set set to pointers from the beginning of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void GetHead( decPointerOrderedSet &set, int count ) const;
	
	/**
	 * \brief New set with pointers from the end of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	decPointerOrderedSet GetTail( int count ) const;
	
	/**
	 * \brief Set set to pointers from the end of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void GetTail( decPointerOrderedSet &set, int count ) const;
	
	/**
	 * \brief New set with pointers from the middle of this set.
	 * 
	 * \em from and \em to are clamped to the last index in the set if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 */
	decPointerOrderedSet GetMiddle( int from, int to ) const;
	
	/**
	 * \brief Set set to pointers from the middle of this set.
	 * 
	 * \em from and \em to are clamped to the last index in the set if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 */
	void GetMiddle( decPointerOrderedSet &set, int from, int to ) const;
	
	/**
	 * \brief New set with pointers from the middle of this set using a step size.
	 * 
	 * \em from and \em to are clamped to the last index in the set if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 * \throws deeInvalidParam \em step is less than 1.
	 */
	decPointerOrderedSet GetSliced( int from, int to, int step ) const;
	
	/**
	 * \brief Set set to pointers from the middle of this set using a step size.
	 * 
	 * \em from and \em to are clamped to the last index in the set if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 * \throws deeInvalidParam \em step is less than 1.
	 */
	void GetSliced( decPointerOrderedSet &set, int from, int to, int step ) const;
	
	
	
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
	
	inline void Find(decPointerEvaluator &evaluator, void *&found) const{
		Find(evaluator, found, 0, pPointerCount);
	}
	
	/**
	 * \brief Collect pointer into a new list.
	 * \param[in] evaluator Evaluator.
	 * \param[in] from First index to visit. Negative counts from end of list.
	 * \param[in] to One past last index to visit. Negative counts from end of list.
	 * \param[in] step Step size. Can be negative but not 0.
	 */
	decPointerOrderedSet Collect(decPointerEvaluator &evaluator,
		int from = 0, int to = -1, int step = 1) const;
	
	inline decPointerOrderedSet Collect(decPointerEvaluator &evaluator) const{
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
	decPointerOrderedSet GetSorted(decPointerComparator &comparator) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Determine if this set is equal to another set. */
	bool operator==( const decPointerOrderedSet &set ) const;
	
	/** \brief New set containing all pointers of this set followed by all pointers of another set. */
	decPointerOrderedSet operator+( const decPointerOrderedSet &set ) const;
	
	/**
	 * \brief Pointer at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void *operator[]( int index ) const;
	
	/** \brief Copy set to this set. */
	decPointerOrderedSet &operator=( const decPointerOrderedSet &set );
	
	/** \brief Append pointers of set to this set. */
	decPointerOrderedSet &operator+=( const decPointerOrderedSet &set );
	/*@}*/
	
	
private:
	void pSort(decPointerComparator &comparator, int left, int right);
};

#endif
