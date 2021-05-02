/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLRPTSKYLIGHTGIPREPARE_H_
#define _DEOGLRPTSKYLIGHTGIPREPARE_H_

#include <dragengine/parallel/deParallelTask.h>
#include <dragengine/threading/deSemaphore.h>

class deoglRenderPlanSkyLight;


/**
 * Parallel task finding content.
 */
class deoglRPTSkyLightGIPrepare : public deParallelTask{
private:
	deoglRenderPlanSkyLight &pPlan;
	float pElapsedFindContent;
	float pElapsedUpdateRenderTask;
	int pCountAdded;
	int pCountRemoved;
	int pElapsedAdded;
	int pElapsedRemoved;
	deSemaphore pSemaphore;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create task. */
	deoglRPTSkyLightGIPrepare( deoglRenderPlanSkyLight &plan );
	
	/** Clean up task. */
	virtual ~deoglRPTSkyLightGIPrepare();
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
	inline float GetElapsedFindContent() const{ return pElapsedFindContent; }
	inline float GetElapsedUpdateRenderTask() const{ return pElapsedUpdateRenderTask; }
	inline int GetCountAdded() const{ return pCountAdded; }
	inline int GetCountRemoved() const{ return pCountRemoved; }
	inline int GetElapsedAdded() const{ return pElapsedAdded; }
	inline int GetElapsedRemoved() const{ return pElapsedRemoved; }
	
	/** Finished semaphore. */
	inline deSemaphore &GetSemaphore(){ return pSemaphore; }
	/*@}*/
	
	
	
private:
	void pFindContent();
	void pUpdateRenderTask();
};

#endif
