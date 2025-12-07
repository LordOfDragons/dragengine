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

#ifndef _DEOGLRLUMIMETER_H_
#define _DEOGLRLUMIMETER_H_

#include "../light/deoglLightList.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/deObject.h>

class deoglRenderThread;
class deoglRWorld;
class deoglWorldOctree;



/**
 * Render lumimeter.
 */
class deoglRLumimeter : public deObject{
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoglRLumimeter> Ref;


public:
	deoglRenderThread &pRenderThread;
	
	deoglRWorld *pParentWorld;
	deoglWorldOctree *pOctreeNode;
	bool pDirtyOctree;
	
	decDVector pPosition;
	
	float pLuminance;
	decColor pColor;
	
	bool pDirtyMeasurements;
	
	deoglLightList pLights;
	
	bool pWorldMarkedRemove;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render lumimeter. */
	deoglRLumimeter(deoglRenderThread &renderThread);
	
	/** Clean up render lumimeter. */
	virtual ~deoglRLumimeter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Mark lumimeter dirty. */
	void MarkDirty();
	
	/** Parent world or \em NULL if not set. */
	inline deoglRWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** Set parent scene or \em NULL if not set. */
	void SetParentWorld(deoglRWorld *parentWorld);
	
	/** Octree node or \em NULL if not inserted into the parent world octree. */
	inline deoglWorldOctree *GetOctreeNode() const{ return pOctreeNode; }
	
	/**
	 * Set octree node or \em NULL if not inserted into the parent world octree.
	 * \details This call is to be used only by the deoglWorldOctree only.
	 */
	void SetOctreeNode(deoglWorldOctree *octreeNode);
	
	/** Update position in the parent octree. */
	void UpdateOctreeNode();
	
	
	
	/** Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** Set position. */
	void SetPosition(const decDVector &position);
	
	
	
	/** Update measurements. */
	void UpdateMeasurements();
	
	/** Measured luminance. */
	inline float GetLuminance() const{ return pLuminance; }
	
	/** Measured color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	
	
	/** Prepare for quick disposal of lumimeter. */
	void PrepareQuickDispose();
	/*@}*/
	
	
	
	/** \name Lights Management */
	/*@{*/
	/** Number of lights. */
	int GetLightCount() const;
	
	/** Light at index. */
	deoglRLight *GetLightAt(int index) const;
	
	/** Add light. */
	void AddLight(deoglRLight *light);
	
	/** Remove light. */
	void RemoveLight(deoglRLight *light);
	
	/** Remove all lights. */
	void RemoveAllLights();
	/*@}*/
	
	
	
	/** \name Render world usage */
	/*@{*/
	/**
	 * Marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	inline bool GetWorldMarkedRemove() const{ return pWorldMarkedRemove; }
	
	/**
	 * Set marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	void SetWorldMarkedRemove(bool marked);
	/*@}*/
};

#endif
