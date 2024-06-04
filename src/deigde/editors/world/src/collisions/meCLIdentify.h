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

#ifndef _MECLIDENTIFY_H_
#define _MECLIDENTIFY_H_

#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>
#include <dragengine/common/math/decMath.h>

class meWorld;
class meObject;
class meDecal;
class deCollider;


/**
 * Class for identifying an object.
 */
class meCLIdentify : public deBaseScriptingCollider{
private:
	meWorld &pWorld;
	meObject *pObject;
	meDecal *pDecal;
	float pBestDistance;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create visitor. */
	meCLIdentify( meWorld &world );
	
	/** Clean up visitor. */
	virtual ~meCLIdentify();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Reset visitor. */
	void Reset();
	
	/** Identified object or nullptr. */
	inline meObject *GetObject() const{ return pObject; }
	
	/** Identified decal or nullptr. */
	inline meDecal *GetDecal() const{ return pDecal; }
	
	/** Has identified object. */
	bool HasObject() const;
	
	/** Has identified decal. */
	bool HasDecal() const;
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	virtual void CollisionResponse( deCollider *owner, deCollisionInfo *info );
	virtual bool CanHitCollider( deCollider *owner, deCollider *collider );
	virtual void ColliderChanged( deCollider *owner );
	/*@}*/
	
	
	
private:
	meObject *pGetObjectForCollider( deCollider *collider ) const;
};

#endif
