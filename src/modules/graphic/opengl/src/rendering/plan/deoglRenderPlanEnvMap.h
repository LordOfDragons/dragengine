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

#ifndef _DEOGLRENDERPLANENVMAP_H_
#define _DEOGLRENDERPLANENVMAP_H_

#include <dragengine/common/math/decMath.h>

class deoglEnvironmentMap;



/**
 * Render Plan Environment Map.
 * An environment map used in a render plan to render reflections with. The environment map
 * can be NULL since the render plan uses a fixed size list of environment maps. Additional
 * parameters are stored to allow fading in and out environment maps over time to easy the
 * transition. This state is stored in this object instead of the environment map itself
 * since the same environment map can be used by multiple render plans. Setting and clearing
 * the environment map adds or removes a plan usage from the respective environment map.
 */
class deoglRenderPlanEnvMap{
private:
	deoglEnvironmentMap::Ref pEnvMap;
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
	void SetEnvMap(deoglEnvironmentMap *envmap);
	/** Retrieves the distance of the environment map to the camera. */
	inline float GetDistance() const{ return pDistance; }
	/** Sets the distance of the environment map to the camera. */
	void SetDistance(float distance);
	/** Retrieves the fade time. */
	inline float GetFadeTime() const{ return pFadeTime; }
	/** Resets the fade time to 0. */
	void ResetFadeTime();
	/** Increments the fade time. */
	void IncFadeTime(float timestep);
	/*@}*/
};

#endif
