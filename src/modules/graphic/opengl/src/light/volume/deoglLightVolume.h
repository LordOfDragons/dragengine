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

#ifndef _DEOGLLIGHTVOLUME_H_
#define _DEOGLLIGHTVOLUME_H_

#include <dragengine/common/math/decMath.h>

#include "../../deoglBasics.h"


class deoglRenderThread;
class decConvexVolumeList;



/**
 * Light Volume.
 * Convex light volume enclosing the area of effect of a light source. The volume is guaranteed to be not
 * smaller than the real area of effect of the light source. Most of the time the volume is though larger
 * especially if a tight fitting volume would be concave. The volume can be constructed from a convex
 * volume list.
 */
class deoglLightVolume{
public:
	deoglRenderThread &pRenderThread;
	
	decVector pMinExtend;
	decVector pMaxExtend;
	
	GLuint pVAO;
	
	oglVector3 *pPoints;
	int pPointCount;
	
	GLuint pVBO;
	int pVBOPointCount;
	bool pDirtyVBO;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new light volume. */
	deoglLightVolume(deoglRenderThread &renderThread);
	/** Cleans up the light volume. */
	~deoglLightVolume();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the minimum extend. */
	inline const decVector &GetMinimumExtend() const{return pMinExtend;}
	/** Retrieves the maximum extend. */
	inline const decVector &GetMaximumExtend() const{return pMaxExtend;}
	/** Sets the extends. */
	void SetExtends(const decVector &minExtend, const decVector &maxExtend);
	/** Calculate axis aligned bounding box for a specific transformation. */
	void CalcBoundingBox(decDVector &boxMinExtend, decDVector &boxMaxExtend, const decDMatrix &matrix) const;
	
	/** Retrieves the points. */
	inline oglVector3 *GetPoints() const{return pPoints;}
	/** Retrieves the number of points. */
	inline int GetPointCount() const{return pPointCount;}
	/** Sets the number of points. */
	void SetPointCount(int count);
	
	/** Retrieves the VAO. */
	inline GLuint GetVAO() const{return pVAO;}
	/** Updates the VBO from the points. */
	void UpdateVBO();
	
	/** Creates the light volume using a convex volume list. Creates the VBO and calculates the extends. */
	void CreateFrom(const decConvexVolumeList &list);
	/*@}*/
};

#endif
