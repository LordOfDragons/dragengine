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

#ifndef _DEOGLRPARTICLEEMITTER_H_
#define _DEOGLRPARTICLEEMITTER_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class deoglRParticleEmitterType;
class deoglVBOLayout;


/**
 * Render particle emitter.
 */
class deoglRParticleEmitter : public deObject{
private:
	deoglRenderThread &pRenderThread;
	
	decObjectList pTypes;
	
	deoglVBOLayout *pVBOLayoutShared;
	deoglVBOLayout *pVBOLayoutLocal;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render emitter. */
	deoglRParticleEmitter( deoglRenderThread &renderThread );
	
	/** Clean up render emitter. */
	virtual ~deoglRParticleEmitter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render threaf. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Shared VBO layout. */
	deoglVBOLayout *GetVBOLayoutShared();
	
	/** Local VBO layout. */
	deoglVBOLayout *GetVBOLayoutLocal();
	
	/** Prepare for rendering. */
	void PrepareForRender();
	
	
	
	/** Number of types. */
	int GetTypeCount() const;
	
	/** Type at index. */
	deoglRParticleEmitterType &GetTypeAt( int index ) const;
	
	/** Remove all types. */
	void RemoveAllTypes();
	
	/** Add type. */
	void AddType( deoglRParticleEmitterType *type );
	/*@}*/
};

#endif
