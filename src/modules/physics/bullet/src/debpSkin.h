/* 
 * Drag[en]gine Bullet Physics Module
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
#ifndef _DEBPSKIN_H_
#define _DEBPSKIN_H_

// includes
#include "dragengine/systems/modules/physics/deBasePhysicsSkin.h"
#include "dragengine/common/math/decMath.h"

// predefintions
class deSkin;



/**
 * @brief Bullet Physics Skin Peer
 * The peer for skin resources in the ODE Physics Module. The main
 * purpose of this class is to provide an octree of faces for quick
 * collision detection if the skin is a simple skin. Complex skins
 * have to be stored inside the Component Peer. Simple skins are
 * much quicker as they do not change over time. A skin is considered
 * simple if it has no weights that could influence the vertices.
 */
class debpSkin : public deBasePhysicsSkin{
private:
	deSkin *pSkin;
	
public:
	// constructor, destructor
	debpSkin( deSkin *skin );
	~debpSkin();
	// management
	inline deSkin *GetSkin() const{ return pSkin; }
	
private:
	void pCleanUp();
};

// end of include only once
#endif
