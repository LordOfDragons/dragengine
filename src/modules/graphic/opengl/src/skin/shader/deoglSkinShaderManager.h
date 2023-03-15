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

#ifndef _DEOGLSKINSHADERMANAGER_H_
#define _DEOGLSKINSHADERMANAGER_H_

#include "deoglSkinShader.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/threading/deMutex.h>

class deoglRenderThread;
class deoglSkinShaderConfig;
class deoglShaderUnitSourceCode;



/**
 * Skin Shader Manager.
 */
class deoglSkinShaderManager{
public:
	/** Unit source code path. */
	enum eUnitSourceCodePath{
		euscpVertexGeometry,
		euscpVertexDepth,
		euscpVertexParticle,
		euscpVertexPassThrough,
		euscpVertexGIMaterialMap,
		
		euscpGeometryGeometry,
		euscpGeometryDepth,
		euscpGeometryParticle,
		euscpGeometryParticleRibbon,
		
		euscpTessControlGeometry,
		euscpTessControlDepth,
		
		euscpTessEvalGeometry,
		euscpTessEvalDepth,
		
		euscpFragmentGeometry,
		euscpFragmentDepth,
		euscpFragmentGIMaterialMap
	};
	
	const static int UnitSourceCodePathCount = euscpFragmentGIMaterialMap + 1;
	
	
	
private:
	deoglRenderThread &pRenderThread;
	decObjectOrderedSet pShaderList;
	int pMaintananceInterval;
	deMutex pMutex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new shader manager object. */
	deoglSkinShaderManager( deoglRenderThread &renderThread );
	/** Cleans up the shader manager object. */
	~deoglSkinShaderManager();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Retrieves a unit source code path. */
	const char *GetUnitSourceCodePath( eUnitSourceCodePath unitSourceCodePath ) const;
	
	/** Retrieves the shader with the given configuration creating it if not existing. */
	deoglSkinShader::Ref GetShaderWith( deoglSkinShaderConfig &configuration );
	
	/** Retrieves the number of shaders. */
	int GetShaderCount();
	
	/** Retrieves shader by index. */
	const deoglSkinShader &GetShaderAt( int index );
	/*@}*/
};

#endif
