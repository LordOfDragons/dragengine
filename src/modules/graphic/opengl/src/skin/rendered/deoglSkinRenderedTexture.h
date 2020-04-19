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

#ifndef _DEOGLSKINRENDEREDTEXTURE_H_
#define _DEOGLSKINRENDEREDTEXTURE_H_


#include <dragengine/common/math/decMath.h>


class deoglSkinRendered;
class deoglRenderPlan;
class deoglRSkin;


/**
 * \brief Skin rendered texture.
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
	/** \brief Create skin state rendered. */
	deoglSkinRenderedTexture( deoglSkinRendered &skinRendered, deoglRSkin &skin,
		int texture, int modelTexture );
	
	/** \brief Clean up skin state rendered. */
	~deoglSkinRenderedTexture();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Skin rendered. */
	inline deoglSkinRendered &GetSkinRendered() const{ return pSkinRendered; }
	
	/** \brief Add render plans. */
	void AddRenderPlans( deoglRenderPlan &plan );
	
	/** \brief Drop objects containing delayed deletion support. */
	void DropDelayedDeletionObjects();
	/*@}*/
	
	
	
private:
	void pMirrorAddRenderPlans( deoglRenderPlan &plan );
	
	void pPlaneFromTexture( decVector &planeNormal, decVector &planePosition ) const;
	
	void pFrustumFromTexture( int width, int height, double projX, double projY, double near,
		double far, const decDMatrix &matrixInvCamera, const decMatrix &matrixMVP ) const;
};

#endif
