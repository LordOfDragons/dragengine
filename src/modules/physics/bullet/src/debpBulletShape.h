/* 
 * Drag[en]gine Bullet Physics Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DEBPBULLETSHAPE_H_
#define _DEBPBULLETSHAPE_H_

#include <dragengine/deObject.h>

class btCollisionShape;



/**
 * \brief Wraps a bullet collision shape.
 * 
 * Bullet does not manage the lifetime of create collision shapes. This is
 * done to allow using collision shapes on multiple collision objects. To
 * avoid memory leaking the created collision shapes have to be wrapped
 * into debpBulletShape which is reference counted to properly release the
 * bullet shapes once nobody uses them anymore.
 */
class debpBulletShape : public deObject {
private:
	btCollisionShape *pShape;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create bullet shape wrapper taking ownership of bullet shape. */
	debpBulletShape( btCollisionShape *shape );
	
	/** \brief Clean up bullet shape wrapper deleting wrapped bullet shape. */
	virtual ~debpBulletShape();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Bullet shape. */
	inline btCollisionShape *GetShape() const{ return pShape; }
	/*@}*/
};

#endif
