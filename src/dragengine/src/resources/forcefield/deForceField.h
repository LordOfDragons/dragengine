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

#ifndef _DEFORCEFIELD_H_
#define _DEFORCEFIELD_H_

#include "../deResource.h"
#include "../../common/math/decMath.h"
#include "../../common/utils/decCollisionFilter.h"
#include "../../common/shape/decShapeList.h"

class deForceFieldManager;
class deBasePhysicsForceField;
class deWorld;


/**
 * \brief Force Field.
 *
 * Applies a force to prop fields, particles cast by particle systems and
 * dynamic colliders.
 * 
 * The field affects elements inside its area of effect defined by a shape
 * list. The force is at the maximum inside the shape. Optionally it
 * diminishes by distance from the border of the shape. An exponent can be
 * defined to alter the falloff from the border. No area shape equals a
 * point force field.
 * 
 * The force is applied either radially from the center, linearily along
 * direction or vertex like swirrling around the direction. Negative force
 * creates a force field attracting elements instead of repelling them. For
 * vortrex type positive force swirls around counter-clock-wise while
 * negative force swirls around clock-wise. The force direction is relative
 * to the force field orientation allowing to alter the force direction
 * without needing to adjust the area of influence shape which is oriented
 * to the force field orientation itself. The force is measured in newton
 * and can be applied directly, by surface or by volume.
 * 
 * Directly applied the force affects all elements exactly the same. This
 * can be used for example for paranomal effects where the shape of elements
 * is unimportant.
 * 
 * Applied by surface the set force is Newton per Meter-Squared. The area
 * used to calculate the force is the surface area exposed to the force
 * direction. This can be used for wind or water forces pushing against
 * exposed surface area. Physics modules are free to approximate this surface
 * area as they see fit.
 * 
 * Applied by volume the set force is Newton per Meter-Cubic. The volume used
 * to calculate the force is the real or representative volume of elements.
 * For elements having a shape the volume is the volume of all space occupied
 * by all shapes combined. For particles the shape is usually a sphere roughly
 * representing the particle. This can be used for field effects like magentism
 * which do not care about exposed surface area only the volume they can affect.
 * Physics modules are free to approximate this volume as they see fit.
 * 
 * The force direction and force can be subject to fluctuation to make the
 * force field more life-like. Physics modules alter the applied force direction
 * and force strength randomly over time.
 * 
 * Force fields can be disabled avoiding the need to remove and add them to
 * the world.
 * 
 * Default settings are radial type, apply direct, no shape, 1N force,
 * 1m radius, exponent 1, no fluctuations and enabled.
 * 
 * \todo using a 3d image as custom force field? like red=x-axis, green=y-axis
 * and blue=z-axis?
 * 
 * \todo Force fields can be set to be blocked by non-dynamic colliders? Force
 * fields have no effect on objects along the force direction if a
 * non-dynamic colliders is crossed? (bool pBlocked?)
 */
class DE_DLL_EXPORT deForceField : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deForceField> Ref;
	
	
public:
	/** \brief Field types. */
	enum eFieldTypes{
		/** \brief Force is applied radial from the center. */
		eftRadial,
		
		/** \brief Force is applied along the force direction. */
		eftLinear,
		
		/** \brief Force is applied  vortex like swirling around. */
		eftVortex
	};
	
	/** \brief Force application types. */
	enum eApplicationTypes{
		/** \brief Apply force equally to all elements independend of shape. */
		eatDirect,
		
		/** \brief Apply force on exposed surface area. */
		eatSurface,
		
		/** \brief Apply force on mass. */
		eatMass,
		
		/** \brief Apply force relative to speed of element. */
		eatSpeed
	};
	
	
	
private:
	decDVector pPosition;
	decQuaternion pOrientation;
	
	decShapeList pInfluenceArea;
	float pRadius;
	float pExponent;
	
	eFieldTypes pFieldType;
	eApplicationTypes pApplicationType;
	decVector pDirection;
	float pForce;
	float pFluctuationDirection;
	float pFluctuationForce;
	
	decShapeList pShape;
	decCollisionFilter pCollisionFilter;
	bool pEnabled;
	
	deBasePhysicsForceField *pPeerPhysics;
	
	deWorld *pParentWorld;
	deForceField *pLLWorldPrev;
	deForceField *pLLWorldNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create force field. */
	deForceField(deForceFieldManager *manager);
	
protected:
	/**
	 * \brief Clean up force field.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~deForceField() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition(const decDVector &position);
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation(const decQuaternion &orientation);
	
	
	
	/** \brief Influence area. */
	inline const decShapeList &GetInfluenceArea() const{ return pInfluenceArea; }
	
	/** \brief Set influence area. */
	void SetInfluenceArea(const decShapeList &area);
	
	/** \brief Falloff radius. */
	inline float GetRadius() const{ return pRadius; }
	
	/** \brief Set falloff radius. */
	void SetRadius(float radius);
	
	/** \brief Falloff exponent. */
	inline float GetExponent() const{ return pExponent; }
	
	/** \brief Set falloff exponent. */
	void SetExponent(float exponent);
	
	
	
	/** \brief Field type. */
	inline eFieldTypes GetFieldType() const{ return pFieldType; }
	
	/** \brief Set field type. */
	void SetFieldType(eFieldTypes type);
	
	/** \brief Force application type. */
	inline eApplicationTypes GetApplicationType() const{ return pApplicationType; }
	
	/** \brief Set application type. */
	void SetApplicationType(eApplicationTypes type);
	
	/** \brief Force direction. */
	inline const decVector &GetDirection() const{ return pDirection; }
	
	/** \brief Set force direction. */
	void SetDirection(const decVector &direction);
	
	/** \brief Force in newton. */
	inline float GetForce() const{ return pForce; }
	
	/** \brief Set force in newton. */
	void SetForce(float force);
	
	/** \brief Fluctuation of direction in radians. */
	inline float GetFluctuationDirection() const{ return pFluctuationDirection; }
	
	/** \brief Set fluctuation of direction in radians. */
	void SetFluctuationDirection(float fluctuation);
	
	/** \brief Fluctuation of force in newton. */
	inline float GetFluctuationForce() const{ return pFluctuationForce; }
	
	/** \brief Set fluctuation of force in newton. */
	void SetFluctuationForce(float fluctuation);
	
	
	
	/** \brief Sound shape. */
	inline const decShapeList &GetShape() const{ return pShape; }
	
	/** \brief Set sound shape. */
	void SetShape(const decShapeList &shape);
	
	/** \brief Collision filter. */
	inline const decCollisionFilter &GetCollisionFilter() const{ return pCollisionFilter; }
	
	/** \brief Set collision filter. */
	void SetCollisionFilter(const decCollisionFilter &filter);
	
	/** \brief Force field is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if force field is enabled. */
	void SetEnabled(bool enabled);
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Physics system peer object. */
	inline deBasePhysicsForceField *GetPeerPhysics() const{ return pPeerPhysics; }
	
	/** \brief Set physics system peer object. */
	void SetPeerPhysics(deBasePhysicsForceField *peer);
	/*@}*/
	
	
	
	/** \name Linked List */
	/*@{*/
	/** \brief Parent world or NULL. */
	inline deWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or NULL. */
	void SetParentWorld(deWorld *world);
	
	/** \brief Previous force field in the parent world linked list. */
	inline deForceField *GetLLWorldPrev() const{ return pLLWorldPrev; }
	
	/** \brief Set next force field in the parent world linked list. */
	void SetLLWorldPrev(deForceField *forceField);
	
	/** \brief Next force field in the parent world linked list. */
	inline deForceField *GetLLWorldNext() const{ return pLLWorldNext; }
	
	/** \brief Set next force field in the parent world linked list. */
	void SetLLWorldNext(deForceField *forceField);
	/*@}*/
};

#endif
