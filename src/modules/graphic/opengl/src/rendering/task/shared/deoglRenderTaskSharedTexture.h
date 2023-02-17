/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLRENDERTASKSHAREDTEXTURE_H_
#define _DEOGLRENDERTASKSHAREDTEXTURE_H_

#include <dragengine/deObject.h>

class deoglRenderTaskSharedPool;
class deoglTexUnitsConfig;


/**
 * Shared render task shader.
 */
class deoglRenderTaskSharedTexture : public deObject{
public:
	typedef deTObjectReference<deoglRenderTaskSharedTexture> Ref;
	
	
	
private:
	deoglRenderTaskSharedPool &pPool;
	const int pIndex;
	
	deoglTexUnitsConfig *pTUC;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shared render task shader. */
	deoglRenderTaskSharedTexture( deoglRenderTaskSharedPool &pool, int index );
	
protected:
	/** Clean up shared render task shader. */
	virtual ~deoglRenderTaskSharedTexture();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Pool. */
	inline deoglRenderTaskSharedPool &GetPool() const{ return pPool; }
	
	/** Index. */
	inline int GetIndex() const{ return pIndex; }
	
	
	
	/** Texture units configuration. */
	inline deoglTexUnitsConfig *GetTUC() const{ return pTUC; }
	
	/** Set texture units configuration. */
	void SetTUC( deoglTexUnitsConfig *tuc );
	
	
	
	/** Clear. */
	void Clear();
	
	/** Return to pool. */
	void ReturnToPool();
	/*@}*/
};

#endif
