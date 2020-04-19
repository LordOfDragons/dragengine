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

#ifndef _DEEFFECVERLAYIMAGE_H_
#define _DEEFFECVERLAYIMAGE_H_

#include "deEffect.h"
#include "../image/deImageReference.h"
#include "../../common/math/decMath.h"


/**
 * \brief Effect overlay image.
 * 
 * The overlay image effect simply renders a predefined image over the existing
 * render output. The image can occupy any part of the existing image using the
 * texture coordinates. The image has to contain transparency to work best although
 * you can specify an additional transparency yourself. Furthermore additional
 * effects can be applied to this image ( effect chaining ) before applying it.
 */
class deEffectOverlayImage : public deEffect{
private:
	deImageReference pImage;
	decVector2 pTexCoords[ 4 ];
	float pTransparency;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create effect overlay image. */
	deEffectOverlayImage( deEffectManager *manager );
	
protected:
	/**
	 * \brief Clean up effect overlay image.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deEffectOverlayImage();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Image or NULL if not set. */
	inline deImage *GetImage() const{ return pImage; }
	
	/** \brief Set image or NULL if not set. */
	void SetImage( deImage *image );
	
	/**
	 * \brief Texture coordinates for corner.
	 * \throw deeOutOfBoundary \em corner is less than 0 or greater than 3.
	 */
	const decVector2 &GetTextureCoordinatesFor( int corner ) const;
	
	/**
	 * \brief Set texture coordinates for corner.
	 * \throw deeOutOfBoundary \em corner is less than 0 or greater than 3.
	 */
	void SetTextureCoordinatesFor( int corner, const decVector2 &textureCoordinates );
	
	/** \brief Transparency. */
	inline float GetTransparency() const{ return pTransparency; }
	
	/** \brief Set transparency. */
	void SetTransparency( float transparency );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit effect. */
	virtual void Visit( deEffectVisitor &visitor );
	/*@}*/
};

#endif
