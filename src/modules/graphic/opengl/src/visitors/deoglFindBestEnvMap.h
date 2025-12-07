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

#ifndef _DEOGLFINDBESTENVMAP_H_
#define _DEOGLFINDBESTENVMAP_H_

#include <dragengine/common/math/decMath.h>

#include "../world/deoglWorldOctreeVisitor.h"

class deoglEnvironmentMap;
class deoglEnvironmentMapList;



/**
 * Find best environment map world octree visitor.
 * Uses as input the position to search the best environment map for. The environment map with the
 * smallest distance to the target position is considered the best. After visiting the found
 * environment map with stored or NULL otherwise if no result has been found.
 */
class deoglFindBestEnvMap : public deoglWorldOctreeVisitor{
private:
	decDVector pPosition;
	deoglEnvironmentMap *pEnvMap; ///< weak reference
	double pDistance;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new visitor. */
	deoglFindBestEnvMap();
	/** Cleans up the visitor. */
	~deoglFindBestEnvMap() override;
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the target position. */
	inline const decDVector &GetPosition() const{
		return pPosition;
	}
	/** Sets the target position. */
	void SetPosition(const decDVector &position);
	
	/** Retrieves the found environment map or NULL if no environment map could be found. */
	inline deoglEnvironmentMap *GetEnvMap() const{
		return pEnvMap;
	}
	/** Retrieves the distance of the found environment map to the target position or 0 if no environment map could be found. */
	inline double GetDistance() const{
		return pDistance;
	}
	
	/** Resets the visitor setting the found environment map to NULL. */
	void Reset();
	/*@}*/
	
	/** \name Visiting */
	/*@{*/
	/** Visits an octree node. */
	void VisitNode(deoglDOctree *node, int intersection) override;
	
	/** Test all environment maps in a list of environment maps. */
	void VisitList(const deoglEnvironmentMapList &list);
	/*@}*/
};

#endif
