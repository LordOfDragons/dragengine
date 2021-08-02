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

#ifndef _DEOGLGIBVHSHARED_H_
#define _DEOGLGIBVHSHARED_H_

#include "../deoglBasics.h"
#include "../utils/bvh/deoglBVH.h"

#include <dragengine/deObjectReference.h>
#include <dragengine/common/math/decMath.h>

class deoglGIBVHSharedLocal;
class deoglGIInstance;
class deoglGIInstances;
class deoglRComponent;
class deoglRComponentLOD;
class deoglRComponentTexture;
class deoglRDynamicSkin;
class deoglRenderPlan;
class deoglRenderThread;
class deoglRSkin;
class deoglRWorld;
class deoglSkinState;
class deoglSkinTexture;
class deoglTexUnitsConfig;
class deoglDynamicTBOFloat32;
class deoglDynamicTBOFloat16;
class deoglDynamicTBOUInt32;
class deoglDynamicTBOUInt16;
class deoglDynamicTBOShared;


/**
 * Global illumination BVH.
 */
class deoglGIBVHShared{
private:
	deoglRenderThread &pRenderThread;
	
	deoglDynamicTBOFloat32 *pTBONodeBox;
	deoglDynamicTBOUInt16 *pTBOIndex;
	deoglDynamicTBOUInt16 *pTBOFace;
	deoglDynamicTBOFloat32 *pTBOVertex;
	deoglDynamicTBOFloat16 *pTBOTexCoord;
	deoglDynamicTBOUInt32 *pTBOMaterial;
	deoglDynamicTBOFloat16 *pTBOMaterial2;
	
	deoglDynamicTBOShared *pSharedTBONode;
	deoglDynamicTBOShared *pSharedTBOFace;
	deoglDynamicTBOShared *pSharedTBOVertex;
	deoglDynamicTBOShared *pSharedTBOMaterial;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create global illumination BVH. */
	deoglGIBVHShared( deoglRenderThread &renderThread );
	
	/** Clean up global illumination BVH. */
	~deoglGIBVHShared();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** TBO for BVH node boundaries. */
	inline deoglDynamicTBOFloat32 *GetTBONodeBox() const{ return pTBONodeBox; }
	
	/** TBO for BVH node indices. */
	inline deoglDynamicTBOUInt16 *GetTBOIndex() const{ return pTBOIndex; }
	
	/** TBO for mesh faces. */
	inline deoglDynamicTBOUInt16 *GetTBOFace() const{ return pTBOFace; }
	
	/** TBO for mesh vertices. */
	inline deoglDynamicTBOFloat32 *GetTBOVertex() const{ return pTBOVertex; }
	
	/** TBO for mesh texture coordinates. */
	inline deoglDynamicTBOFloat16 *GetTBOTexCoord() const{ return pTBOTexCoord; }
	
	/** TBO for material parameters. */
	inline deoglDynamicTBOUInt32 *GetTBOMaterial() const{ return pTBOMaterial; }
	
	/** TBO for material float parameters. */
	inline deoglDynamicTBOFloat16 *GetTBOMaterial2() const{ return pTBOMaterial2; }
	
	/** Shared TBOs. */
	inline deoglDynamicTBOShared *GetSharedTBONode() const{ return pSharedTBONode; }
	inline deoglDynamicTBOShared *GetSharedTBOFace() const{ return pSharedTBOFace; }
	inline deoglDynamicTBOShared *GetSharedTBOVertex() const{ return pSharedTBOVertex; }
	inline deoglDynamicTBOShared *GetSharedTBOMaterial() const{ return pSharedTBOMaterial; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
