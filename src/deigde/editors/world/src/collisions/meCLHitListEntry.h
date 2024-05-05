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

#ifndef _MECLHITLISTENTRY_H_
#define _MECLHITLISTENTRY_H_

#include <dragengine/common/math/decMath.h>

class meObject;
class meObjectShape;
class meDecal;
class meNavigationSpace;



/**
 * \brief Hit list entry.
 */
class meCLHitListEntry{
private:
	meObject *pObject;
	meObjectShape *pObjectShape;
	meDecal *pDecal;
	meNavigationSpace *pNavSpace;
	int pHTNavSpacePoint;
	
	float pDistance;
	decVector pNormal;
	
	
	
public:
	/** \name Constructors, Destructors */
	/*@{*/
	/** \brief Create hit list entry. */
	meCLHitListEntry();
	
	/** \brief Clean up hit list entry. */
	~meCLHitListEntry();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Clear entry. */
	void Clear();
	
	/** \brief Object or \em NULL. */
	inline meObject *GetObject() const{ return pObject; }
	
	/** \brief Set object or \em NULL. */
	void SetObject( meObject *object );
	
	/** \brief Object shape or \em NULL. */
	inline meObjectShape *GetObjectShape() const{ return pObjectShape; }
	
	/** \brief Set object shape or \em NULL. */
	void SetObjectShape( meObjectShape *objectShape );
	
	/** \brief Decal or \em NULL. */
	inline meDecal *GetDecal() const{ return pDecal; }
	
	/** \brief Set decal or \em NULL. */
	void SetDecal( meDecal *decal );
	
	/** \brief Navigation space or \em NULL. */
	inline meNavigationSpace *GetNavigationSpace() const{ return pNavSpace; }
	
	/** \brief Set navigation space or \em NULL. */
	void SetNavigationSpace( meNavigationSpace *navspac3 );
	
	/** \brief Height terrain navigation space point or -1. */
	inline int GetHTNavSpacePoint() const{ return pHTNavSpacePoint; }
	
	/** \brief Set height terrain navigation space point or -1. */
	void SetHTNavSpacePoint( int point );
	
	
	
	/** \brief Distance. */
	inline float GetDistance() const{ return pDistance; }
	
	/** \brief Set distance. */
	void SetDistance( float distance );
	
	/** \brief Normal. */
	inline const decVector &GetNormal() const{ return pNormal; }
	
	/** \brief Set normal. */
	void SetNormal( const decVector &normal );
	
	
	
	/** \brief Sort decals. */
	void SortDecals();
	
	/**
	 * \brief Compare entry to another one.
	 * \returns 1 if entry is ordinally larger, -1 if smaller or 0 if equal.
	 */
	int CompareTo( const meCLHitListEntry &entry ) const;
	/*@}*/
};

#endif
