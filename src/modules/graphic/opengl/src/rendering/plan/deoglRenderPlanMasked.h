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

#ifndef _DEOGLRENDERPLANMASKED_H_
#define _DEOGLRENDERPLANMASKED_H_

#include <dragengine/common/math/decMath.h>

class deoglRenderPlan;
class deoglRComponent;

/**
 * Stores a masked render plan. A masked render plan is used to render
 * views into an world render call using a mask to limit the touched
 * pixels. A render plan masked consists of a render plan itself as
 * well as information on how to obtain the mask.
 */
class deoglRenderPlanMasked{
private:
	deoglRenderPlan *pPlan;
	
	deoglRComponent *pComponent;
	int pComponentTexture;
	
	bool pUseClipPlane;
	decVector pClipNormal;
	float pClipDistance;
	decVector pClipNormalStereo;
	float pClipDistanceStereo;
	
	int pStencilMask;
	int pParentStencilMask;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create masked render plan. */
	deoglRenderPlanMasked();
	
	/** Clean up masked render plan. */
	~deoglRenderPlanMasked();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render plan. */
	inline deoglRenderPlan *GetPlan() const{ return pPlan; }
	
	/** Set render plan. */
	void SetPlan( deoglRenderPlan *plan );
	
	/** Component or nuullptr. */
	inline deoglRComponent *GetComponent() const{ return pComponent; }
	
	/** Component texture. */
	inline int GetComponentTexture() const{ return pComponentTexture; }
	
	/** Set component and texture or nullptr if unused. */
	void SetComponent( deoglRComponent *component, int texture );
	
	/** Clip plane is used. */
	inline bool GetUseClipPlane() const{ return pUseClipPlane; }
	
	/** Set if clip plane is used. */
	void SetUseClipPlane( bool useClipPlane );
	
	/** Clipping plane normal. */
	inline const decVector &GetClipNormal() const{ return pClipNormal; }
	
	/** Set clipping plane normal. */
	void SetClipNormal( const decVector &normal );
	
	/** Clipping plane distance. */
	inline float GetClipDistance() const{ return pClipDistance; }
	
	/** Set clipping plane distance. */
	void SetClipDistance( float distance );
	
	/** Stereo clipping plane normal. */
	inline const decVector &GetClipNormalStereo() const{ return pClipNormalStereo; }
	
	/** Set stereo clipping plane normal. */
	void SetClipNormalStereo( const decVector &normal );
	
	/** Stereo clipping plane distance. */
	inline float GetClipDistanceStereo() const{ return pClipDistanceStereo; }
	
	/** Set stereo clipping plane distance. */
	void SetClipDistanceStereo( float distance );
	
	/** Stencil mask. */
	inline int GetStencilMask() const{ return pStencilMask; }
	
	/** Set stencil mask. */
	void SetStencilMask( int mask );
	
	/** Parent stencil mask. */
	inline int GetParentStencilMask() const{ return pParentStencilMask; }
	
	/** Set parent stencil mask. */
	void SetParentStencilMask( int mask );
	/*@}*/
};

#endif
