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

#ifndef _DEOGLGIBVHSHARED_H_
#define _DEOGLGIBVHSHARED_H_

#include "../deoglBasics.h"
#include "../utils/bvh/deoglBVH.h"

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
	deoglGIBVHShared(deoglRenderThread &renderThread);
	
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
