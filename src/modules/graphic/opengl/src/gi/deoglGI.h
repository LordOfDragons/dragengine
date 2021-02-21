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

#ifndef _DEOGLGI_H_
#define _DEOGLGI_H_

#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../tbo/deoglDynamicTBOFloat32.h"
#include "../tbo/deoglDynamicTBOUInt32.h"

class deoglRenderThread;
class deoglROcclusionMesh;


/**
 * \brief Global illumination.
 * 
 * Shared parameters and data for global illumination.
 */
class deoglGI{
public:
	/** \brief UBO Parameters. */
	enum eUBOTracingParameter{
		eutpProbeCount,
		eutpProbesPerLine,
		eutpProbeSize,
		eutpResolution,
		eutpSpacing,
		eutpOrigin
	};
	
	
	
private:
	deoglRenderThread &pRenderThread;
	
	int pRaysPerProbe;
	deoglSPBlockUBO *pUBOTracing;
	
	deoglDynamicTBOFloat32 pTBONodeBox;
	deoglDynamicTBOUInt32 pTBOIndex;
	deoglDynamicTBOUInt32 pTBOFace;
	deoglDynamicTBOFloat32 pTBOVertex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create global illumination. */
	deoglGI( deoglRenderThread &renderThread );
	
	/** \brief Clean up global illumination. */
	~deoglGI();
	/*@}*/
	
	
	
	/** @name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** \brief Rays per probe. */
	inline int GetRaysPerProbe() const{ return pRaysPerProbe; }
	
	/** \brief UBO Tracing parameters preparing it if not created yet. */
	deoglSPBlockUBO &GetUBOTracing();
	
	/** \brief TBO for BVH node boundaries. */
	inline const deoglDynamicTBOFloat32 &GetTBONodeBox() const{ return pTBONodeBox; }
	
	/** \brief TBO for BVH node indices. */
	inline const deoglDynamicTBOUInt32 &GetTBOIndex() const{ return pTBOIndex; }
	
	/** \brief TBO for mesh faces. */
	inline const deoglDynamicTBOUInt32 &GetTBOFace() const{ return pTBOFace; }
	
	/** \brief TBO for mesh vertices. */
	inline const deoglDynamicTBOFloat32 &GetTBOVertex() const{ return pTBOVertex; }
	
	
	
	/** \brief Prepare ray tracing information. */
	void PrepareRayTracing( deoglROcclusionMesh &occlusionMesh );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pWriteTBOs();
};

#endif
