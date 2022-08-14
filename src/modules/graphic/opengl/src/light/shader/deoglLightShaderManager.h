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

#ifndef _DEOGLLIGHTSHADERMANAGER_H_
#define _DEOGLLIGHTSHADERMANAGER_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class deoglRenderThread;
class deoglLightShader;
class deoglLightShaderConfig;
class deoglShaderUnitSourceCode;



/**
 * @brief Light Shader Manager.
 */
class deoglLightShaderManager{
public:
	/** Unit source code path. */
	enum eUnitSourceCodePath{
		euscpVertexLight,
		euscpVertexParticle,
		euscpGeometryParticle,
		euscpGeometryStereo,
		euscpFragmentLight,
		EUSCP_COUNT
	};
	
private:
	deoglRenderThread &pRenderThread;
	decObjectOrderedSet pShaderList;
	int pMaintananceInterval;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new shader manager object. */
	deoglLightShaderManager( deoglRenderThread &renderThread );
	/** Cleans up the shader manager object. */
	~deoglLightShaderManager();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Retrieves a unit source code path. */
	const char *GetUnitSourceCodePath( int unitSourceCodePath ) const;
	
	/** Determines if a shader with the given configuration exists. */
	bool HasShaderWith( deoglLightShaderConfig &configuration ) const;
	/** Retrieves the shader with the given configuration creating it if not existing. */
	deoglLightShader *GetShaderWith( deoglLightShaderConfig &configuration );
	
	/** Retrieves the number of shaders. */
	int GetShaderCount() const;
	/** Retrieves shader by index. */
	deoglLightShader *GetShaderAt( int index ) const;
	/** Adds a shader. */
	void AddShader( deoglLightShader *shader );
	/** Removes a shader. */
	void RemoveShader( deoglLightShader *shader );
	/** Removes all shaders. */
	void RemoveAllShaders();
	
	/** Per-frame maintanance call from the graphic module. */
	void Maintanance();
	/*@}*/
};

#endif
