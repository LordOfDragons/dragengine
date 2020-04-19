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

#ifndef _DEOGLRENDERTRANSPARENTPASSES_H_
#define _DEOGLRENDERTRANSPARENTPASSES_H_

#include "deoglRenderBase.h"
#include "../shaders/deoglShaderProgramUsage.h"

class deoglRenderPlan;
class deoglRenderPlanMasked;


/**
 * \brief Transparent passes renderer.
 */
class deoglRenderTransparentPasses : public deoglRenderBase{
private:
	deoglShaderProgramUsage pShaderCopyDepthLimit;
	deoglShaderProgramUsage pShaderCopyDepthColor;
	deoglShaderProgramUsage pShaderCopyColor;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create renderer. */
	deoglRenderTransparentPasses( deoglRenderThread &renderThread );
	
	/** \brief Clean up renderer. */
	virtual ~deoglRenderTransparentPasses();
	/*@}*/
	
	
	
	/** \name Rendering */
	/*@{*/
	/** \brief Render Transparent Passes. */
	void RenderTransparentPasses( deoglRenderPlan &plan, deoglRenderPlanMasked *mask );
	
	/** \brief Render transparent geometry pass. */
	void RenderTransparentGeometryPass( deoglRenderPlan &plan, deoglRenderPlanMasked *mask );
	
	/** \brief Render transparent depth limite pass. */
	void RenderTransparentLimitDepth( deoglRenderPlan &plan, deoglRenderPlanMasked *mask );
	
	/** \brief Render volumetric pass. */
	void RenderVolumetricPass( deoglRenderPlan &plan, deoglRenderPlanMasked *mask, bool inbetween );
	
	/** \brief Copy color texture to temporary texture. */
	void CopyColorToTemporary();
	/*@}*/
};

#endif
