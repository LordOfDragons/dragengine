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

#ifndef _DEOGLGIAREATRACKER_H_
#define _DEOGLGIAREATRACKER_H_

#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/common/math/decMath.h>

#include "../configuration/deoglConfiguration.h"
#include "../collidelist/deoglCollideList.h"

class deoglRWorld;
class deoglWorldOctree;
class deoglRComponent;


/**
 * World octree visitor tracking instances entering or leaving GI State area
 * while GI State moves
 */
class deoglGIAreaTracker{
private:
	struct sBox{
		decDVector minExtend, maxExtend;
	};
	
	deoglCollideList pEntering;
	deoglCollideList pLeaving;
	bool pAllLeaving;
	
	deoglRWorld *pWorld;
	decDVector pHalfExtends;
	decLayerMask pLayerMask;
	double pUpdateThreshold;
	int pGIImportance;
	
	decDVector pLastUpdatePosition;
	decDVector pPosition;
	bool pValid;
	
	sBox pBoxLastUpdate;
	sBox pBox;
	sBox pBoxKeep;
	sBox pBoxVisit;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create visitor. */
	deoglGIAreaTracker( int giimportance );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** World or NULL. */
	inline deoglRWorld *GetWorld() const{ return pWorld; }
	
	/** Set world or NULL. */
	void SetWorld( deoglRWorld *world );
	
	/** Half-extends. */
	inline const decDVector &GetHalfExtends() const{ return pHalfExtends; }
	
	/** Set half extends. */
	void SetHalfExtends( const decDVector &halfExtends );
	
	/** Layer mask. */
	const decLayerMask &GetLayerMask(){ return pLayerMask; }
	
	/** Set layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	/** Position distance threshold before updating tracking. */
	inline double GetUpdateThreshold() const{ return pUpdateThreshold; }
	
	/** Set position distance threshold before updating tracking. */
	void SetUpdateThreshold( double threshold );
	
	/** Set GI importance. */
	void SetGIImportance( int importance );
	
	
	
	/** Position tracking is valid for. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** Set position. */
	void SetPosition( const decDVector &position );
	
	/** Tracking is valid. If not becomes valid after next update. */
	inline bool GetValid() const{ return pValid; }
	
	
	
	/** Update tracked instances. */
	void Update();
	
	
	
	/** List of instances entering area. Valid after Update() has been called. */
	inline const deoglCollideList &GetEntering() const{ return pEntering; }
	
	/** List of instances leaving area. Valid after Update() has been called. */
	inline const deoglCollideList &GetLeaving() const{ return pLeaving; }
	
	/** All instances area leaving. Valid after Update() has been called. */
	inline bool GetAllLeaving() const{ return pAllLeaving; }
	
	/** Entering or leaving list is not empty or all leaving flag is set. */
	bool HasChanged() const;
	
	/** Entering and leaving list are empty and all leaving flag is cleared. */
	bool HasNotChanged() const;
	
	/** Clear lists and all leaving flag. */
	void ClearChanges();
	
	/** Reject component. */
	bool RejectComponent( const deoglRComponent &component ) const;
	
	/** Component touches current box. */
	bool ComponentTouches( const deoglRComponent &component ) const;
	
	
	
	/** GI importance from GI quality. */
	static int GIImportanceFromGIQuality( deoglConfiguration::eGIQuality quality );
	/*@}*/
	
	
	
private:
	void pVisitNodeColliding( const deoglWorldOctree &node );
	void pVisitNode( const deoglWorldOctree &node );
	void pVisitComponents( const deoglWorldOctree &node );
	
	void pVisitNodeCollidingNewOnly( const deoglWorldOctree &node );
	void pVisitNodeNewOnly( const deoglWorldOctree &node );
	void pVisitComponentsNewOnly( const deoglWorldOctree &node );
};

#endif
