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

#ifndef _DEOGLSKINSHADERMANAGER_H_
#define _DEOGLSKINSHADERMANAGER_H_

#include "deoglSkinShader.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/collection/decPointerList.h>
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
	
	class cGetShaderListener{
	public:
		cGetShaderListener() = default;
		virtual ~cGetShaderListener() = default;
		
		/** Get shader finished. If prepare shader failed shader is nullptr. */
		virtual void GetShaderFinished(deoglSkinShader *shader) = 0;
		/*@}*/
	};
	
	
private:
	class cPrepareShader : public deoglSkinShader::cShaderPreparedListener{
	private:
		deoglSkinShaderManager &pManager;
		deoglSkinShader::Ref pShader;
		decPointerList pListeners;
		
	public:
		cPrepareShader(deoglSkinShaderManager &manager, const deoglSkinShader::Ref &shader);
		
		inline const deoglSkinShader::Ref &GetShader() const{ return pShader; }
		
		void AddListener(cGetShaderListener *listener);
		
		void PrepareShaderFinished(deoglSkinShader &shader) override;
		/*@}*/
	};
	
	friend class cCompileProgram;
	
	
	deoglRenderThread &pRenderThread;
	decObjectOrderedSet pShaderList;
	int pMaintananceInterval;
	deMutex pMutex;
	decPointerList pPrepareShaders;
	
	
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
	
	/** Unit source code path. */
	const char *GetUnitSourceCodePath( eUnitSourceCodePath unitSourceCodePath ) const;
	
	/** Shader with configuration creating it if absent. */
	deoglSkinShader *GetShaderWith( deoglSkinShaderConfig &configuration );
	
	/** Asynchonous shader with configuration creating it if absent. */
	void GetShaderWithAsync(deoglSkinShaderConfig &configuration, cGetShaderListener *listener);
	
	/** Retrieves the number of shaders. */
	int GetShaderCount();
	
	/** Retrieves shader by index. */
	const deoglSkinShader &GetShaderAt( int index );
	/*@}*/
	
	
private:
	deoglSkinShader *pGetShaderWith(const deoglSkinShaderConfig &configuration) const;
	cPrepareShader *pGetPrepareWith(const deoglSkinShaderConfig &configuration) const;
};

#endif
