/* 
 * Drag[en]gine AI Module
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

#ifndef _DEDAICOSTTABLE_H_
#define _DEDAICOSTTABLE_H_

#include <dragengine/common/collection/decIntOrderedSet.h>



/**
 * \brief Cost Table.
 * 
 * Definition of a cost table for a given layer. All navigation elements with the same layer
 * have the same layout of type entries as defined in the cost table. Changes to the cost
 * table are communicated to the individual navigation elements upon the next time the layer
 * is prepared for usage.
 */
class dedaiCostTable{
private:
	decIntOrderedSet pTypes;
	bool pChanged;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create cost table. */
	dedaiCostTable();
	
	/** \brief Clean up cost table. */
	~dedaiCostTable();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Cost table definition changed since the last time. */
	inline bool GetChanged() const{ return pChanged; }
	
	/** \brief Clear changed state. */
	void ClearChanged();
	
	
	
	/** \brief Number of types. */
	int GetTypeCount() const;
	
	/** \brief Type at index. */
	int GetTypeAt( int index ) const;
	
	/**
	 * \brief Index of type with type number.
	 * \details Adds type if absent.
	 */
	int GetTypeWith( int typeNumber );
	
	/**
	 * \brief Index of type with type number.
	 * \returns \em defaultValue if type is absent.
	 */
	int GetTypeWith( int typeNumber, int defaultValue ) const;
	
	/** \brief Remove all types. */
	void RemoveAllTypes();
	/*@}*/
};

#endif
