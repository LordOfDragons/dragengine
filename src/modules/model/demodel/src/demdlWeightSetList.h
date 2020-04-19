/* 
 * Drag[en]gine Model Module
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

#ifndef _DEMDLWEIGHTSETLIST_H_
#define _DEMDLWEIGHTSETLIST_H_

#include <dragengine/common/collection/decPointerList.h>

class demdlWeightSet;



/**
 * @brief Weight Set List.
 * List of weight sets. Supports sorting to output the weight sets in the way the model resource expects them.
 */
class demdlWeightSetList{
private:
	decPointerList pList;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new weight set list. */
	demdlWeightSetList();
	/** Cleans up the list. */
	~demdlWeightSetList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the count of weight sets. */
	int GetCount() const;
	/** Retrieves the weight set at the given index. */
	demdlWeightSet *GetAt( int index ) const;
	/** Adds a weight set. */
	void Add( demdlWeightSet *weightSet );
	/** Remove all weight sets. */
	void RemoveAll();
	/** Retrieves the index of the weight set which is equal to the given weight set or -1 if not found. */
	int IndexOfEqual( const demdlWeightSet &weightSet ) const;
	/** Retrieves the largest number of weights in all of the weight sets. */
	int GetLargestWeightCount() const;
	/*@}*/
};

#endif
