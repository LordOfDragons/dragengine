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

#ifndef _DECINTORDEREDSET_H_
#define _DECINTORDEREDSET_H_



/**
 * \brief Ordered set of integer values. Each value can exists only once.
 */
class decIntOrderedSet{
private:
	int *pValues;
	int pValueCount;
	int pValueSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new set. */
	decIntOrderedSet();
	
	/**
	 * \brief Creates a new set with initial capacity.
	 * \throws deeInvalidParam \em capacity is less than 0.
	 */
	decIntOrderedSet( int capacity );
	
	/** \brief Creates a copy of a set. */
	decIntOrderedSet( const decIntOrderedSet &set );
	
	/** \brief Clean up the set. */
	~decIntOrderedSet();
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
	
	/** \brief Index of the a value or -1 if not found. */
	int IndexOf( int value ) const;
	
	/** \brief Determine if a value exists. */
	bool Has( int value ) const;
	
	/**
	 * \brief Add value.
	 * \throws deeInvalidParam \em value is contained in the set.
	 */
	void Add( int value );
	
	/** \brief Add value if not existing. */
	void AddIfAbsent( int value );
	
	/**
	 * \brief Insert value.
	 * \throws deeInvalidParam \em value is contained in the set.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void Insert( int value, int index );
	
	/**
	 * \brief Move value.
	 * \throws deeInvalidParam \em from is less than 0 or larger than GetCount()-1.
	 * \throws deeInvalidParam \em to is less than 0 or larger than GetCount().
	 */
	void Move( int value, int to );
	
	/**
	 * \brief Remove value.
	 * \throws deeInvalidParam \em value is absent from the set.
	 */
	void Remove( int value );
	
	/** \brief Remove value if present. */
	void RemoveIfPresent( int value );
	
	/** \brief Removes all values. */
	void RemoveAll();
	
	/** \brief Determine if this set is equal to another set. */
	bool Equals( const decIntOrderedSet &set ) const;
	
	/**
	 * \brief New set with values from the beginning of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	decIntOrderedSet GetHead( int count ) const;
	
	/**
	 * \brief Set set to values from the beginning of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void GetHead( decIntOrderedSet &list, int count ) const;
	
	/**
	 * \brief New set with values from the end of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	decIntOrderedSet GetTail( int count ) const;
	
	/**
	 * \brief Set set to values from the end of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void GetTail( decIntOrderedSet &list, int count ) const;
	
	/**
	 * \brief New set with values from the middle of this set.
	 * \details \em from and \em to are clamped to the last index in the set if larger.
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 */
	decIntOrderedSet GetMiddle( int from, int to ) const;
	
	/**
	 * \brief Set set to values from the middle of this set.
	 * \details \em from and \em to are clamped to the last index in the set if larger.
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 */
	void GetMiddle( decIntOrderedSet &list, int from, int to ) const;
	
	/**
	 * \brief New set with values from the middle of this set using a step size.
	 * \details \em from and \em to are clamped to the last index in the list if larger.
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 * \throws deeInvalidParam \em step is less than 1.
	 */
	decIntOrderedSet GetSliced( int from, int to, int step ) const;
	
	/**
	 * \brief Set set to values from the middle of this set using a step size.
	 * \details \em from and \em to are clamped to the last index in the list if larger.
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 * \throws deeInvalidParam \em step is less than 1.
	 */
	void GetSliced( decIntOrderedSet &list, int from, int to, int step ) const;
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** \brief Determine if this set is equal to another set. */
	bool operator==( const decIntOrderedSet &set ) const;
	
	/** \brief New set with all values from this set followed by all values of another set. */
	decIntOrderedSet operator+( const decIntOrderedSet &set ) const;
	
	/**
	 * \brief Value at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	int operator[]( int index ) const;
	
	/** \brief Copy set to this set. */
	decIntOrderedSet &operator=( const decIntOrderedSet &set );
	
	/** \brief Append absent values of set to this set. */
	decIntOrderedSet &operator+=( const decIntOrderedSet &set );
	/*@}*/
};

#endif
