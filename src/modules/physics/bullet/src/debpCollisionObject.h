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

#ifndef _DEBPCOLLISIONBODY_H_
#define _DEBPCOLLISIONBODY_H_

struct btCollisionObjectWrapper;
class btManifoldPoint;
class debpTouchSensor;
class debpCollider;
class debpHTSector;



/**
 * \brief Base class for bullet collision object used to identify collision objects.
 */
class debpCollisionObject{
private:
	debpCollider *pOwnerCollider;
	int pOwnerBone;
	debpHTSector *pOwnerHTSector;
	debpTouchSensor *pOwnerTouchSensor;
	bool pDirtyAABB;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create collision object. */
	debpCollisionObject();
	
	/** \brief Clean up collision object. */
	virtual ~debpCollisionObject();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Owner collider or \em NULL if not owned by a collider. */
	inline debpCollider *GetOwnerCollider() const{return pOwnerCollider;}
	
	/** \brief Owner bone or -1 if not owned by a bone. */
	inline int GetOwnerBone() const{return pOwnerBone;}
	
	/** \brief Set owner collider. */
	void SetOwnerCollider(debpCollider *collider, int bone);
	
	/** \brief Owner height terrain sector or \em NULL if not owned by a height terrain sector. */
	inline debpHTSector *GetOwnerHTSector() const{return pOwnerHTSector;}
	
	/** \brief Set owner height terrain sector or \em NULL if not owned by a height terrain sector. */
	void SetOwnerHTSector(debpHTSector *htsector);
	
	/** \brief Owner touch sensor or \em NULL if not owned by a touch sensor. */
	inline debpTouchSensor *GetOwnerTouchSensor() const{return pOwnerTouchSensor;}
	
	/** \brief Set owner touch sensor or \em NULL if not owned by a touch sensor. */
	void SetOwnerTouchSensor(debpTouchSensor *touchSensorShape);
	
	
	
	/** \brief Owner is a collider. */
	bool IsOwnerCollider() const;
	
	/** \brief Owner is a height terrain sector. */
	bool IsOwnerHTSector() const;
	
	/** \brief Owner is a touch sensor shape. */
	bool IsOwnerTouchSensor() const;
	
	
	
	/**
	 * \brief AABB is dirty.
	 * \details Used by debpCollisionWorld for optimizing inter-frame updates.
	 */
	inline bool GetDirtyAABB() const{return pDirtyAABB;}
	
	/**
	 * \brief Set if AABB is dirty.
	 * \details Used by debpCollisionWorld for optimizing inter-frame updates.
	 */
	void SetDirtyAABB(bool dirty);
	
	
	
	/**
	 * \brief Calculate friction and restitution between two collision objects.
	 * \details Special static method required until bullet provides a proper virtual implementation.
	 */
	static bool CallbackAddContact(btManifoldPoint &cp, const btCollisionObjectWrapper *colObj0Wrap,
	int partId0, int index0, const btCollisionObjectWrapper *colObj1Wrap, int partId1, int index1);
	
	/**
	 * \brief Fix shape and triangle parameters.
	 * 
	 * Special static method required until bullet provides a proper virtual implementation.
	 * 
	 * Bullet stores shape and triangle parameters in the manifold point using partId0, partId1,
	 * index0 and index1. These values are though not correct for our use. Adjust the parameters
	 * so we can use them properly.
	 */
	static void CallbackAddContactFixParameters(btManifoldPoint &cp,
	const btCollisionObjectWrapper &colObj0Wrap, const btCollisionObjectWrapper &colObj1Wrap);
	/*@}*/
};

#endif
