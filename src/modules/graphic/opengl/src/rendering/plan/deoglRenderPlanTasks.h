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

#ifndef _DEOGLRENDERERPLANTASKS_H_
#define _DEOGLRENDERERPLANTASKS_H_

#include "../task/deoglRenderTask.h"
#include "../task/deoglComputeRenderTask.h"

class deoglRenderPlan;
class deoglRenderPlanMasked;
class deoglRPTBuildRTsDepth;
class deoglRPTBuildRTsGeometry;


/**
 * Render Plan tasks.
 */
class deoglRenderPlanTasks{
private:
	deoglRenderPlan &pPlan;
	
	deoglRenderTask *pSolidDepthTask;
	deoglRenderTask *pSolidDepthOutlineTask;
	deoglRenderTask *pSolidGeometryTask;
	deoglRenderTask *pSolidGeometryHeight1Task;
	deoglRenderTask *pSolidGeometryHeight2Task;
	deoglRenderTask *pSolidGeometryOutlineTask;
	deoglRenderTask *pSolidDecalsTask;
	
	deoglRenderTask *pSolidDepthXRayTask;
	deoglRenderTask *pSolidDepthOutlineXRayTask;
	deoglRenderTask *pSolidGeometryXRayTask;
	deoglRenderTask *pSolidGeometryHeight1XRayTask;
	deoglRenderTask *pSolidGeometryHeight2XRayTask;
	deoglRenderTask *pSolidGeometryOutlineXRayTask;
	deoglRenderTask *pSolidDecalsXRayTask;
	
	deoglRPTBuildRTsDepth *pTaskDepth;
	deoglRPTBuildRTsGeometry *pTaskGeometry;
	
	deoglComputeRenderTask::Ref pCRTSolidDepth;
	deoglComputeRenderTask::Ref pCRTSolidGeometry;
	
	deoglComputeRenderTask::Ref pCRTSolidDepthXRay;
	deoglComputeRenderTask::Ref pCRTSolidGeometryXRay;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render plan tasks. */
	deoglRenderPlanTasks( deoglRenderPlan &plan );
	
	/** Clean up render plan tasks. */
	~deoglRenderPlanTasks();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Parent plan. */
	inline deoglRenderPlan &GetPlan() const{ return pPlan; }
	
	
	
	inline deoglRenderTask &GetSolidDepthTask(){ return *pSolidDepthTask; }
	
	inline deoglRenderTask &GetSolidDepthOutlineTask(){ return *pSolidDepthOutlineTask; }
	
	inline deoglRenderTask &GetSolidGeometryTask(){ return *pSolidGeometryTask; }
	
	inline deoglRenderTask &GetSolidGeometryHeight1Task(){ return *pSolidGeometryHeight1Task; }
	inline deoglRenderTask &GetSolidGeometryHeight2Task(){ return *pSolidGeometryHeight2Task; }
	
	inline deoglRenderTask &GetSolidGeometryOutlineTask(){ return *pSolidGeometryOutlineTask; }
	
	inline deoglRenderTask &GetSolidDecalsTask(){ return *pSolidDecalsTask; }
	
	
	
	inline deoglRenderTask &GetSolidDepthXRayTask(){ return *pSolidDepthXRayTask; }
	
	inline deoglRenderTask &GetSolidDepthOutlineXRayTask(){ return *pSolidDepthOutlineXRayTask; }
	
	inline deoglRenderTask &GetSolidGeometryXRayTask(){ return *pSolidGeometryXRayTask; }
	
	inline deoglRenderTask &GetSolidGeometryHeight1XRayTask(){ return *pSolidGeometryHeight1XRayTask; }
	inline deoglRenderTask &GetSolidGeometryHeight2XRayTask(){ return *pSolidGeometryHeight2XRayTask; }
	
	inline deoglRenderTask &GetSolidGeometryOutlineXRayTask(){ return *pSolidGeometryOutlineXRayTask; }
	
	inline deoglRenderTask &GetSolidDecalsXRayTask(){ return *pSolidDecalsXRayTask; }
	
	
	
	inline const deoglComputeRenderTask::Ref &GetCRTSolidDepth() const{ return pCRTSolidDepth; }
	inline const deoglComputeRenderTask::Ref &GetCRTSolidGeometry() const{ return pCRTSolidGeometry; }
	
	inline const deoglComputeRenderTask::Ref &GetCRTSolidDepthXRay() const{ return pCRTSolidDepthXRay; }
	inline const deoglComputeRenderTask::Ref &GetCRTSolidGeometryXRay() const{ return pCRTSolidGeometryXRay; }
	
	
	
	/** Build compute render tasks. */
	void BuildComputeRenderTasks( const deoglRenderPlanMasked *mask );
	
	/** Sort compute render tasks and begin reading back steps. */
	void SortComputeRenderTasks();
	
	/** Finish read back compute render tasks. */
	void FinishReadBackComputeRenderTasks();
	
	/** Start building render tasks. */
	void StartBuildTasks( const deoglRenderPlanMasked *mask );
	
	/** Wait for task building to finish. */
	void WaitFinishBuildingTasksDepth();
	void WaitFinishBuildingTasksGeometry();
	
	/** Clean up after rendering. */
	void CleanUp();
	/*@}*/
	
	
	
private:
	void pBuildCRTSolidDepth( deoglComputeRenderTask &renderTask,
		const deoglRenderPlanMasked *mask , bool xray );
	
	void pBuildCRTSolidGeometry( deoglComputeRenderTask &renderTask,
		const deoglRenderPlanMasked *mask , bool xray );
};

#endif
