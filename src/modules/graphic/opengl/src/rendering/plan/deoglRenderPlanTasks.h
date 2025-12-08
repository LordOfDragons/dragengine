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

#ifndef _DEOGLRENDERERPLANTASKS_H_
#define _DEOGLRENDERERPLANTASKS_H_

#include "../task/deoglRenderTask.h"
#include "../task/deoglComputeRenderTask.h"

class deoglRenderPlan;
class deoglRenderPlanMasked;
#include "parallel/deoglRPTBuildRTsDepth.h"
#include "parallel/deoglRPTBuildRTsGeometry.h"


/**
 * Render Plan tasks.
 */
class deoglRenderPlanTasks : public deObject{
public:
	typedef deTObjectReference<deoglRenderPlanTasks> Ref;
	
	
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
	
	deoglRPTBuildRTsDepth::Ref pTaskDepth;
	deoglRPTBuildRTsGeometry::Ref pTaskGeometry;
	
	deoglComputeRenderTask::Ref pCRTSolidDepth;
	deoglComputeRenderTask::Ref pCRTSolidGeometry;
	
	deoglComputeRenderTask::Ref pCRTSolidDepthXRay;
	deoglComputeRenderTask::Ref pCRTSolidGeometryXRay;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render plan tasks. */
	deoglRenderPlanTasks(deoglRenderPlan &plan);
	
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
	
	
	
	/** Build compute render tasks and begin reading back steps. */
	void BuildComputeRenderTasks(const deoglRenderPlanMasked *mask, bool rebuild = false);
	
	/** Finish read back compute render tasks. */
	void FinishReadBackComputeRenderTasks(const deoglRenderPlanMasked *mask);
	
	/** Start building render tasks. */
	void StartBuildTasks(const deoglRenderPlanMasked *mask);
	
	/** Wait for task building to finish. */
	void WaitFinishBuildingTasksDepth();
	void WaitFinishBuildingTasksGeometry();
	
	/** Clean up after rendering. */
	void CleanUp();
	/*@}*/
	
	
	
private:
	void pBuildCRTSolidDepth(deoglComputeRenderTask &renderTask,
		const deoglRenderPlanMasked *mask , bool xray);
	
	void pBuildCRTSolidGeometry(deoglComputeRenderTask &renderTask,
		const deoglRenderPlanMasked *mask , bool xray);
};

#endif
