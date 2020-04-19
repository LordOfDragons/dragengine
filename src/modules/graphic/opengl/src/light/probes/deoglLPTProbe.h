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

#ifndef _DEOGLLPTPROBE_H_
#define _DEOGLLPTPROBE_H_

#include <dragengine/common/math/decMath.h>



/**
 * @brief Light Probe Texture Probe.
 * Each probe contains 6 rgb-color readings indicating the color hitting the probe from
 * this direction. Probes are stored as rgb-float-color in the texture as a consecutive
 * run (each of the 6 pixels stores the color of one sample). The probes store the
 * position (vector, relative to camera) and the texture coordinates (2-integer) of the
 * first (left most) pixel in the texture.
 */
class deoglLPTProbe{
private:
	decVector pPosition;
	decPoint pTexCoords;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new light probe texture probe. */
	deoglLPTProbe();
	/** Cleans up the light probe texture probe. */
	~deoglLPTProbe();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the probe position relative to the camera. */
	inline const decVector &GetPosition() const{ return pPosition; }
	/** Sets the probe position relative to the camera. */
	void SetPosition( const decVector &position );
	/** Retrieves the texture coordinates. */
	inline const decPoint &GetTextureCoordinates() const{ return pTexCoords; }
	/** Sets the texture coordinates. */
	void SetTextureCoordinates( const decPoint &tc );
	/** Sets the texture coordinates. */
	void SetTextureCoordinates( int x, int y );
	/*@}*/
};

#endif
