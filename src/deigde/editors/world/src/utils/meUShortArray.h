/* 
 * Drag[en]gine IGDE World Editor
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
	meUShortArray( int colons, int rows );
	
	/** \brief Create copy of array. */
	meUShortArray( const meUShortArray &array );
	
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
	int GetValueAt( int x, int y ) const;
	
	/** \brief Set value at coordinate. */
	void SetValueAt( int x, int y, int value );
	
	/** \brief Set all values. */
	void SetAll( int value );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Arrays are equal. */
	bool operator==( const meUShortArray &array ) const;
	
	/** \brief Copy operator. */
	meUShortArray &operator=( const meUShortArray &array );
	/*@}*/
};

#endif
