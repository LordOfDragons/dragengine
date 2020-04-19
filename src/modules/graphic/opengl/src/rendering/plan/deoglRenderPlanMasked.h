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

// include only once
#ifndef _DEOGLRENDERPLANMASKED_H_
#define _DEOGLRENDERPLANMASKED_H_

// includes
#include "dragengine/common/math/decMath.h"

// predefinitions
class deoglRenderPlan;
class deoglRComponent;



/**
 * @brief Render Plan Masked.
 *
 * Stores a masked render plan. A masked render plan is used to render
 * views into an world render call using a mask to limit the touched
 * pixels. A render plan masked consists of a render plan itself as
 * well as informations on how to obtain the mask.
 */
class deoglRenderPlanMasked{
private:
	deoglRenderPlan *pPlan;
	
	deoglRComponent *pComponent;
	int pComponentTexture;
	
	bool pUseClipPlane;
	decVector pClipNormal;
	float pClipDistance;
	
	int pStencilMask;
	int pParentStencilMask;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new masked render plan. */
	deoglRenderPlanMasked();
	
	/** Cleans up the masked render plan. */
	~deoglRenderPlanMasked();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the render plan. */
	inline deoglRenderPlan *GetPlan() const{ return pPlan; }
	/** Sets the render plan. */
	void SetPlan( deoglRenderPlan *plan );
	
	/** Retrieves the component or NULL. */
	inline deoglRComponent *GetComponent() const{ return pComponent; }
	/** Retrieves the component texture. */
	inline int GetComponentTexture() const{ return pComponentTexture; }
	/** Sets the component and texture or NULL if unused. */
	void SetComponent( deoglRComponent *component, int texture );
	
	/** Determines if the clip plane is used. */
	inline bool GetUseClipPlane() const{ return pUseClipPlane; }
	/** Sets if the clip plane is used. */
	void SetUseClipPlane( bool useClipPlane );
	/** Retrieves the clipping plane normal. */
	inline const decVector &GetClipNormal() const{ return pClipNormal; }
	/** Sets the clipping plane normal. */
	void SetClipNormal( const decVector &normal );
	/** Retrieves the clipping plane distance. */
	inline float GetClipDistance() const{ return pClipDistance; }
	/** Sets the clipping plane distance. */
	void SetClipDistance( float distance );
	
	/** Retrieves the stencil mask. */
	inline int GetStencilMask() const{ return pStencilMask; }
	/** Sets the stencil mask. */
	void SetStencilMask( int mask );
	/** Retrieves the parent stencil mask. */
	inline int GetParentStencilMask() const{ return pParentStencilMask; }
	/** Sets the parent stencil mask. */
	void SetParentStencilMask( int mask );
	/*@}*/
};

// end of include only once
#endif
