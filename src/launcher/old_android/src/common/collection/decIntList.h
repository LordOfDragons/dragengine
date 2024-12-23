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

#ifndef _DECINTLIST_H_
#define _DECINTLIST_H_



/**
 * \brief List of integer values.
 */
class decIntList{
private:
	int *pValues;
	int pValueCount;
	int pValueSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new list. */
	decIntList();
	
	/**
	 * \brief Create a new list with an initial capacity.
	 * \throws deeInvalidParam \em capacity is less than 0.
	 */
	decIntList( int capacity );
	
	/** \brief Create a copy of a list. */
	decIntList( const decIntList &list );
	
	/** \brief Clean up the list. */
	~decIntList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Number of values. */
	inline int GetCount() const{ return pValueCount; }
	
	/**
	 * \brief Value at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	int GetAt( int index ) const;
	
	/**
	 * \brief Set value at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void SetAt( int index, int value );
	
	/** \brief Index of the first occurance of an value or -1 if not found. */
	int IndexOf( int value ) const;
	
	/**
	 * \brief Index of the first occurance of value or -1 if not found.
	 * \throws deeInvalidParam \em start is less than 0 or larger than GetCount()-1.
	 */
	int IndexOf( int value, int start ) const;
	
	/** \brief Determine if value exists at least once in the list. */
	bool Has( int value ) const;
	
	/** \brief Number of times value exists in the list. */
	int CountOccurance( int value ) const;
	
	/** \brief Add value. */
	void Add( int value );
	
	/**
	 * \brief Insert value.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void Insert( int value, int index );
	
	/**
	 * \brief Move value.
	 * \throws deeInvalidParam \em from is less than 0 or larger than GetCount()-1.
	 * \throws deeInvalidParam \em to is less than 0 or larger than GetCount().
	 */
	void Move( int from, int to );
	
	/**
	 * \brief Remove value from index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void RemoveFrom( int index );
	
	/** \brief Remove all values. */
	void RemoveAll();
	
	/** \brief Determine if this list is equal to another list. */
	bool Equals( const decIntList &list ) const;
	
	/**
	 * \brief New list with the values from the beginning of this list.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	decIntList GetHead( int count ) const;
	
	/**
	 * \brief Set list to values from the beginning of this list.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void GetHead( decIntList &list, int count ) const;
	
	/**
	 * \brief New list with values from the end of this list.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	decIntList GetTail( int count ) const;
	
	/**
	 * \brief Set list to values from the end of this list.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void GetTail( decIntList &list, int count ) const;
	
	/**
	 * \brief New list with values from the middle of this list.
	 * \details \em from and \em to are clamped to the last index in the list if larger.
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 */
	decIntList GetMiddle( int from, int to ) const;
	
	/**
	 * \brief Set list to values from the middle of this list.
	 * \details \em from and \em to are clamped to the last index in the list if larger.
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 */
	void GetMiddle( decIntList &list, int from, int to ) const;
	
	/**
	 * \brief New list with values from the middle of this list using a step size.
	 * \details \em from and \em to are clamped to the last index in the list if larger.
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 * \throws deeInvalidParam \em step is less than 1.
	 */
	decIntList GetSliced( int from, int to, int step ) const;
	
	/**
	 * \brief Set list to values from the middle of this list using a step size.
	 * \details \em from and \em to are clamped to the last index in the list if larger.
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 * \throws deeInvalidParam \em step is less than 1.
	 */
	void GetSliced( decIntList &list, int from, int to, int step ) const;
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** \brief Determine if this list is equal to another list. */
	bool operator==( const decIntList &list ) const;
	
	/** \brief New list containing all values of this list followed by all values of another list. */
	decIntList operator+( const decIntList &list ) const;
	
	/**
	 * \brief Value at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	int operator[]( int index ) const;
	
	/** \brief Copy list to this list. */
	decIntList &operator=( const decIntList &list );
	
	/** \brief Append values of list to this list. */
	decIntList &operator+=( const decIntList &list );
	/*@}*/
};

#endif
