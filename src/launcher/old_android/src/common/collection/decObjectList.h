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

#ifndef _DECOBJECTLIST_H_
#define _DECOBJECTLIST_H_

class deObject;



/**
 * \brief List of objects.
 * 
 * All objects including \em NULL are allowed. Objects can be included
 * multiple times in the list.
 */
class decObjectList{
private:
	deObject **pObjects;
	int pObjectCount;
	int pObjectSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new list. */
	decObjectList();
	
	/**
	 * \brief Create a new list with an initial capacity.
	 * \throws deeInvalidParam \em capacity is less than 0.
	 */
	decObjectList(int capacity);
	
	/** \brief Create a copy of a list. */
	decObjectList(const decObjectList &list);
	
	/** \brief Clean up the list. */
	~decObjectList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Number of objects. */
	inline int GetCount() const{return pObjectCount;}
	
	/**
	 * \brief Object at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	deObject *GetAt(int index) const;
	
	/**
	 * \brief Set object at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void SetAt(int index, deObject *object);
	
	/** \brief Index of the first occurance of an object or -1 if not found. */
	int IndexOf(deObject *object) const;
	
	/**
	 * \brief Index of the first occurance of an object or -1 if not found.
	 * \throws deeInvalidParam \em start is less than 0 or larger than GetCount()-1.
	 */
	int IndexOf(deObject *object, int start) const;
	
	/** \brief Determine if object exists in the list. */
	bool Has(deObject *object) const;
	
	/** \brief Number of times object exists in the list. */
	int CountOccurance(deObject *object) const;
	
	/** \brief Add object. */
	void Add(deObject *object);
	
	/**
	 * \brief Insert object.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void Insert(deObject *object, int index);
	
	/**
	 * \brief Move object.
	 * \throws deeInvalidParam \em from is less than 0 or larger than GetCount()-1.
	 * \throws deeInvalidParam \em to is less than 0 or larger than GetCount().
	 */
	void Move(int from, int to);
	
	/**
	 * \brief Remove object from index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void RemoveFrom(int index);
	
	/** \brief Remove all objects. */
	void RemoveAll();
	
	/** \brief Determine if this list is equal to another list. */
	bool Equals(const decObjectList &list) const;
	
	/**
	 * \brief New list with the values from the beginning of this list.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	decObjectList GetHead(int count) const;
	
	/**
	 * \brief Set list to values from the beginning of this list.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void GetHead(decObjectList &list, int count) const;
	
	/**
	 * \brief New list with values from the end of this list.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	decObjectList GetTail(int count) const;
	
	/**
	 * \brief Set list to values from the end of this list.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void GetTail(decObjectList &list, int count) const;
	
	/**
	 * \brief New list with values from the middle of this list.
	 * \details \em from and \em to are clamped to the last index in the list if larger.
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 */
	decObjectList GetMiddle(int from, int to) const;
	
	/**
	 * \brief Set list to values from the middle of this list.
	 * \details \em from and \em to are clamped to the last index in the list if larger.
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 */
	void GetMiddle(decObjectList &list, int from, int to) const;
	
	/**
	 * \brief New list with values from the middle of this list using a step size.
	 * \details \em from and \em to are clamped to the last index in the list if larger.
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 * \throws deeInvalidParam \em step is less than 1.
	 */
	decObjectList GetSliced(int from, int to, int step) const;
	
	/**
	 * \brief Set list to values from the middle of this list using a step size.
	 * \details \em from and \em to are clamped to the last index in the list if larger.
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 * \throws deeInvalidParam \em step is less than 1.
	 */
	void GetSliced(decObjectList &list, int from, int to, int step) const;
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** \brief Determine if this list is equal to another list. */
	bool operator==(const decObjectList &list) const;
	
	/** \brief New list containing all values of this list followed by all values of another list. */
	decObjectList operator+(const decObjectList &list) const;
	
	/**
	 * \brief Object at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	deObject *operator[](int position) const;
	
	/** \brief Copy list to this list. */
	decObjectList &operator=(const decObjectList &list);
	
	/** \brief Append values of list to this list. */
	decObjectList &operator+=(const decObjectList &list);
	/*@}*/
};

#endif
