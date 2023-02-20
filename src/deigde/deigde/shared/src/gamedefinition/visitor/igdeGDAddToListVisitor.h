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

#ifndef _IGDEGDADDTOLISTVISITOR_H_
#define _IGDEGDADDTOLISTVISITOR_H_

#include "igdeGDVisitor.h"
#include "../../gui/igdeIconListBoxReference.h"


class deImage;
class igdeEnvironment;
class igdeListItemReference;



/**
 * \brief Visitor adding the visited game definition elements to a list.
 */
class DE_DLL_EXPORT igdeGDAddToListVisitor : public igdeGDVisitor{
private:
	igdeEnvironment &pEnvironment;
	igdeIconListBoxReference pIconListBox;
	int pSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	igdeGDAddToListVisitor( igdeEnvironment &environment, igdeIconListBox *iconListBox, int size );
	
	/** \brief Clean up visitor. */
	virtual ~igdeGDAddToListVisitor();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Add item to list. */
	void AddItemToList( const char *caption, deImage &image, void *userPointer );
	
	/** \brief Add item to list. */
	void AddItemToList( igdeListItemReference &item, const char *caption, deImage &image, void *userPointer );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit object class. */
	virtual void VisitObjectClass( igdeGDClass *gdclass );
	
	/** \brief Visit skin. */
	virtual void VisitSkin( igdeGDSkin *gdskin );
	
	/** \brief Visits sky. */
	virtual void VisitSky( igdeGDSky *gdsky );
	/*@}*/
};

#endif
