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
	
	deoglRenderTask pSolidDepthTask;
	deoglRenderTask pSolidDepthOutlineTask;
	deoglRenderTask pSolidGeometryTask;
	deoglRenderTask pSolidGeometryHeight1Task;
	deoglRenderTask pSolidGeometryHeight2Task;
	deoglRenderTask pSolidGeometryOutlineTask;
	deoglRenderTask pSolidDecalsTask;
	
	deoglRPTBuildRTsDepth *pTaskDepth;
	deoglRPTBuildRTsGeometry *pTaskGeometry;
	
	
	
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
	
	/** Solid depth. */
	inline deoglRenderTask &GetSolidDepthTask(){ return pSolidDepthTask; }
	
	/** Solid depth outline. */
	inline deoglRenderTask &GetSolidDepthOutlineTask(){ return pSolidDepthOutlineTask; }
	
	/** Solid geometry. */
	inline deoglRenderTask &GetSolidGeometryTask(){ return pSolidGeometryTask; }
	
	/** Solid geometry height terrain. */
	inline deoglRenderTask &GetSolidGeometryHeight1Task(){ return pSolidGeometryHeight1Task; }
	inline deoglRenderTask &GetSolidGeometryHeight2Task(){ return pSolidGeometryHeight2Task; }
	
	/** Solid geometry outline. */
	inline deoglRenderTask &GetSolidGeometryOutlineTask(){ return pSolidGeometryOutlineTask; }
	
	/** Solid decals. */
	inline deoglRenderTask &GetSolidDecalsTask(){ return pSolidDecalsTask; }
	
	/** Start building render tasks. */
	void StartBuildTasks( const deoglRenderPlanMasked *mask );
	
	/** Wait for task building to finish. */
	void WaitFinishBuildingTasksDepth();
	void WaitFinishBuildingTasksGeometry();
	
	/** Clean up after rendering. */
	void CleanUp();
	/*@}*/
};

#endif
