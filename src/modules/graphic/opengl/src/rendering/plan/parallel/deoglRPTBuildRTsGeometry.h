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

#ifndef _DEOGLRPTBUILDRTSGEOMETRY_H_
#define _DEOGLRPTBUILDRTSGEOMETRY_H_

#include <dragengine/parallel/deParallelTask.h>
#include <dragengine/threading/deSemaphore.h>

class deoglRenderPlanTasks;
class deoglRenderPlanMasked;


/**
 * Parallel task build render tasks.
 */
class deoglRPTBuildRTsGeometry : public deParallelTask{
private:
	deoglRenderPlanTasks &pPlan;
	const deoglRenderPlanMasked *pMask;
	float pElapsedTime;
	deSemaphore pSemaphore;
	int pPipelineModifier;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create task. */
	deoglRPTBuildRTsGeometry( deoglRenderPlanTasks &plan, const deoglRenderPlanMasked *mask );
	
	/** Clean up task. */
	virtual ~deoglRPTBuildRTsGeometry();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Run task. */
	virtual void Run();
	
	/** Task finished. */
	virtual void Finished();
	
	/** Debug name. */
	virtual decString GetDebugName() const;
	
	/** Elapsed time. */
	inline float GetElapsedTime() const{ return pElapsedTime; }
	
	/** Finished semaphore. */
	inline deSemaphore &GetSemaphore(){ return pSemaphore; }
	/*@}*/
	
	
	
private:
	void pInitPipelineModifier();
	void pSolid( bool xray );
	void pSolidTerrain( bool xray );
	void pSolidOutline( bool xray );
	void pSolidDecals( bool xray );
};

#endif
