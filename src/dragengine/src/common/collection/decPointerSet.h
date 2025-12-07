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

#ifndef _DECPOINTERSET_H_
#define _DECPOINTERSET_H_

#include "decCollectionInterfaces.h"


/**
 * \brief Set of pointers.
 * 
 * All pointers including NULL are allowed and they can occure only once in the set.
 */
class DE_DLL_EXPORT decPointerSet{
private:
	void **pPointers;
	int pPointerCount;
	int pPointerSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new set. */
	decPointerSet();
	
	/**
	 * \brief Create a new set with initial capacity.
	 * \throws deeInvalidParam \em capacity is less than 0.
	 */
	decPointerSet(int capacity);
	
	/** \brief Create copy of a set. */
	decPointerSet(const decPointerSet &set);
	
	/** \brief Clean up the set. */
	~decPointerSet();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of objects. */
	inline int GetCount() const{ return pPointerCount; }
	
	/**
	 * \brief Pointer at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void *GetAt(int position) const;
	
	/** \brief Determine if pointer exists in the list. */
	bool Has(void *pointer) const;
	
	/**
	 * \brief Add pointer.
	 * \throws deeInvalidParam \em object is present in the set.
	 */
	void Add(void *pointer);
	
	/** \brief Add pointer if absent from the set. */
	void AddIfAbsent(void *pointer);
	
	/**
	 * \brief Remove pointer.
	 * \throws deeInvalidParam \em object is is absent from the set.
	 */
	void Remove(void *pointer);
	
	/** \brief Remove pointer if present in the set. */
	void RemoveIfPresent(void *pointer);
	
	/** \brief Remove all pointers. */
	void RemoveAll();
	
	/** \brief Determine if this set is equal to another set. */
	bool Equals(const decPointerSet &set) const;
	
	
	
	/**
	 * \brief Visit pointers.
	 * \param[in] visitor Visitor.
	 * \param[in] from First index to visit. Negative counts from end of list.
	 * \param[in] to One past last index to visit. Negative counts from end of list.
	 * \param[in] step Step size. Can be negative but not 0.
	 */
	void Visit(decPointerVisitor &visitor, int from, int to = -1, int step = 1) const;
	
	inline void Visit(decPointerVisitor &visitor) const{Visit(visitor, 0, pPointerCount);}
	
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
	decPointerSet Collect(decPointerEvaluator &evaluator, int from, int to = -1, int step = 1) const;
	
	inline decPointerSet Collect(decPointerEvaluator &evaluator) const{
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
	
	inline void RemoveIf(decPointerEvaluator &evaluator){RemoveIf(evaluator, 0, pPointerCount);}
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Determine if this set is equal to another set. */
	bool operator==(const decPointerSet &set) const;
	
	/** \brief New set containing all objects of this set followed by all objects of another set. */
	decPointerSet operator+(const decPointerSet &set) const;
	
	/**
	 * \brief Object at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void *operator[](int position) const;
	
	/** \brief Copy set to this set. */
	decPointerSet &operator=(const decPointerSet &set);
	
	/** \brief Append objects of set to this set. */
	decPointerSet &operator+=(const decPointerSet &set);
	/*@}*/
	
	
	
private:
	/** \brief Index of the first occurance of a pointer or -1 if not found. */
	int pIndexOf(void *pointer) const;
};

#endif
