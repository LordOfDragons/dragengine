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

#ifndef _DEOGLRENDERTASKSHAREDVAO_H_
#define _DEOGLRENDERTASKSHAREDVAO_H_

#include <dragengine/deObject.h>

class deoglVAO;
class deoglRenderTaskSharedPool;


/**
 * Shared render task shader.
 */
class deoglRenderTaskSharedVAO : public deObject{
public:
	typedef deTObjectReference<deoglRenderTaskSharedVAO> Ref;
	
	
	
private:
	deoglRenderTaskSharedPool &pPool;
	const int pIndex;
	
	deoglVAO *pVAO;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shared render task shader. */
	deoglRenderTaskSharedVAO( deoglRenderTaskSharedPool &pool, int index );
	
protected:
	/** Clean up shared render task shader. */
	virtual ~deoglRenderTaskSharedVAO();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Pool. */
	inline deoglRenderTaskSharedPool &GetPool() const{ return pPool; }
	
	/** Index. */
	inline int GetIndex() const{ return pIndex; }
	
	
	
	/** VAO. */
	inline deoglVAO *GetVAO() const{ return pVAO; }
	
	/** Set vao. */
	void SetVAO( deoglVAO *shader );
	
	
	
	/** Clear. */
	void Clear();
	
	/** Return to pool. */
	void ReturnToPool();
	/*@}*/
};

#endif
