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

// include only once
#ifndef _DEBPFFVORTEX_H_
#define _DEBPFFVORTEX_H_

// includes
#include <dragengine/common/math/decMath.h>

// predefinitions



/**
 * @brief Force Field Vortex.
 *
 * A vortex in a force field to create more interesting waving behavior
 * of prop field instances. To describe a vortex a few parameters are
 * used avoiding complicated calculations. The velocity defines the
 * magnitude of the velocity vectory on the rings close to the center
 * of the vortex. The rotation speed of objects depending on the distance
 * to the center of the vortex is calculated directly from the velocity
 * parameter. The vortex itself is considered to be a 2D rotational
 * force field with an orientation in the world.
 */
class debpFFVortex{
private:
	decVector pPosition;
	decVector pView;
	float pSize;
	
	float pFullVelocity;
	float pCurrentVelocity;
	
	float pTimeToLive;
	float pLivedTime;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new vortex. */
	debpFFVortex();
	/** Cleans up the vortex. */
	~debpFFVortex();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the position. */
	inline const decVector &GetPosition() const{return pPosition;}
	/** Sets the position. */
	void SetPosition(const decVector &position);
	/** Retrieves the view direction. */
	inline const decVector &GetView() const{return pView;}
	/** Sets the view direction. */
	void SetView(const decVector &view);
	/** Retrieves the size. */
	inline float GetSize() const{return pSize;}
	/** Sets the size. */
	void SetSize(float size);
	
	/** Retrieves the full velocity. */
	inline float GetFullVelocity() const{return pFullVelocity;}
	/** Sets the full velocity. */
	void SetFullVelocity(float velocity);
	/** Retrieves the current velocity. */
	inline float GetCurrentVelocity() const{return pCurrentVelocity;}
	/** Sets the current velocity. */
	void SetCurrentVelocity(float velocity);
	
	/** Retrieves the time to live. */
	inline float GetTimeToLive() const{return pTimeToLive;}
	/** Sets the time to live. */
	void SetTimeToLive(float time);
	/** Retrieves the lived time. */
	inline float GetLivedTime() const{return pLivedTime;}
	/** Sets the lived time. */
	void SetLivedTime(float time);
	/** Increases the lived time. */
	void IncreaseLivedTime(float time);
	/** Determines if the vortex is still living. */
	inline bool IsLiving() const{return pLivedTime < pTimeToLive;}
	/** Determines if the vortex has died. */
	inline bool HasDied() const{return pLivedTime >= pTimeToLive;}
	
	/** Updates the parameters depending on the lived time. */
	void Update();
	/*@}*/
};

// end of include only once
#endif
