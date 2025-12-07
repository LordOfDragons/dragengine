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

// include only once
#ifndef _DEBPPROPFIELD_H_
#define _DEBPPROPFIELD_H_

// includes
#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/physics/deBasePhysicsPropField.h>

// predefinitions
class debpWorld;
class debpPropFieldType;
class dePhysicsBullet;

class dePropField;



/**
 * @brief Prop Field.
 *
 * Peer for the force field resource.
 */
class debpPropField : public deBasePhysicsPropField{
private:
	dePhysicsBullet *pBullet;
	dePropField *pPropField;
	
	debpPropFieldType **pTypes;
	int pTypeCount;
	int pTypeSize;
	
	bool pDirty;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new peer. */
	debpPropField(dePhysicsBullet *bullet, dePropField *forceField);
	/** Cleans up the peer. */
	virtual ~debpPropField();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module. */
	inline dePhysicsBullet *GetBullet() const{ return pBullet; }
	/** Retrieves the force field. */
	inline dePropField *GetPropField() const{ return pPropField; }
	
	/** Retrieves the number of types. */
	inline int GetTypeCount() const{ return pTypeCount; }
	/** Retrieves the type at the given index. */
	debpPropFieldType *GetTypeAt(int index) const;
	
	/** Updates the prop field if required. */
	void Update(float elapsed);
	/*@}*/
	
	/** @name Notification */
	/*@{*/
	/** Position changed. */
	virtual void PositionChanged();
	
	/** Type has been added. */
	virtual void TypeAdded(int index, dePropFieldType *type);
	/** Type has been removed. */
	virtual void TypeRemoved(int index, dePropFieldType *type);
	/** All types have been removed. */
	virtual void AllTypesRemoved();
	/** Type changed. */
	virtual void TypeChanged(int index, dePropFieldType *type);
	
	/** Instances changed. */
	virtual void InstancesChanged(int index, dePropFieldType *type);
	
	/** Project all instances to the given ground. */
	virtual void ProjectInstances(const dePropFieldGround &ground, const decVector &direction);
	/*@}*/
	
private:
	void pCleanUp();
	void pProjectInstancesDown(const dePropFieldGround &ground);
};

// end of include only once
#endif
