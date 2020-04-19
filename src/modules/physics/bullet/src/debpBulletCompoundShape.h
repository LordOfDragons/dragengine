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

#ifndef _DEBPBULLETCOMPOUNDSHAPE_H_
#define _DEBPBULLETCOMPOUNDSHAPE_H_

#include "debpBulletShape.h"

#include <dragengine/common/collection/decObjectList.h>

class btCompoundShape;



/**
 * \brief Wraps a bullet compound collision shape.
 * 
 * All child shapes are also stored in addition to the root shape.
 */
class debpBulletCompoundShape : public debpBulletShape {
private:
	decObjectList pChildShapes;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create bullet shape wrapper taking ownership of bullet shape. */
	debpBulletCompoundShape( btCompoundShape *shape );
	
	/** \brief Clean up bullet shape wrapper deleting wrapped bullet shape. */
	virtual ~debpBulletCompoundShape();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Bullet shape. */
	inline btCompoundShape *GetCompoundShape() const{ return ( btCompoundShape* )GetShape(); }
	
	/** \brief Add child shape to clean up once the parent shape is finished. */
	void AddChildShape( debpBulletShape *shape );
	/*@}*/
};

#endif
