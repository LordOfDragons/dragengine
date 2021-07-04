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

#ifndef _DEOGLMODELTEXTURE_H_
#define _DEOGLMODELTEXTURE_H_

#include <dragengine/common/string/decString.h>

#include "../../shaders/paramblock/shared/deoglSharedSPBRTIGroupList.h"

class deoglRenderThread;


/**
 * \brief Model texture.
 */
class deoglModelTexture{
private:
	int pFirstFace;
	int pFaceCount;
	bool pDoubleSided;
	bool pDecal;
	int pDecalOffset;
	deoglSharedSPBRTIGroupList pRTIGroups;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create texture. */
	deoglModelTexture( deoglRenderThread &renderThread );
	
	/** \brief Clean up texture. */
	~deoglModelTexture();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief First face index. */
	inline int GetFirstFace() const{ return pFirstFace; }
	
	/** \brief Set first face index. */
	void SetFirstFace( int faceIndex );
	
	/** \brief Face count. */
	inline int GetFaceCount() const{ return pFaceCount; }
	
	/** \brief Set face count. */
	void SetFaceCount( int faceCount );
	
	/** \brief Texture is double sided. */
	inline bool GetDoubleSided() const{ return pDoubleSided; }
	
	/** \brief Set texture is double sided. */
	void SetDoubleSided( bool doubleSided );
	
	/** \brief Texture is rendered as decal. */
	inline bool GetDecal() const{ return pDecal; }
	
	/** \brief Set if texture is rendered as decal. */
	void SetDecal( bool decal );
	
	/** \brief Decal offset. */
	inline int GetDecalOffset() const{ return pDecalOffset; }
	
	/** \brief Set decal offset. */
	void SetDecalOffset( int offset );
	
	/** \brief Render task instance groups. */
	inline deoglSharedSPBRTIGroupList &GetRTIGroups(){ return pRTIGroups; }
	/*@}*/
};

#endif
