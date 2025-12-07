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

#ifndef _DECINTSET_H_
#define _DECINTSET_H_



/**
 * @brief Int Set.
 * Set of int values.
 */
class decIntSet{
private:
	int *pValues;
	int pValueCount;
	int pValueSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new set. */
	decIntSet();
	
	/**
	 * \brief Creates a new set with initial capacity.
	 * \throws deeInvalidParam \em capacity is less than 0.
	 */
	decIntSet(int capacity);
	
	/** \brief Creates a copy of a set. */
	decIntSet(const decIntSet &set);
	
	/** \brief Clean up the set. */
	~decIntSet();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** \brief Number of values. */
	inline int GetCount() const{ return pValueCount; }
	
	/**
	 * \brief Value at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	int GetAt(int index) const;
	
	/** \brief Determine if a value exists. */
	bool Has(int value) const;
	
	/**
	 * \brief Add value.
	 * \throws deeInvalidParam \em value is contained in the set.
	 */
	void Add(int value);
	
	/** \brief Add value if not existing. */
	void AddIfAbsent(int value);
	
	/**
	 * \brief Remove value.
	 * \throws deeInvalidParam \em value is absent from the set.
	 */
	void Remove(int value);
	
	/** \brief Remove value if present. */
	void RemoveIfPresent(int value);
	
	/** \brief Removes all values. */
	void RemoveAll();
	
	/** \brief Determine if this set is equal to another set. */
	bool Equals(const decIntSet &set) const;
	/*@}*/
	
	/** @name Operators */
	/*@{*/
	/** \brief Determine if this set is equal to another set. */
	bool operator==(const decIntSet &set) const;
	
	/** \brief New set with all values from this set followed by all values of another set. */
	decIntSet operator+(const decIntSet &set) const;
	
	/**
	 * \brief Value at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	int operator[](int position) const;
	
	/** \brief Copy set to this set. */
	decIntSet &operator=(const decIntSet &set);
	
	/** \brief Append absent values of set to this set. */
	decIntSet &operator+=(const decIntSet &set);
	/*@}*/
	
private:
	int pIndexOf(int value) const;
};

#endif
