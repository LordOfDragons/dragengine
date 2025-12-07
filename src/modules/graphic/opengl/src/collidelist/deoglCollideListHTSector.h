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

#ifndef _DEOGLCOLLIDELISTHTSECTOR_H_
#define _DEOGLCOLLIDELISTHTSECTOR_H_

#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>

class deoglHTViewSector;
class deoglOcclusionTest;
class deoglCollideListHTSCluster;


/**
 * Collide List Height Terrain Sector.
 */
class deoglCollideListHTSector{
private:
	deoglHTViewSector *pSector;
	
	decPointerList pClusters;
	int pClusterCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create height terrain sector. */
	deoglCollideListHTSector();
	
	/** Clean up height terrain sector. */
	~deoglCollideListHTSector();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Clear sector. */
	void Clear();
	
	/** Start occlusion test. */
	void StartOcclusionTest(deoglOcclusionTest &occlusionTest, const decDVector &referencePosition);
	
	
	
	/** Sector. */
	inline deoglHTViewSector *GetSector() const{return pSector;}
	
	/** Set sector. */
	void SetSector(deoglHTViewSector *sector);
	
	/** Count of clusters. */
	inline int GetClusterCount() const{return pClusterCount;}
	
	/** Cluster at index. */
	deoglCollideListHTSCluster &GetClusterAt(int index) const;
	
	/** Add cluster. */
	deoglCollideListHTSCluster *AddCluster(const decPoint &coordinates);
	
	/** Remove all clusters. */
	void RemoveAllClusters();
	
	/** Remove culled clusters. */
	void RemoveCulledClusters();
	/*@}*/
};

#endif
