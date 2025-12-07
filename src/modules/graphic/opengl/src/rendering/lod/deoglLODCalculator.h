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

#ifndef _DEOGLLODCALCULATOR_H_
#define _DEOGLLODCALCULATOR_H_

#include <dragengine/common/math/decMath.h>

class deoglCollideList;
class deoglCollideListComponent;



/**
 * Helper class calculating LOD levels to use for different objects. Supports storing results
 * accross multiple frames if required.
 */
class deoglLODCalculator{
private:
	int pMaxPixelError;
	int pLodOffset;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new lod calculator. */
	deoglLODCalculator();
	/** Cleans up the lod calculator. */
	~deoglLODCalculator();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Set the maximum pixel error on screen. */
	void SetMaxPixelError(int maxPixelError);
	
	/** Set lod offset. */
	void SetLodOffset(int offset);
	
	/** Set lod level of all components to 0. */
	void SetComponentLOD0(const deoglCollideList &collideList);
	
	/** Set lod level of all components to highest. */
	void SetComponentLODMax(const deoglCollideList &collideList);
	
	/** Calculate projective lod levels for all components. */
	void SetComponentLODProjection(const deoglCollideList &collideList, const decDVector &position,
		const decDVector &view, float fovX, float fovY, int screenWidth, int screenHeight);
	
	/** Calculate orthographic lod levels for all components. */
	void SetComponentLODOrtho(const deoglCollideList &collideList, float boxWidth, float boxHeight,
		int screenWidth, int screenHeight);
	
	/** Calculate orthographic lod levels for all components. */
	void SetComponentLODOrtho(deoglCollideListComponent &clistComponent, float boxWidth,
		float boxHeight, int screenWidth, int screenHeight);
	
	/** Calculate omnidirection lod levels for all components. */
	void SetComponentLODOmniDir(const deoglCollideList &collideList, const decDVector &position, int size);
	/*@}*/
};

#endif
