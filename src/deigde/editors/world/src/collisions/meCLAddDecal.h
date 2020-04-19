/* 
 * Drag[en]gine IGDE World Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _MECLADDDECAL_H_
#define _MECLADDDECAL_H_

#include "meCLHitList.h"

#include <deigde/undo/igdeUndoReference.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>


class meWorld;
class meObject;
class deCollider;
class meWindowMain;



/**
 * Adds a new decal to hit components.
 */
class meCLAddDecal : public deBaseScriptingCollider{
private:
	meWindowMain *pWndMain;
	meWorld *pWorld;
	
	meCLHitList pHitList;
	
	decDVector pRayOrigin;
	decVector pRayDirection;
	
	bool pAlignWithNormal;
	
	igdeUndoReference pUndo;
	
public:
	// constructor, destructor
	meCLAddDecal( meWindowMain *windowMain, meWorld *world );
	virtual ~meCLAddDecal();
	
	// management
	void SetRay( const decDVector &rayOrigin, const decVector &rayDirection );
	/** Sets if the decal has to be aligned with the surface normal upon creation. */
	void SetAlignWithNormal( bool alignWithNormal );
	
	void Reset();
	void RunAction();
	void Finish();
	void Cancel();
	
	// notifications
	virtual void CollisionResponse( deCollider *owner, deCollisionInfo *info );
	virtual bool CanHitCollider( deCollider *owner, deCollider *collider );
	virtual void ColliderChanged( deCollider *owner );
	
private:
	decVector pGetRotationForNormal( const decVector &normal ) const;
};

// end of include only once
#endif
