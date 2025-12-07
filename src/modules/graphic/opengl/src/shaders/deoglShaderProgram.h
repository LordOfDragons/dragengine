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

#ifndef _DEOGLSHADERPROGRAM_H_
#define _DEOGLSHADERPROGRAM_H_

#include "deoglShaderDefines.h"

#include <dragengine/deObject.h>
#include <dragengine/common/utils/decTimer.h>

class deoglRenderThread;
class deoglShaderProgramUnit;
class deoglShaderSources;
class deoglShaderCompiled;



/**
 * A shader program. Shader programs are compiled from a base code loaded
 * from disk using defines to produce a specialized program.
 */
class deoglShaderProgram : public deObject{
public:
	typedef deTObjectReference<deoglShaderProgram> Ref;
	
	
private:
	deoglRenderThread &pRenderThread;
	const deoglShaderSources *pSources;
	deoglShaderDefines pDefines;
	decString pCacheId;
	
	deoglShaderProgramUnit *pUnitCompute;
	deoglShaderProgramUnit *pUnitTessellationControl;
	deoglShaderProgramUnit *pUnitTessellationEvaluation;
	deoglShaderProgramUnit *pUnitGeometry;
	deoglShaderProgramUnit *pUnitVertex;
	deoglShaderProgramUnit *pUnitFragment;
	
	deoglShaderCompiled *pCompiled;
	
	unsigned int pUniqueKey;
	decTimer pTimerCompile;
	
	
public:
	bool ready, isCompiling;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shader program. */
	deoglShaderProgram(deoglRenderThread &renderThread, const deoglShaderSources *sources);
	
	/** Create shader program. */
	deoglShaderProgram(deoglRenderThread &renderThread,
		const deoglShaderSources *sources, const deoglShaderDefines &defines);
	
protected:
	/** Clean up shader program. */
	virtual ~deoglShaderProgram();
	/*@}*/
	
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Sources. */
	inline const deoglShaderSources *GetSources() const{ return pSources; }
	
	/** Defines. */
	inline const deoglShaderDefines &GetDefines() const{ return pDefines; }
	
	/** Cache identifier. */
	inline const decString &GetCacheId() const{ return pCacheId; }
	
	/** Set cache identifier. */
	void SetCacheId(const decString &id);
	
	/** Compute unit or nullptr. */
	inline deoglShaderProgramUnit *GetUnitCompute() const{ return pUnitCompute; }
	
	/** Set compute unit or nullptr. */
	void SetUnitCompute(deoglShaderProgramUnit *unit);
	
	/** Tessellation control unit or nullptr. */
	inline deoglShaderProgramUnit *GetUnitTessellationControl() const{ return pUnitTessellationControl; }
	
	/** Set tessellation control unit or nullptr. */
	void SetUnitTessellationControl(deoglShaderProgramUnit *unit);
	
	/** Tessellation evaluation unit or nullptr. */
	inline deoglShaderProgramUnit *GetUnitTessellationEvaluation() const{ return pUnitTessellationEvaluation; }
	
	/** Set tessellation evaluation unit or nullptr. */
	void SetUnitTessellationEvaluation(deoglShaderProgramUnit *unit);
	
	/** Geometry unit or nullptr. */
	inline deoglShaderProgramUnit *GetUnitGeometry() const{ return pUnitGeometry; }
	
	/** Set geometry unit or nullptr. */
	void SetUnitGeometry(deoglShaderProgramUnit *unit);
	
	/** Vertex unit or nullptr. */
	inline deoglShaderProgramUnit *GetUnitVertex() const{ return pUnitVertex; }
	
	/** Set vertex unit or nullptr. */
	void SetUnitVertex(deoglShaderProgramUnit *unit);
	
	/** Fragment unit or nullptr. */
	inline deoglShaderProgramUnit *GetUnitFragment() const{ return pUnitFragment; }
	
	/** Set fragment unit or nullptr. */
	void SetUnitFragment(deoglShaderProgramUnit *unit);
	
	/** Shader can compile (all units finished compiling). */
	bool CanCompile() const;
	
	/** Compiled shader or nullptr. */
	inline deoglShaderCompiled *GetCompiled() const{ return pCompiled; }
	
	/** Set compiled shader or nullptr. */
	void SetCompiled(deoglShaderCompiled *compiled);
	
	/** Move compiled shader or nullptr. */
	void MoveCompiled(deoglShaderProgram &program);
	
	/** Unique key for use with dictionaries. */
	inline unsigned int GetUniqueKey() const{ return pUniqueKey; }
	
	/** Compile timer. */
	inline decTimer &GetTimerCompile(){ return pTimerCompile; }
	/*@}*/
};

#endif
