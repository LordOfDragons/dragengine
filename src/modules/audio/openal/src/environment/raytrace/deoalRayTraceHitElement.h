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

#ifndef _DEOALRAYTRACEHITELEMENT_H_
#define _DEOALRAYTRACEHITELEMENT_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decTList.h>

class deoalAComponent;


/**
 * Ray tracing hit element.
 */
class deoalRayTraceHitElement{
public:
	class List : public decTList<deoalRayTraceHitElement>{
	public:
		/** Add element to hit component face. */
		void AddComponentFace(float distance, const decDVector &point, const decDVector &normal,
			deoalAComponent *component, int face, bool frontFacing);
	};
	
private:
private:
	float pDistance;
	decDVector pPoint;
	decDVector pNormal;
	bool pForwardFacing;
	
	deoalAComponent *pComponent;
	int pComponentFace;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create empty ray trace hit element. */
	deoalRayTraceHitElement();
	
	/** Create copy of ray trace hit element. */
	deoalRayTraceHitElement(const deoalRayTraceHitElement &element);
	
	/** Create element. */
	deoalRayTraceHitElement(float distance, const decDVector &point, const decDVector &normal,
		deoalAComponent *component, int face, bool frontFacing);
	
	/** Move. */
	deoalRayTraceHitElement(deoalRayTraceHitElement &&element) noexcept = default;
	deoalRayTraceHitElement &operator=(deoalRayTraceHitElement &&element) noexcept = default;
	/*@}*/
	
	
	
	/** \name Manegement */
	/*@{*/
	/** Distance in meters to hit point. */
	inline float GetDistance() const{ return pDistance; }
	
	/** Hit point in world coordinates. */
	inline const decDVector &GetPoint() const{ return pPoint; }
	
	/** Hit normal in world coordinates. */
	inline const decDVector &GetNormal() const{ return pNormal; }
	
	/** Hit element is forward facing. */
	inline bool GetForwardFacing() const{ return pForwardFacing; }
	
	
	
	/** Hit component or NULL. */
	inline deoalAComponent *GetComponent() const{ return pComponent; }
	
	/** Hit component face or -1. */
	inline int GetComponentFace() const{ return pComponentFace; }
	
	
	
	/** Set empty. */
	void SetEmpty();
	
	/** Set element to hit component face. */
	void SetComponentFace(float distance, const decDVector &point, const decDVector &normal,
		deoalAComponent *component, int face, bool frontFacing);
	
	
	
	/** Assign element. */
	deoalRayTraceHitElement &operator=(const deoalRayTraceHitElement &element);
	/*@}*/
};

#endif
