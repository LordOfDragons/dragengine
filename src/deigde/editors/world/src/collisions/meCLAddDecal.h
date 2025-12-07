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

#ifndef _MECLADDDECAL_H_
#define _MECLADDDECAL_H_

#include "meCLHitList.h"

#include <deigde/undo/igdeUndo.h>

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
	
	igdeUndo::Ref pUndo;
	
public:
	// constructor, destructor
	meCLAddDecal(meWindowMain *windowMain, meWorld *world);
	virtual ~meCLAddDecal();
	
	// management
	void SetRay(const decDVector &rayOrigin, const decVector &rayDirection);
	/** Sets if the decal has to be aligned with the surface normal upon creation. */
	void SetAlignWithNormal(bool alignWithNormal);
	
	void Reset();
	void RunAction();
	void Finish();
	void Cancel();
	
	// notifications
	virtual void CollisionResponse(deCollider *owner, deCollisionInfo *info);
	virtual bool CanHitCollider(deCollider *owner, deCollider *collider);
	virtual void ColliderChanged(deCollider *owner);
	
private:
	decVector pGetRotationForNormal(const decVector &normal) const;
};

// end of include only once
#endif
