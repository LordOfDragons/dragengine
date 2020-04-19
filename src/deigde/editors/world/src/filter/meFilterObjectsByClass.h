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
#ifndef _MEFILTEROBJECTSBYCLASS_H_
#define _MEFILTEROBJECTSBYCLASS_H_

// includes
#include "meFilterObjects.h"

#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringSet.h>

// predefinitions
class meObject;



/**
 * @brief Filter Objects By Class.
 *
 * A filter accepting objects based on their class name. Supports
 * inclusive or exclusive matching. Using inclusive matching only
 * objects with a class name matching any of the specified class
 * names are accepted. Using exclusive matching only objects with
 * a class name not matching any of the specified class names are
 * accepted. By default exclusive matching is used. In addition
 * an object to ignore can be specified. This is useful for
 * situations where an object should not accept itself. Furthermore
 * ghost objects can be accepted if required. By default ghost
 * objects are rejected.
 */
class meFilterObjectsByClass : public meFilterObjects{
private:
	decStringSet pClassNames;
	bool pMatchInclusive;
	
	meObject *pRejectObject;
	bool pRejectGhosts;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new filter. */
	meFilterObjectsByClass();
	/** Cleans up the filter. */
	virtual ~meFilterObjectsByClass();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Determines if inclusive or exlusive matching has to be used. */
	inline bool GetMatchInclusive() const{ return pMatchInclusive; }
	/** Sets if inclusive or exclusive matching has to be used. */
	void SetMatchInclusive( bool matchInclusive );
	/** Determines if ghost objects are rejected. */
	inline bool GetRejectGhosts() const{ return pRejectGhosts; }
	/** Sets if ghost objects are rejected. */
	void SetRejectGhosts( bool rejectGhosts );
	/** Retrieves the object to reject or NULL. */
	inline meObject *GetRejectObject() const{ return pRejectObject; }
	/** Sets the object to reject or NULL. */
	void SetRejectObject( meObject *object );
	
	/** Adds a class name. */
	void AddClassName( const char *className );
	/** Removes all class names. */
	void RemoveAllClassNames();
	/** Sets class names set. */
	void SetClassNamesFrom( const decStringSet &set );
	
	/**
	 * Determines if the given object is accepted by the filter.
	 */
	virtual bool AcceptObject( meObject *object ) const;
	/*@}*/
};

// end of include only once
#endif
