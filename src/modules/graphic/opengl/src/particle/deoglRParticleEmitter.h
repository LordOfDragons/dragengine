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
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoglRParticleEmitter> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render emitter. */
	deoglRParticleEmitter(deoglRenderThread &renderThread);
	
	/** Clean up render emitter. */
	virtual ~deoglRParticleEmitter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render threaf. */
	inline deoglRenderThread &GetRenderThread() const{return pRenderThread;}
	
	/** Shared VBO layout. */
	deoglVBOLayout *GetVBOLayoutShared();
	
	/** Local VBO layout. */
	deoglVBOLayout *GetVBOLayoutLocal();
	
	/** Prepare for rendering. */
	void PrepareForRender();
	
	
	
	/** Number of types. */
	int GetTypeCount() const;
	
	/** Type at index. */
	deoglRParticleEmitterType &GetTypeAt(int index) const;
	
	/** Remove all types. */
	void RemoveAllTypes();
	
	/** Add type. */
	void AddType(deoglRParticleEmitterType *type);
	/*@}*/
};

#endif
