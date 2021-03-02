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

#ifndef _DEOGLRENDERGI_H_
#define _DEOGLRENDERGI_H_

#include "deoglRenderLightBase.h"

#include <dragengine/deObjectReference.h>


class deoglRayTraceField;
class deoglSPBlockUBO;
class deoglShaderProgram;
class deoglGIState;
class deoglRenderTask;
class deoglAddToRenderTaskGIMaterial;


/**
 * \brief GI Renderer.
 */
class deoglRenderGI : public deoglRenderLightBase{
private:
	deoglShaderProgram *pShaderFieldTraceRays;
	deoglShaderProgram *pShaderTraceRays;
	deoglShaderProgram *pShaderUpdateProbeIrradiance;
	deoglShaderProgram *pShaderUpdateProbeDistance;
	deoglShaderProgram *pShaderLight;
	
	deObjectReference pUBORenderLight;
	deObjectReference pUBORenderLightSSS;
	
	deoglRenderTask *pRenderTask;
	deoglAddToRenderTaskGIMaterial *pAddToRenderTask;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create renderer. */
	deoglRenderGI( deoglRenderThread &renderThread );
	
	/** \brief Clean up renderer. */
	virtual ~deoglRenderGI();
	/*@}*/
	
	
	
	/** \name Rendering */
	/*@{*/
	/** \brief Render light UBO. */
	inline deoglSPBlockUBO &GetUBORenderLight() const{ return ( deoglSPBlockUBO& )( deObject& )pUBORenderLight; }
	
	/** \brief GI state to update or NULL. */
	deoglGIState *GetUpdateGIState( const deoglRenderPlan &plan ) const;
	
	/** \brief GI state to render or NULL. */
	deoglGIState *GetRenderGIState( const deoglRenderPlan &plan ) const;
	
	/** \brief Render task for GI material. */
	inline deoglRenderTask &GetRenderTask() const{ return *pRenderTask; }
	
	/** \brief Add to render task for GI material. */
	inline deoglAddToRenderTaskGIMaterial &GetAddToRenderTask() const{ return *pAddToRenderTask; }
	
	
	
	/** \brief Trace rays for field. */
	void TraceRays( deoglRayTraceField &field );
	
	/** \brief Trace rays. */
	void TraceRays( deoglRenderPlan &plan );
	
	/** \brief Prepare render light UBO. */
	void PrepareUBORenderLight( deoglRenderPlan &plan );
	
	/** \brief Update probes. */
	void UpdateProbes( deoglRenderPlan &plan );
	
	/** \brief Render light. */
	void RenderLight( deoglRenderPlan &plan, bool solid );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pCreateUBORenderLight();
};

#endif
