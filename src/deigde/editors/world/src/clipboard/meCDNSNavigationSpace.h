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

#ifndef _MECDNSNAVSPACE_H_
#define _MECDNSNAVSPACE_H_

#include <dragengine/common/math/decMath.h>

class meNavigationSpace;



/**
 * \brief Navigation space clipboard clip.
 */
class meCDNSNavigationSpace {
private:
	decDVector pPosition;
	decVector pOrientation;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create navigation space clip data. */
	meCDNSNavigationSpace( meNavigationSpace *navspace );
	
	/** \brief Clean up clip data. */
	~meCDNSNavigationSpace();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Orientation. */
	inline const decVector &GetOrientation() const{ return pOrientation; }
	
	/** \brief Update navigation space with stored data. */
	void UpdateNavSpace( meNavigationSpace &navspace ) const;
	/*@}*/
};

#endif
