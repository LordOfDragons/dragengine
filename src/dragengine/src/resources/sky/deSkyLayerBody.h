/* 
 * Drag[en]gine Game Engine
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

#ifndef _DESKYLAYERBODY_H_
#define _DESKYLAYERBODY_H_

#include "../../common/math/decMath.h"
#include "../skin/deSkinReference.h"


/**
 * \brief Sky Bodies Layer Body.
 *
 * Defines a body in a sky bodies layer. For positioning the body the
 * sky is considered to be a sphere of unit radius. The position of
 * the body is then defined using a quaternion rotation. At identity
 * rotation the body is located at the equator at the 0° meridian
 * which is the same as the body located 1 unit along the Z-Axis.
 * The size of the body is defined using two angles measured in
 * radians. The angles indicate the entire width and height of the
 * body not half the width and height. This angle is comparable to
 * the astronomic angle under which you see a body in the sky. The
 * body is drawn using the given image which can use transparency.
 * Furthermore a color including transparency can be defined which
 * is multiplied component wise with the image pixels.
 */
class DE_DLL_EXPORT deSkyLayerBody{
private:
	decQuaternion pOrientation;
	decVector2 pSize;
	decColor pColor;
	deSkinReference pSkin;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new sky bodies layer body. */
	deSkyLayerBody();
	
	/** \brief Clean up sky bodies layer body. */
	~deSkyLayerBody();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation( const decQuaternion &orientation );
	
	/** \brief Size in radians. */
	inline const decVector2 &GetSize() const{ return pSize; }
	
	/** \brief Set size in radians. */
	void SetSize( const decVector2 &size );
	
	/** \brief Blending color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** \brief Set blending color. */
	void SetColor( const decColor &color );
	
	/** \brief Skin or NULL if not set. */
	inline deSkin *GetSkin() const{ return pSkin; }
	
	/** \brief Set skin or NULL if not set. */
	void SetSkin( deSkin *skin );
	/*@}*/
};

#endif
