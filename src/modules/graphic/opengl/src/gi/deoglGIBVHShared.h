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
#include "../tbo/deoglDynamicTBOFloat32.h"
#include "../tbo/deoglDynamicTBOFloat16.h"
#include "../tbo/deoglDynamicTBOUInt32.h"
#include "../tbo/deoglDynamicTBOUInt16.h"
#include "../tbo/deoglDynamicTBOShared.h"


/**
 * Global illumination BVH.
 */
class deoglGIBVHShared{
private:
	deoglRenderThread &pRenderThread;
	
	deoglDynamicTBOFloat32::Ref pTBONodeBox;
	deoglDynamicTBOUInt16::Ref pTBOIndex;
	deoglDynamicTBOUInt16::Ref pTBOFace;
	deoglDynamicTBOFloat32::Ref pTBOVertex;
	deoglDynamicTBOFloat16::Ref pTBOTexCoord;
	deoglDynamicTBOUInt32::Ref pTBOMaterial;
	deoglDynamicTBOFloat16::Ref pTBOMaterial2;
	
	deoglDynamicTBOShared::Ref pSharedTBONode;
	deoglDynamicTBOShared::Ref pSharedTBOFace;
	deoglDynamicTBOShared::Ref pSharedTBOVertex;
	deoglDynamicTBOShared::Ref pSharedTBOMaterial;
	
	
	
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
	inline const deoglDynamicTBOFloat32::Ref &GetTBONodeBox() const{ return pTBONodeBox; }
	
	/** TBO for BVH node indices. */
	inline const deoglDynamicTBOUInt16::Ref &GetTBOIndex() const{ return pTBOIndex; }
	
	/** TBO for mesh faces. */
	inline const deoglDynamicTBOUInt16::Ref &GetTBOFace() const{ return pTBOFace; }
	
	/** TBO for mesh vertices. */
	inline const deoglDynamicTBOFloat32::Ref &GetTBOVertex() const{ return pTBOVertex; }
	
	/** TBO for mesh texture coordinates. */
	inline const deoglDynamicTBOFloat16::Ref &GetTBOTexCoord() const{ return pTBOTexCoord; }
	
	/** TBO for material parameters. */
	inline const deoglDynamicTBOUInt32::Ref &GetTBOMaterial() const{ return pTBOMaterial; }
	
	/** TBO for material float parameters. */
	inline const deoglDynamicTBOFloat16::Ref &GetTBOMaterial2() const{ return pTBOMaterial2; }
	
	/** Shared TBOs. */
	inline const deoglDynamicTBOShared::Ref &GetSharedTBONode() const{ return pSharedTBONode; }
	inline const deoglDynamicTBOShared::Ref &GetSharedTBOFace() const{ return pSharedTBOFace; }
	inline const deoglDynamicTBOShared::Ref &GetSharedTBOVertex() const{ return pSharedTBOVertex; }
	inline const deoglDynamicTBOShared::Ref &GetSharedTBOMaterial() const{ return pSharedTBOMaterial; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
