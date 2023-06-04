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

#ifndef _IGDELISTITEM_H_
#define _IGDELISTITEM_H_

#include "../resources/igdeIconReference.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringList.h>



/**
 * \brief IGDE UI List Item.
 * 
 * Stores a text and optional icon representing a list item an widgets able to show lists.
 * Optionally a data pointer can be assigned.
 */
class DE_DLL_EXPORT igdeListItem : public deObject{
private:
	decString pText;
	decString pDescription;
	igdeIconReference pIcon;
	void *pData;
	decStringList pDetails;
	bool pSelected;
	
	
	
public:
	/** \text Constructors and Destructors */
	/*@{*/
	/** \brief Create list item. */
	igdeListItem( const char *text );
	
	igdeListItem( const char *text, igdeIcon *icon );
	
	igdeListItem( const char *text, igdeIcon *icon, const char *description );
	
	/** \brief Create list item. */
	igdeListItem( const char *text, void *data );
	
	igdeListItem( const char *text, igdeIcon *icon, void *data );
	
	igdeListItem( const char *text, igdeIcon *icon, const char *description, void *data );
	
	/** \brief Create copy of list item. */
	igdeListItem( const igdeListItem &listItem );
	
	
	
protected:
	/**
	 * \brief Clean up list item.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeListItem();
	/*@}*/
	
	
	
public:
	/** \text Management */
	/*@{*/
	/** \brief Text. */
	inline const decString &GetText() const{ return pText; }
	
	/** \brief Set text. */
	void SetText( const char *text );
	
	/** \brief Description shown in tool tips. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description shown in tool tips. */
	void SetDescription( const char *description );
	
	/** \brief Icon or NULL. */
	inline igdeIcon *GetIcon() const{ return pIcon; }
	
	/** \brief Set icon or NULL. */
	void SetIcon( igdeIcon *icon );
	
	/** \brief User data pointer. */
	inline void *GetData() const{ return pData; }
	
	/** \brief Set user data pointer. */
	void SetData( void *data );
	
	/**
	 * \brief Details used for igdeListBox only in detail mode.
	 * 
	 * Details are displayed for an item if the owning list box is in detail mode and
	 * the number of columns shown is larger than the detail index. Details exteeding
	 * the column count are ignored. If less details are present than the column count
	 * requires the missing details are considered empty strings.
	 */
	decStringList &GetDetails(){ return pDetails; }
	inline const decStringList &GetDetails() const{ return pDetails; }
	
	/** \brief Item is selected. */
	inline bool GetSelected() const{ return pSelected; }
	
	/** \brief Set if item is selected. */
	void SetSelected( bool selected );
	
	
	
	/** \brief Set from another item. */
	virtual igdeListItem &operator=( const igdeListItem &listItem );
	/*@}*/
};

#endif
