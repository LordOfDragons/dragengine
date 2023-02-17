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
 * Model texture.
 */
class deoglModelTexture{
private:
	int pFirstFace;
	int pFaceCount;
	bool pDoubleSided;
	bool pDecal;
	int pDecalOffset;
	deoglSharedSPBRTIGroupList::Ref pRTIGroups;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create texture. */
	deoglModelTexture( deoglRenderThread &renderThread );
	
	/** Clean up texture. */
	~deoglModelTexture();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** First face index. */
	inline int GetFirstFace() const{ return pFirstFace; }
	
	/** Set first face index. */
	void SetFirstFace( int faceIndex );
	
	/** Face count. */
	inline int GetFaceCount() const{ return pFaceCount; }
	
	/** Set face count. */
	void SetFaceCount( int faceCount );
	
	/** Texture is double sided. */
	inline bool GetDoubleSided() const{ return pDoubleSided; }
	
	/** Set texture is double sided. */
	void SetDoubleSided( bool doubleSided );
	
	/** Texture is rendered as decal. */
	inline bool GetDecal() const{ return pDecal; }
	
	/** Set if texture is rendered as decal. */
	void SetDecal( bool decal );
	
	/** Decal offset. */
	inline int GetDecalOffset() const{ return pDecalOffset; }
	
	/** Set decal offset. */
	void SetDecalOffset( int offset );
	
	/** Render task instance groups. */
	inline deoglSharedSPBRTIGroupList &GetRTIGroups(){ return pRTIGroups; }
	/*@}*/
};

#endif
