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

#ifndef _DEOGLRENDERPLANENVMAP_H_
#define _DEOGLRENDERPLANENVMAP_H_

#include <dragengine/common/math/decMath.h>

class deoglEnvironmentMap;



/**
 * @brief Render Plan Environment Map.
 * An environment map used in a render plan to render reflections with. The environment map
 * can be NULL since the render plan uses a fixed size list of environment maps. Additional
 * parameters are stored to allow fading in and out environment maps over time to easy the
 * transition. This state is stored in this object instead of the environment map itself
 * since the same environment map can be used by multiple render plans. Setting and clearing
 * the environment map adds or removes a plan usage from the respective environment map.
 */
class deoglRenderPlanEnvMap{
private:
	deoglEnvironmentMap *pEnvMap;
	float pDistance;
	float pFadeTime;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new render plan env map. */
	deoglRenderPlanEnvMap();
	/** Cleans up the render . */
	~deoglRenderPlanEnvMap();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the environment map or NULL if not set. */
	inline deoglEnvironmentMap *GetEnvMap() const{ return pEnvMap; }
	/** Sets the environment map or NULL if not set. */
	void SetEnvMap( deoglEnvironmentMap *envmap );
	/** Retrieves the distance of the environment map to the camera. */
	inline float GetDistance() const{ return pDistance; }
	/** Sets the distance of the environment map to the camera. */
	void SetDistance( float distance );
	/** Retrieves the fade time. */
	inline float GetFadeTime() const{ return pFadeTime; }
	/** Resets the fade time to 0. */
	void ResetFadeTime();
	/** Increments the fade time. */
	void IncFadeTime( float timestep );
	/*@}*/
};

#endif
