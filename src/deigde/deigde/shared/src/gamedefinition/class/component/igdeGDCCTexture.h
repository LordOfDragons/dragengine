/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEGDCCTEXTURE_H_
#define _IGDEGDCCTEXTURE_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringDictionary.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/deObject.h>



/**
 * \brief Game Definition Class Component Texture.
 */
class igdeGDCCTexture : public deObject{
private:
	decString pName;
	decString pPathSkin;
	decVector2 pOffset;
	decVector2 pScale;
	float pRotation;
	decColor pColorTint;
	decStringDictionary pProperties;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create texture definition. */
	igdeGDCCTexture();
	
	/** \brief Create copy of texture definition. */
	igdeGDCCTexture( const igdeGDCCTexture &texture );
	
	/** \brief Clean up texture definition. */
	virtual ~igdeGDCCTexture();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name of texture to replace. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name of texture to replace. */
	void SetName( const char *name );
	
	/** \brief Path of skin to use. */
	inline const decString &GetPathSkin() const{ return pPathSkin; }
	
	/** \brief Set path of skin to use. */
	void SetPathSkin( const char *path );
	
	/** \brief Texture coordinate offset. */
	inline const decVector2 &GetOffset() const{ return pOffset; }
	
	/** \brief Set texture coordinate offset. */
	void SetOffset( const decVector2 &offset );
	
	/** \brief Texture coordinate scale. */
	inline const decVector2 &GetScale() const{ return pScale; }
	
	/** \brief Set texture coordinate scale. */
	void SetScale( const decVector2 &scale );
	
	/** \brief Texture coordinate rotation. */
	inline float GetRotation() const{ return pRotation; }
	
	/** \brief Set texture coordinate rotation. */
	void SetRotation( float rotation );
	
	/** \brief Color tint. */
	inline const decColor &GetColorTint() const{ return pColorTint; }
	
	/** \brief Set color tint. */
	void SetColorTint( const decColor &color );
	
	/** \brief Properties. */
	inline const decStringDictionary &GetProperties() const{ return pProperties; }
	
	/** \brief Set properties. */
	void SetProperties( const decStringDictionary &properties );
	/*@}*/
};

#endif
