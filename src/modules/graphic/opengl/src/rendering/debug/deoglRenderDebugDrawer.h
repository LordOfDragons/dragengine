/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLRENDERDEBUGDRAWER_H_
#define _DEOGLRENDERDEBUGDRAWER_H_

#include "../deoglRenderBase.h"

class deoglRenderPlan;
class deoglRDebugDrawer;



/**
 * @brief Render Debug Drawer.
 *
 * Provides support for rendering debug drawers.
 */
class deoglRenderDebugDrawer : public deoglRenderBase{
private:
	deoglShaderProgramUsage pShaderShapeXRay;
	deoglShaderProgramUsage pShaderShapeXRayStereo;
	deoglShaderProgramUsage pShaderShapeSolid;
	deoglShaderProgramUsage pShaderShapeSolidStereo;
	deoglShaderProgramUsage pShaderMeshXRay;
	deoglShaderProgramUsage pShaderMeshXRayStereo;
	deoglShaderProgramUsage pShaderMeshSolid;
	deoglShaderProgramUsage pShaderMeshSolidStereo;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new renderer. */
	deoglRenderDebugDrawer( deoglRenderThread &renderThread );
	/** Cleans up the renderer. */
	virtual ~deoglRenderDebugDrawer();
	/*@}*/
	
	/** @name Rendering */
	/*@{*/
	/** Renders debug drawers. */
	void RenderDebugDrawers( deoglRenderPlan &plan );
	/*@}*/
	
private:
	void pCleanUp();
	
	void pRenderDDSShapes( const deoglRenderPlan &plan, const decDMatrix &matrixModel,
		deoglRDebugDrawer &debugDrawer );
	
	void pRenderDDSFaces( const deoglRenderPlan &plan, const decDMatrix &matrixModel,
		deoglRDebugDrawer &debugDrawer );
};

#endif
