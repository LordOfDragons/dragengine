/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLSHAPECAPSULE_H_
#define _DEOGLSHAPECAPSULE_H_

#include "deoglShape.h"

#include <dragengine/common/math/decMath.h>



/**
 * @brief Capsule Shape.
 */
class deoglShapeCapsule : public deoglShape{
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new shape. */
	deoglShapeCapsule( deoglRenderThread &renderThread );
	/** Cleans up the shape. */
	virtual ~deoglShapeCapsule();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Calculate matrices for capsule parameters. */
	void CalcMatrices( decMatrix &matrix1, decMatrix &matrix2, const decVector &position,
	const decQuaternion &orientation, float halfHeight, float topRadius, float bottomRadius );
	
	/** Add lines data. */
	virtual void AddVBOLines( sVBOData *data );
	/** Add faces data. */
	virtual void AddVBOFaces( sVBOData *data );
	/*@}*/
};

#endif
