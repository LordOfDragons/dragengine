/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEGDVISITOR_H_
#define _IGDEGDVISITOR_H_

class igdeGDClass;
class igdeGDSkin;
class igdeGDSky;



/**
 * \brief Game Definition Visitor.
 */
class igdeGDVisitor{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	igdeGDVisitor();
	
	/** \brief Clean up visitor. */
	virtual ~igdeGDVisitor();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit object class. */
	virtual void VisitObjectClass( igdeGDClass *gdclass );
	
	/** \brief Visit skin. */
	virtual void VisitSkin( igdeGDSkin *gdskin );
	
	/** \brief Visit sky. */
	virtual void VisitSky( igdeGDSky *gdsky );
	/*@}*/
};

#endif
