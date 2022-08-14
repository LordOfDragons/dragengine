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

#ifndef _DEOGLRENDERTRANSPCOUNTING_H_
#define _DEOGLRENDERTRANSPCOUNTING_H_

#include "deoglRenderBase.h"

class deoglRenderPlan;
class deoglRenderPlanMasked;
class deoglOcclusionQuery;


/**
 * \brief Transparency counting renderer.
 */
class deoglRenderTranspCounting : public deoglRenderBase{
private:
	deoglOcclusionQuery *pOccQuery;
	
	deoglShaderProgramUsage pShaderTraCountMaxCount;
	deoglShaderProgramUsage pShaderTraCountMaxCountStereo;
	
	deoglShaderProgramUsage pShaderTraCountGetCount;
	deoglShaderProgramUsage pShaderTraCountGetCountStereo;
	
	bool pHasCount;
	int pCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create renderer. */
	deoglRenderTranspCounting( deoglRenderThread &renderThread );
	
	/** \brief Clean up renderer. */
	virtual ~deoglRenderTranspCounting();
	/*@}*/
	
	
	
	/** \name Rendering */
	/*@{*/
	/**
	 * \brief Count transparency.
	 * 
	 * Using FBO Def-Ren Diffuse+Depth. Clears diffuse buffer.
	 * - render transparent geometry writing 1/255 for each fragment
	 * Invalidates no attachments
	 * 
	 * Using ping-pong between FBO Def-Ren Diffuse and Reflectivity. Clear respective
	 * texture each turn. Invalidates no attachments each turn.
	 * 
	 * Using last used Diffuse/Reflectivity buffer. Clears nothing. Invalidates buffer
	 * since result is written to render plan.
	 */
	void CountTransparency( deoglRenderPlan &plan, const deoglRenderPlanMasked *mask );
	
	/**
	 * \brief Last found transparency count.
	 * 
	 * Count is cached until the next time CountTransparency() is called.
	 */
	int GetCount();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
