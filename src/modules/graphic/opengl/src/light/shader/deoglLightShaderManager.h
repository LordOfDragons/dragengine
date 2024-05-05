/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _DEOGLLIGHTSHADERMANAGER_H_
#define _DEOGLLIGHTSHADERMANAGER_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class deoglRenderThread;
class deoglLightShader;
class deoglLightShaderConfig;
class deoglShaderUnitSourceCode;



/**
 * Light Shader Manager.
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
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new shader manager object. */
	deoglLightShaderManager( deoglRenderThread &renderThread );
	/** Cleans up the shader manager object. */
	~deoglLightShaderManager();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
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
