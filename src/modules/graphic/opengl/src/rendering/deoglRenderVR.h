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

#ifndef _DEOGLRENDERVR_H_
#define _DEOGLRENDERVR_H_

#include "deoglRenderBase.h"

#include <dragengine/common/collection/decPointerList.h>

class deoglRWorld;


/**
 * OpenGL VR renderer.
 */
class deoglRenderVR : public deoglRenderBase{
private:
	deoglShaderProgramUsage pShaderHiddenArea;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create renderer. */
	deoglRenderVR( deoglRenderThread &renderThread );
	
	/** Clean up renderer. */
	~deoglRenderVR();
	/*@}*/
	
	
	
	/** \name Rendering */
	/*@{*/
	/** Render hidden area. */
	void RenderHiddenArea( deoglRenderPlan &plan );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
