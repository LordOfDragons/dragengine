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

#ifndef _MEUSHORTARRAY_H_
#define _MEUSHORTARRAY_H_



/**
 * \brief Unsigned short array.
 */
class meUShortArray{
private:
	int pColons;
	int pRows;
	unsigned short *pValues;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create array. */
	meUShortArray(int colons, int rows);
	
	/** \brief Create copy of array. */
	meUShortArray(const meUShortArray &array);
	
	/** \brief Clean up array. */
	~meUShortArray();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of colons. */
	inline int GetColons() const{ return pColons; }
	
	/** \brief Number of rows. */
	inline int GetRows() const{ return pRows; }
	
	/** \brief Value at coordinate. */
	int GetValueAt(int x, int y) const;
	
	/** \brief Set value at coordinate. */
	void SetValueAt(int x, int y, int value);
	
	/** \brief Set all values. */
	void SetAll(int value);
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Arrays are equal. */
	bool operator==(const meUShortArray &array) const;
	
	/** \brief Copy operator. */
	meUShortArray &operator=(const meUShortArray &array);
	/*@}*/
};

#endif
