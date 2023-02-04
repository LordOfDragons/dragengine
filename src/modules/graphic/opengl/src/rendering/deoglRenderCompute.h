/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLRENDERCOMPUTE_H_
#define _DEOGLRENDERCOMPUTE_H_

#include "deoglRenderBase.h"


/**
 * Compute stuff used by deoglRenderPlan outside of actual rendering.
 */
class deoglRenderCompute : public deoglRenderBase{
private:
	const deoglPipeline *pPipelineUpdateElements;
	const deoglPipeline *pPipelineFindContentNode;
	const deoglPipeline *pPipelineFindContentElement;
	
	deoglSPBlockSSBO::Ref pSSBOUpdateElements;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create renderer. */
	deoglRenderCompute( deoglRenderThread &renderThread );
	
	/** \brief Clean up renderer. */
	virtual ~deoglRenderCompute();
	/*@}*/
	
	
	
	/** \name Rendering */
	/*@{*/
	/** SSBO update elements. */
	inline const deoglSPBlockSSBO::Ref &GetSSBOUpdateElements() const{ return pSSBOUpdateElements; }
	
	/** Update elements. */
	void UpdateElements( const deoglRenderPlan &plan );
	
	/** Find content. */
	void FindContent( const deoglRenderPlan &plan );
	/*@}*/
	
	
	
protected:
};

#endif
