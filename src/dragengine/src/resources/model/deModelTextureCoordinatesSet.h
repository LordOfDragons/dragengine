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

#ifndef _DEMODELTEXTURECOORDINATESSET_H_
#define _DEMODELTEXTURECOORDINATESSET_H_

#include "../../common/math/decMath.h"


/**
 * \brief Model Texture Coordinates Set.
 * Defines an additional named set of texture coordinates for all faces
 * in the model.
 */
class DE_DLL_EXPORT deModelTextureCoordinatesSet{
private:
	decVector2 *pTextureCoordinates;
	int pTextureCoordinatesCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new model texture coordinate set. */
	deModelTextureCoordinatesSet();
	
	/** \brief Clean up model texture coordinate set. */
	~deModelTextureCoordinatesSet();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of texture coordinates. */
	inline int GetTextureCoordinatesCount() const{ return pTextureCoordinatesCount; }
	
	/** \brief Set number of texture coordinates. */
	void SetTextureCoordinatesCount( int count );
	
	/** \brief Texture coordinates by index. */
	const decVector2 &GetTextureCoordinatesAt( int index ) const;
	
	/** \brief Set texture coordinates  by index. */
	void SetTextureCoordinatesAt( int index, const decVector2 &textureCoordinates );
	
	/** \brief Pointer to the texture coordinates. */
	inline decVector2 *GetTextureCoordinates() const{ return pTextureCoordinates; }
	/*@}*/
};

#endif
