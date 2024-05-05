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

#ifndef _DEOGLSKINRENDEREDTEXTURE_H_
#define _DEOGLSKINRENDEREDTEXTURE_H_


#include <dragengine/common/math/decMath.h>


class deoglSkinRendered;
class deoglRenderPlan;
class deoglRSkin;


/**
 * Skin rendered texture.
 */
class deoglSkinRenderedTexture{
private:
	deoglSkinRendered &pSkinRendered;
	deoglRSkin &pSkin;
	int pTexture;
	int pModelTexture;
	deoglRenderPlan *pPlan;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin state rendered. */
	deoglSkinRenderedTexture( deoglSkinRendered &skinRendered, deoglRSkin &skin,
		int texture, int modelTexture );
	
	/** Clean up skin state rendered. */
	~deoglSkinRenderedTexture();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Skin rendered. */
	inline deoglSkinRendered &GetSkinRendered() const{ return pSkinRendered; }
	
	/** Add render plans. */
	void AddRenderPlans( deoglRenderPlan &plan );
	
	/** Drop objects containing delayed deletion support. */
	void DropDelayedDeletionObjects();
	/*@}*/
	
	
	
private:
	struct sMirrorMatrix{
		decDMatrix ownerMatrix;
		decVector planeNormal;
		decVector planePosition;
		decDMatrix mirrorMatrix;
		decDVector mirrorNormal;
		decDVector mirrorRefPoint;
		decDMatrix mirrorFreeMatrix;
	};
	
	void pMirrorAddRenderPlans( deoglRenderPlan &plan );
	
	void pPlaneFromTexture( sMirrorMatrix &mirrorMatrix ) const;
	
	void pFrustumFromTexture( int width, int height, double projX, double projY, double near,
		double far, const decDMatrix &matrixInvCamera, const decMatrix &matrixMVP ) const;
	
	void pMirrorMatrix( const decDMatrix &invCamMatrix, sMirrorMatrix &mirrorMatrix );
};

#endif
