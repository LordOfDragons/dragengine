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

#ifndef _DEBPSHAREDCOLLISIONFILTERING_H_
#define _DEBPSHAREDCOLLISIONFILTERING_H_

class debpCollisionObject;
class debpCollider;
class debpHTSector;
class debpTouchSensor;
class debpWorld;

class btCollisionObject;


/**
 * \brief Shared collision filter functions.
 */
class debpSharedCollisionFiltering{
private:
	debpWorld &pWorld;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create shared collision filtering. */
	debpSharedCollisionFiltering( debpWorld &world );
	
	/** \brief Clean up shared collision filtering. */
	~debpSharedCollisionFiltering();
	/*@}*/
	
	
	
	/** \name Filtering */
	/*@{*/
	/** \brief Check if collision body can hit collision body. */
	bool CanBodyHitBody( const btCollisionObject &body0, const btCollisionObject &body1 ) const;
	
	/** \brief Check if collision object can hit collision object. */
	bool CanCObjHitCObj( const debpCollisionObject &colObj0, const debpCollisionObject &colObj1 ) const;
	
	
	/** \brief Check if collider can hit collision object. */
	bool CanColliderHitCObj( const debpCollisionObject &colObj0, const debpCollider &collider0,
		const debpCollisionObject &colObj1 ) const;
	
	/** \brief Check if height terrain can hit with collision object. */
	bool CanHTerrainHitCObj( const debpHTSector &htsector, const debpCollisionObject &colObj1 ) const;
	
	/** \brief Check if touch sensor can hit collision object. */
	bool CanTSensorHitCObj( const debpTouchSensor &touchSensor, const debpCollisionObject &colObj1 ) const;
	
	
	/** \brief Check if two colliders can collide. */
	bool CanColliderHitCollider( const debpCollisionObject &colObj0, const debpCollider &collider0,
		const debpCollisionObject &colObj1, const debpCollider &collider1 ) const;
	/*@}*/
};

#endif
