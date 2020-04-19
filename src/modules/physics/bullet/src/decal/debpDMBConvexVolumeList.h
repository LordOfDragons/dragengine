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
#ifndef _DEBPDMBCONVEXVOLUMELIST_H_
#define _DEBPDMBCONVEXVOLUMELIST_H_

// includes
#include "dragengine/common/math/decConvexVolumeList.h"



/**
 * @brief OpenGL Decal Mesh Builder Convex Volume List.
 * Specialized version of the convex volume list used by the
 * decal mesh builder.
 */
class debpDMBConvexVolumeList : public decConvexVolumeList{
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new decal mesh builder convex volume list. */
	debpDMBConvexVolumeList();
	/** Cleans up the decal mesh builder convex volume list. */
	virtual ~debpDMBConvexVolumeList();
	/*@}*/
	
	/** @name Subclassing */
	/*@{*/
	/**
	 * Creates a new convex volume face that originates from the given face.
	 * It is possible that the face is NULL in which case a default face
	 * has to be created. NULL faces typically occure if a split is done using
	 * a split plane instead of a split volume. Overwrite this function to
	 * create convex volume face of specialized subclasses. The default
	 * implementation creates a convex volume face of type decConvexVolumeFace.
	 */
	virtual decConvexVolumeFace *CreateVolumeFace( decConvexVolumeFace *face );
	/*@}*/
};

// end of include only once
#endif
