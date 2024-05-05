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

#ifndef _MECOLLIDEROWNER_H_
#define _MECOLLIDEROWNER_H_

class meDecal;
class meNavigationSpace;
class meObject;
class meObjectShape;
class meObjectSnapPoint;

class igdeEnvironment;

class deCollider;



/**
 * \brief Collider owner.
 * 
 * Used as user pointer for colliders to identify the owner of a collider. This class is
 * immutable. Owners have to use the matching constructor to set the parameters.
 * 
 * All class members store only weak references.
 */
class meColliderOwner{
private:
	meObject *pObject;
	meObjectShape *pShape;
	meObjectSnapPoint *pSnapPoint;
	meDecal *pDecal;
	meNavigationSpace *pNavigationSpace;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create collider owner for object. */
	meColliderOwner( meObject *object );
	
	/** \brief Create collider owner for object shape. */
	meColliderOwner( meObjectShape *shape );
	
	/** \brief Create collider owner for object snap point. */
	meColliderOwner( meObjectSnapPoint *snapPoint );
	
	/** \brief Create collider owner for decal. */
	meColliderOwner( meDecal *decal );
	
	/** \brief Create collider owner for navigation space. */
	meColliderOwner( meNavigationSpace *navspace );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Owner object class or \em NULL if not set. */
	inline meObject *GetObject() const{ return pObject; }
	
	/** \brief Object shape or \em NULL if not set. */
	inline meObjectShape *GetShape() const{ return pShape; }
	
	/** \brief Snap point or \em NULL if not set. */
	inline meObjectSnapPoint *GetSnapPoint() const{ return pSnapPoint; }
	
	/** \brief Owner decal or \em NULL if not set. */
	inline meDecal *GetDecal() const{ return pDecal; }
	
	/** \brief Owner navigation space or \em NULL if not set. */
	inline meNavigationSpace *GetNavigationSpace() const{ return pNavigationSpace; }
	
	
	
	/** \brief Get collider owner for collider or \em NULL if not set. */
	static const meColliderOwner *GetColliderOwner(
		igdeEnvironment &environment, deCollider *collider );
	/*@}*/
};

#endif
