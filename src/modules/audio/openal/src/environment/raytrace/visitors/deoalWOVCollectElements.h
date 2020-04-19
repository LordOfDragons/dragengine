/* 
 * Drag[en]gine OpenAL Audio Module
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

#ifndef _DEOALWOVCOLLECTELEMENTS_H_
#define _DEOALWOVCOLLECTELEMENTS_H_

#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>

#include "../../../world/octree/deoalWorldOctreeVisitor.h"

class deoalAComponent;



/**
 * \brief World octree visitor collecting elements.
 * 
 * Visits octree with box. All elements hit by the box are stored in a list.
 */
class deoalWOVCollectElements : public deoalWorldOctreeVisitor{
private:
	decDVector pBoxMin;
	decDVector pBoxMax;
	decPointerList pComponents;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	deoalWOVCollectElements();
	
	/** \brief Clean up visitor. */
	virtual ~deoalWOVCollectElements();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Box minimum. */
	inline const decDVector &GetBoxMin() const{ return pBoxMin; }
	
	/** \brief Box maximum. */
	inline const decDVector &GetBoxMax() const{ return pBoxMax; }
	
	/** \brief Set box. */
	void SetBox( const decDVector &minimum, const decDVector &maximum );
	
	/** \brief Set box. */
	void SetBoxRange( const decDVector &position, double range );
	
	/** \brief Reset. */
	void Reset();
	
	
	
	/** \brief Number of components. */
	int GetComponentCount() const;
	
	/** \brief Component at index. */
	deoalAComponent *GetComponentAt( int index ) const;
	
	
	
	/** \brief Visit component. */
	virtual void VisitComponent( deoalAComponent *component );
	/*@}*/
};

#endif
