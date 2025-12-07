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

#ifndef _DEBPFORCEFIELD_H_
#define _DEBPFORCEFIELD_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/physics/deBasePhysicsForceField.h>

class debpWorld;
class debpFFVortex;
class dePhysicsBullet;

class deForceField;



/**
 * \brief Force field.
 */
class debpForceField : public deBasePhysicsForceField{
private:
	dePhysicsBullet &pBullet;
	const deForceField &pForceField;
	
	debpFFVortex **pVortices;
	int pVortexCount;
	int pVortexSize;
	float pTimeUntilNextVortex;
	
	decVector pDirection;
	decMatrix pVortexMatrix;
	bool pDirtyGeometry;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	debpForceField(dePhysicsBullet &bullet, const deForceField &forceField);
	
	/** \brief Clean up peer. */
	virtual ~debpForceField();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline dePhysicsBullet &GetBullet() const{return pBullet;}
	
	/** \brief Force field engine resource. */
	inline const deForceField &GetForceField() const{return pForceField;}
	
	
	
	/** \brief Force field direction vector. */
	inline const decVector &GetDirection() const{return pDirection;}
	
	/** \brief Vortex matrix. */
	inline const decMatrix &GetVortexMatrix() const{return pVortexMatrix;}
	
	
	
	/** \brief Update force field. */
	void Update(float elapsed);
	
	
	
	/**
	 * Retrieves the force acting on a given point. The point has to be in the force field
	 * coordinate frame. The result force is stored in the force vector. */
	//void GetForceAtPoint( const decVector &position, decVector &force );
	/*@}*/
	
	
	
	/** \name Vortices */
	/*@{*/
	/** \brief Number of vortices. */
	inline int GetVortexCount() const{return pVortexCount;}
	
	/** \brief Vortex at the given index. */
	debpFFVortex *GetVortexAt(int index) const;
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Influence area changed. */
	virtual void InfluenceAreaChanged();
	
	/** \brief Radius changed. */
	virtual void RadiusChanged();
	
	/** \brief Exponent changed. */
	virtual void ExponentChanged();
	
	/** \brief Field or application type changed. */
	virtual void TypeChanged();
	
	/** \brief Direction changed. */
	virtual void DirectionChanged();
	
	/** \brief Force changed. */
	virtual void ForceChanged();
	
	/** \brief Direction or force fluctuation changed. */
	virtual void FluctuationChanged();
	
	/** \brief Collision filter changed. */
	virtual void CollisionFilterChanged();
	
	/** \brief Enabled changed. */
	virtual void EnabledChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pUpdateGeometry();
};

#endif
