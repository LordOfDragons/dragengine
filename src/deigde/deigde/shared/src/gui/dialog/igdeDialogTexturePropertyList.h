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

#ifndef _IGDEDIALOGTEXTUREPROPERTYLIST_H
#define _IGDEDIALOGTEXTUREPROPERTYLIST_H

#include "igdeDialog.h"
#include "../igdeListBoxReference.h"
#include "../igdeTextFieldReference.h"
#include "../igdeTextAreaReference.h"


class igdeTexturePropertyList;


/**
 * \brief Texture Property List Dialog.
 * 
 * Dialog showing content of texture property list.
 */
class DE_DLL_EXPORT igdeDialogTexturePropertyList : public igdeDialog{
private:
	igdeListBoxReference pListProperties;
	igdeTextFieldReference pEditName;
	igdeTextAreaReference pEditDescription;
	igdeTextFieldReference pEditType;
	igdeTextFieldReference pEditComponentCount;
	igdeTextFieldReference pEditDefault;
	igdeTextFieldReference pEditAffectedModules;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	igdeDialogTexturePropertyList( igdeEnvironment &environment );
	
	
	
protected:
	/** \brief Clean up dialog. */
	virtual ~igdeDialogTexturePropertyList();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Update list of properties. */
	void UpdatePropertyList();
	
	/** \brief Update selected property. */
	void UpdateProperty();
	/*@}*/
};

#endif
