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

// include only once
#ifndef _MEFILTERLISTOBJECTS_H_
#define _MEFILTERLISTOBJECTS_H_

// includes
#include "meFilterObjects.h"

// predefinitions
class meObject;



/**
 * @brief Filter List Objects.
 *
 * A list of object filters acting as one object filter. For each
 * object the list of filters is checked. If the acceptAny flag
 * is set an object is accepted if at last one filter in the list
 * accepts the object. Otherwise the object is only accepted if
 * all filters accept it. By default acceptAny is true.
 */
class meFilterListObjects : public meFilterObjects{
	meFilterObjects **pFilters;
	int pFilterCount;
	
	bool pAcceptAny;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new filter list. */
	meFilterListObjects();
	/** Cleans up the filter list. */
	virtual ~meFilterListObjects();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Determines if any filter can accept the object or only all together. */
	inline bool GetAcceptAny() const{ return pAcceptAny; }
	/** Sets if any filter can accept the object or only all together. */
	void SetAcceptAny( bool acceptAny );
	
	/** Adds a filter. */
	void AddFilter( meFilterObjects *filter );
	/** Removes all filters. */
	void RemoveAllFilters();
	
	/**
	 * Determines if the given object is accepted by the filter.
	 */
	virtual bool AcceptObject( meObject *object ) const;
	/*@}*/
};

// end of include only once
#endif
