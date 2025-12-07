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



/**
 * \brief Set of pointers.
 * 
 * All pointers including \em NULL are allowed and they can occure only once in the set.
 */
class decPointerSet{
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
	
	/** \brief Index of the first occurance of a pointer or -1 if not found. */
	int IndexOf(void *pointer) const;
	
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
	 * \brief Remove pointer from index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void RemoveFrom(int position);
	
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
	 * \brief New set with the values from the beginning of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	decPointerSet GetHead(int count) const;
	
	/**
	 * \brief Set set to values from the beginning of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void GetHead(decPointerSet &set, int count) const;
	
	/**
	 * \brief New set with values from the end of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	decPointerSet GetTail(int count) const;
	
	/**
	 * \brief Set set to values from the end of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void GetTail(decPointerSet &set, int count) const;
	
	/**
	 * \brief New set with values from the middle of this set.
	 * \details \em from and \em to are clamped to the last index in the set if larger.
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 */
	decPointerSet GetMiddle(int from, int to) const;
	
	/**
	 * \brief Set set to values from the middle of this set.
	 * \details \em from and \em to are clamped to the last index in the set if larger.
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 */
	void GetMiddle(decPointerSet &set, int from, int to) const;
	
	/**
	 * \brief New set with values from the middle of this set using a step size.
	 * \details \em from and \em to are clamped to the last index in the set if larger.
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 * \throws deeInvalidParam \em step is less than 1.
	 */
	decPointerSet GetSliced(int from, int to, int step) const;
	
	/**
	 * \brief Set set to values from the middle of this set using a step size.
	 * \details \em from and \em to are clamped to the last index in the set if larger.
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 * \throws deeInvalidParam \em step is less than 1.
	 */
	void GetSliced(decPointerSet &set, int from, int to, int step) const;
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
};

#endif
