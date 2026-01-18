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

#ifndef _DEBPPROPFIELD_H_
#define _DEBPPROPFIELD_H_

// includes
#include <dragengine/common/collection/decTUniqueList.h>
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
	decTUniqueList<debpPropFieldType> pTypes;
	bool pDirty;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new peer. */
	debpPropField(dePhysicsBullet *bullet, dePropField *forceField);
	/** Cleans up the peer. */
	~debpPropField() override;
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module. */
	inline dePhysicsBullet *GetBullet() const{ return pBullet; }
	/** Retrieves the force field. */
	inline dePropField *GetPropField() const{ return pPropField; }
	
	/** Retrieves the number of types. */
	inline int GetTypeCount() const{ return pTypes.GetCount(); }
	/** Retrieves the type at the given index. */
	debpPropFieldType *GetTypeAt(int index) const;
	
	/** Updates the prop field if required. */
	void Update(float elapsed);
	/*@}*/
	
	/** @name Notification */
	/*@{*/
	/** Position changed. */
	void PositionChanged() override;
	
	/** Type has been added. */
	void TypeAdded(int index, dePropFieldType *type) override;
	/** Type has been removed. */
	void TypeRemoved(int index, dePropFieldType *type) override;
	/** All types have been removed. */
	void AllTypesRemoved() override;
	/** Type changed. */
	void TypeChanged(int index, dePropFieldType *type) override;
	
	/** Instances changed. */
	void InstancesChanged(int index, dePropFieldType *type) override;
	
	/** Project all instances to the given ground. */
	void ProjectInstances(const dePropFieldGround &ground, const decVector &direction) override;
	/*@}*/
	
private:
	void pProjectInstancesDown(const dePropFieldGround &ground);
};

// end of include only once
#endif
