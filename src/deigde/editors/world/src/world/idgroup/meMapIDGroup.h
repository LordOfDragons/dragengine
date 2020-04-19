/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MEMAPIDGROUP_H_
#define _MEMAPIDGROUP_H_

#include <dragengine/deObject.h>

class meIDGroup;
class igdeGDProperty;



/**
 * \brief Mapping between game definition property and Identifier group.
 */
class meMapIDGroup : public deObject{
private:
	igdeGDProperty *pProperty;
	meIDGroup *pGroup;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create mapping between game definition property and Identifier group. */
	meMapIDGroup( igdeGDProperty *property, meIDGroup *group );
	
	/** \brief Clean up mapping between game definition property and Identifier group. */
	virtual ~meMapIDGroup();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Game definition property. */
	inline igdeGDProperty *GetProperty() const{ return pProperty; }
	
	/** \brief Identifier group. */
	inline meIDGroup *GetGroup() const{ return pGroup; }
	/*@}*/
};

#endif
