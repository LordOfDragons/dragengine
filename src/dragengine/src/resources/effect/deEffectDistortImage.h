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

#ifndef _DEEFFEDISTORTIMAGE_H_
#define _DEEFFEDISTORTIMAGE_H_

#include "deEffect.h"
#include "../image/deImageReference.h"
#include "../../common/math/decMath.h"

class deEffectChain;


/**
 * \brief Effect Distort Image Class.
 *
 * The distort image effect applies a distortion to the rendered image using a
 * normal map type image. The red and green channel are used as an offset to
 * look up the pixel from the rendered image. The offset is first multiplied
 * by the strength factor.
 */
class deEffectDistortImage : public deEffect{
private:
	deImageReference pImage;
	decVector2 pTexCoords[ 4 ];
	float pStrengthU;
	float pStrengthV;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new effect distort image. */
	deEffectDistortImage( deEffectManager *manager );
	
protected:
	/**
	 * \brief Clean up effect distort image.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deEffectDistortImage();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Image or null if not set. */
	inline deImage *GetImage() const{ return pImage; }
	
	/** \brief Set image or null to remove it. */
	void SetImage( deImage *image );
	
	/** \brief Texture coordinates of the given corner. */
	const decVector2 &GetTextureCoordinatesFor( int corner ) const;
	
	/** \brief Set texture coordinates if the given corner. */
	void SetTextureCoordinatesFor( int corner, const decVector2 &textureCoordinates );
	
	/** \brief Strength in u direction. */
	inline float GetStrengthU() const{ return pStrengthU; }
	
	/** \brief Set strength in u direction. */
	void SetStrengthU( float strength );
	
	/** \brief Strength in v direction. */
	inline float GetStrengthV() const{ return pStrengthV; }
	
	/** \brief Set strength in v direction. */
	void SetStrengthV( float strength );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit effect. */
	virtual void Visit( deEffectVisitor &visitor );
	/*@}*/
};

#endif
