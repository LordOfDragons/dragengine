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

#ifndef _DEOGLPIPELINEMANAGER_H_
#define _DEOGLPIPELINEMANAGER_H_

#include "deoglPipelineState.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/threading/deMutex.h>

class deoglRenderThread;
class deoglPipeline;
class deoglPipelineConfiguration;


/**
 * Pipeline manager.
 */
class deoglPipelineManager : public deObject{
public:
	typedef deTObjectReference<deoglPipelineManager> Ref;
	
	
private:
	deoglRenderThread &pRenderThread;
	deoglPipelineState pState;
	decObjectList pPipelines;
	
	decPointerList pRTSPipelines;
	deMutex pMutex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create pipeline manager. */
	deoglPipelineManager( deoglRenderThread &renderThread );
	
protected:
	/** Clean up pipelines manager. */
	virtual ~deoglPipelineManager();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** State. */
	inline deoglPipelineState &GetState(){ return pState; }
	
	/** Pipeline with RTS index or nullptr. Caller does not hold reference. */
	const deoglPipeline *GetWithRTSIndex( int index );
	
	/** Pipeline with configuration creating it if absent. Caller does not hold reference. */
	const deoglPipeline *GetWith( const deoglPipelineConfiguration &configuration,
		bool assignRTSIndex = false );
	/*@}*/
};

#endif
