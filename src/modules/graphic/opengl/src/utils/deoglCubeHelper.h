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

#ifndef _DEOGLCUBEHELPER_H_
#define _DEOGLCUBEHELPER_H_

#include <dragengine/common/math/decMath.h>



/**
 * Cube calculation helpers.
 */
class deoglCubeHelper{
public:
	/** \name Helpers */
	/*@{*/
	/**
	 * Calculate cube face visibility.
	 * 
	 * Calculates for each face of a cube map if the object is potentially visible.
	 * Used to optimize rendering cube maps with geometry shaders by not rendering
	 * object faces on cube map faces where the object is not visible.
	 * 
	 * \param[in] boxMin Minimum extend of box to test relative to cube origin.
	 * \param[in] boxMax Maximum extend of box to test relative to cube origin.
	 * \param[out] result Set to visibility for each face. Has to be array of 6 size.
	 *                    Faces are in the order X+, X-, Y-, Y+, Z+, Z- as suitable
	 *                    for special render parameter blocks.
	 */
	static void CalcFaceVisibility( const decDVector &boxMin,
		const decDVector &boxMax, bool *result );
	
	static int CalcFaceVisibility( const decDVector &boxMin, const decDVector &boxMax );
	/*@}*/
};

#endif
