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

#ifndef _DEOGLCOLLIDELISTPROPFIELDCLUSTER_H_
#define _DEOGLCOLLIDELISTPROPFIELDCLUSTER_H_

#include "../occlusiontest/deoglOcclusionTestListener.h"

#include <dragengine/common/math/decMath.h>

class deoglOcclusionTest;
class deoglPropFieldCluster;


/**
 * Collide list prop field type cluster.
 */
class deoglCollideListPropFieldCluster : public deoglOcclusionTestListener{
private:
	deoglPropFieldCluster *pCluster;
	bool pCulled;
	int pCascadeMask;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create cluster. */
	deoglCollideListPropFieldCluster();
	
	/** Clean up cluster. */
	~deoglCollideListPropFieldCluster();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Clear. */
	void Clear();
	
	/** Cluster. */
	inline deoglPropFieldCluster *GetCluster() const{return pCluster;}
	
	/** Set cluster. */
	void SetCluster(deoglPropFieldCluster *cluster);
	
	/** Component is culled. */
	inline bool GetCulled() const{return pCulled;}
	
	/** Set component is culled. */
	void SetCulled(bool culled);
	
	/** Cascade mask. */
	inline int GetCascadeMask() const{return pCascadeMask;}
	
	/** Set cascade mask. */
	void SetCascadeMask(int mask);
	
	/** Start occlusion test. */
	void StartOcclusionTest(deoglOcclusionTest &occlusionTest, const decVector &offset);
	
	/** Occlusion test finished with a result of invisible for the element. */
	virtual void OcclusionTestInvisible();
	/*@}*/
};

#endif
