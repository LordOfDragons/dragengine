/* 
 * Drag[en]gine Rig Module
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

#ifndef _DERMWRITESHAPE_H_
#define _DERMWRITESHAPE_H_

#include <dragengine/common/shape/decShapeVisitor.h>
#include <dragengine/common/string/decString.h>

class decXmlWriter;



/**
 * \brief Write shape visitor.
 * 
 * Writes the visited shape to a file using a variable indent. A line buffer
 * is provided to avoid bloating the module in size with local buffers.
 * The line buffer is guaranteed to be long enough.
 */
class dermWriteShape : public decShapeVisitor{
private:
	decXmlWriter &pWriter;
	decString pProperty;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	dermWriteShape( decXmlWriter &writer );
	
	/** \brief Clean up visitor. */
	virtual ~dermWriteShape();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Set property for string. */
	void SetProperty( const char *property );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit shape. */
	virtual void VisitShape( decShape &shape );
	
	/** \brief Visit sphere shape. */
	virtual void VisitShapeSphere( decShapeSphere &sphere );
	
	/** \brief Visit box shape. */
	virtual void VisitShapeBox( decShapeBox &box );
	
	/** \brief Visit cylinder shape. */
	virtual void VisitShapeCylinder( decShapeCylinder &cylinder );
	
	/** \brief Visit capsule shape. */
	virtual void VisitShapeCapsule( decShapeCapsule &capsule );
	
	/** \brief Visit hull shape. */
	virtual void VisitShapeHull( decShapeHull &hull );
	/*@}*/
};

#endif
