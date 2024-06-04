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

#ifndef _DEOGLRPTSKYLIGHTGIUPDATERT_H_
#define _DEOGLRPTSKYLIGHTGIUPDATERT_H_

#include <dragengine/parallel/deParallelTask.h>
#include <dragengine/threading/deSemaphore.h>

class deoglRenderPlanSkyLight;
class deoglRComponent;


/**
 * Parallel task update render task.
 */
class deoglRPTSkyLightGIUpdateRT : public deParallelTask{
private:
	deoglRenderPlanSkyLight &pPlan;
	float pElapsedTime;
	deSemaphore pSemaphore;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create task. */
	deoglRPTSkyLightGIUpdateRT( deoglRenderPlanSkyLight &plan );
	
	/** Clean up task. */
	virtual ~deoglRPTSkyLightGIUpdateRT();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Run task. */
	virtual void Run();
	
	/** Task finished. */
	virtual void Finished();
	
	/** Debug name. */
	virtual decString GetDebugName() const;
	
	/** Information for updating render task. */
	inline float GetElapsedTime() const{ return pElapsedTime; }
	
	/** Finished semaphore. */
	inline deSemaphore &GetSemaphore(){ return pSemaphore; }
	/*@}*/
	
	
	
private:
	void pUpdateStaticRT();
	void pUpdateDynamicRT();
	bool pIsComponentStatic( const deoglRComponent &component ) const;
};

#endif
