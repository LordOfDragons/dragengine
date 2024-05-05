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
#ifndef _MEHTVRLINK_H_
#define _MEHTVRLINK_H_

// includes
#include <dragengine/deObject.h>

// predefinitions
class meHTVRule;



/**
 * @brief Height Terrain Vegetation Rule Link.
 *
 * Link between two height terrain vegeation rules.
 */
class meHTVRLink : public deObject{
private:
	meHTVRule *pSrcRule;
	int pSrcSlot;
	meHTVRule *pDestRule;
	int pDestSlot;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a link. */
	meHTVRLink( meHTVRule *sourceRule, int sourceSlot, meHTVRule *destinationRule, int destinationSlot );
	/** Cleans up the link. */
	virtual ~meHTVRLink();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the source rule. */
	inline meHTVRule *GetSourceRule() const{ return pSrcRule; }
	/** Retrieves the source slot. */
	inline int GetSourceSlot() const{  return pSrcSlot; }
	/** Retrieves the destination rule. */
	inline meHTVRule *GetDestinationRule() const{ return pDestRule; }
	/** Retrieves the destination slot. */
	inline int GetDestinationSlot() const{  return pDestSlot; }
	/*@}*/
};

// end of include only once
#endif
