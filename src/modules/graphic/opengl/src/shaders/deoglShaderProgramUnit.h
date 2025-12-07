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

#ifndef _DEOGLSHADERPROGRAMUNIT_H_
#define _DEOGLSHADERPROGRAMUNIT_H_

#include "deoglShaderDefines.h"
#include "../deoglBasics.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/threading/deMutex.h>

class deoglRenderThread;
class deoglShaderUnitSourceCode;
class deoglShaderCompileUnitTask;


/**
 * Shader program unit.
 */
class deoglShaderProgramUnit : public deObject{
public:
	typedef deTObjectReference<deoglShaderProgramUnit> Ref;
	
	
private:
	deoglRenderThread &pRenderThread;
	const deoglShaderUnitSourceCode *pSources;
	decString pProcessedSources;
	decObjectList pProcessedSourceLocations;
	deoglShaderDefines pDefines;
	
	GLuint pHandle;
	deMutex pMutex;
	decTimer pTimerCompile;
	
	
public:
	bool isCompiling, compilingFailed;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shader program unit. */
	deoglShaderProgramUnit(deoglRenderThread &renderThread,
		const deoglShaderUnitSourceCode *sources, const deoglShaderDefines &defines);
	
protected:
	/** Clean up shader program. */
	virtual ~deoglShaderProgramUnit();
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Sources. */
	inline const deoglShaderUnitSourceCode *GetSources() const {return pSources;}
	
	/** Defines. */
	inline const deoglShaderDefines &GetDefines() const {return pDefines;}
	
	/** Handle or 0 if not created yet. */
	inline GLuint GetHandle() const{return pHandle;}
	
	/** Create handle if absent. */
	void CreateHandle();
	
	/** Drop handle if present. */
	void DropHandle();
	
	/** Is compiled. */
	inline bool IsCompiled() const{return pHandle != 0;}
	
	/** Processed sources. */
	inline const decString &GetProcessedSources() const{return pProcessedSources;}
	
	/** Set processed sources. */
	void SetProcessedSources(const decString &code);
	
	/** Processed source locations. */
	inline const decObjectList &GetProcessedSourceLocations() const{return pProcessedSourceLocations;}
	
	/** Set processed source locations. */
	void SetProcessedSourceLocations(const decObjectList &locations);
	
	/** Mutex. */
	inline deMutex &GetMutex(){return pMutex;}
	
	/** Compile timer. */
	inline decTimer &GetTimerCompile(){return pTimerCompile;}
	/*@}*/
};

#endif
