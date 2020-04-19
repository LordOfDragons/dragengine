/* 
 * Drag[en]gine IGDE Skin Editor
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

#ifndef _SEDIALOGADDPROPERTY_H_
#define _SEDIALOGADDPROPERTY_H_

#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/dialog/igdeDialog.h>

#include <dragengine/common/string/decStringSet.h>

class seWindowMain;



/**
 * \brief Add Property Dialog.
 */
class seDialogAddProperty : public igdeDialog{
private:
	seWindowMain &pWindowMain;
	igdeListBoxReference pListProperties;
	igdeTextFieldReference pEditCustomPropertyName;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	seDialogAddProperty( seWindowMain &windowMain );
	
protected:
	/** \brief Clean up dialog. */
	virtual ~seDialogAddProperty();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Selected property names. */
	decStringSet GetSelectedPropertyNames() const;
	
	/** \brief Set selected property names. */
	void SetSelectedPropertyNames( const decStringSet &list );
	
	/** \brief Custom property name. */
	const decString &GetCustomPropertyName() const;
	
	/** \brief Set custom property name. */
	void SetCustomPropertyName( const char *name );
	/*@}*/
	
	
	
public:
	void pInitPropertyList();
};

#endif
