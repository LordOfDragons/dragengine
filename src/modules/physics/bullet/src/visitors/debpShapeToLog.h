/* 
 * Drag[en]gine Bullet Physics Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEBPSHAPETOLOG_H_
#define _DEBPSHAPETOLOG_H_

#include <dragengine/common/shape/decShapeVisitor.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>

class debpDCollisionVolume;
class debpDCollisionSphere;
class debpDCollisionBox;
class debpDCollisionCylinder;
class debpDCollisionCapsule;


/** Shape to log string visitor. */
class debpShapeToLog : public decShapeVisitor{
private:
	decString pLog;
	
	
	
public:
	/** \name Constructor, Destructor */
	/*@{*/
	/** \brief Create visitor. */
	debpShapeToLog();
	
	/** \brief Clean up visitor. */
	virtual ~debpShapeToLog() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	inline const decString &GetLog() const{ return pLog; }
	
	void Reset();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit sphere shape. */
	virtual void VisitShapeSphere( decShapeSphere &sphere ) override;
	
	/** \brief Visit box shape. */
	virtual void VisitShapeBox( decShapeBox &box ) override;
	
	/** \brief Visit cylinder shape. */
	virtual void VisitShapeCylinder( decShapeCylinder &cylinder ) override;
	
	/** \brief Visit capsule shape. */
	virtual void VisitShapeCapsule( decShapeCapsule &capsule ) override;
	
	/** \brief Visit hull shape. */
	virtual void VisitShapeHull( decShapeHull &hull ) override;
	/*@}*/
};

#endif
