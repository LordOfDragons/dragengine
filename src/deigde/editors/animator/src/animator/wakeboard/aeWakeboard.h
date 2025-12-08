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
#ifndef _AEWAKEBOARD_H_
#define _AEWAKEBOARD_H_

// includes
#include <dragengine/common/math/decMath.h>

// predefinitions
class aeAnimator;
class deColliderVolume;
class deComponent;



/**
 * Animation Wakeboard.
 *
 * Animation wakeboard.
 */
class aeWakeboard{
private:
	aeAnimator *pAnimator;
	
	deComponent::Ref pEngComponent;
	deColliderVolume::Ref pEngCollider;
	
	bool pEnabled;
	
	decDVector pPosition;
	
	float pTiltUpDown;
	float pMaxTiltUpDown;
	float pTiltLeftRight;
	float pMaxTiltLeftRight;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new wakeboard object. */
	aeWakeboard(aeAnimator *animator);
	/** Cleans up the wakeboard object. */
	~aeWakeboard();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Resets the wakeboard. */
	void Reset();
	
	/** Retrieves the animator. */
	inline aeAnimator *GetAnimator() const{ return pAnimator; }
	/** Retrieves the component. */
	inline const deComponent::Ref &GetComponent() const{ return pEngComponent; }
	/** Retrieves the collider. */
	inline const deColliderVolume::Ref &GetCollider() const{ return pEngCollider; }
	
	/** Determines if the wakeboard is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	/** Sets if the wakeboard is enabled. */
	void SetEnabled(bool enabled);
	
	/** Retrieves the position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	/** Sets the position. */
	void SetPosition(const decDVector &position);
	
	/** Retrieves the up-down tilt in degrees. */
	inline float GetTiltUpDown() const{ return pTiltUpDown; }
	/** Sets the up-down tilt in degrees. */
	void SetTiltUpDown(float tilt);
	/** Retrieves the maximum up-down tilt in degrees. */
	inline float GetMaximumTiltUpDown() const{ return pMaxTiltUpDown; }
	/** Sets the matimum up-down tilt in degrees. */
	void SetMaximumTiltUpDown(float tilt);
	
	/** Retrieves the left-right tilt in degrees. */
	inline float GetTiltLeftRight() const{ return pTiltLeftRight; }
	/** Sets the left-right tilt in degrees. */
	void SetTiltLeftRight(float tilt);
	/** Retrieves the maximum left-right tilt in degrees. */
	inline float GetMaximumTiltLeftRight() const{ return pMaxTiltLeftRight; }
	/** Sets the matimum left-right tilt in degrees. */
	void SetMaximumTiltLeftRight(float tilt);
	
	/** Updates the orientation using the tilt values. */
	void UpdateOrientation();
	/*@}*/
	
private:
	void pCleanUp();
};

// end of include only once
#endif
