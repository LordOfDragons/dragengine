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

#ifndef _IGDEDIALOGMULTILINEVALUE_H_
#define _IGDEDIALOGMULTILINEVALUE_H_

#include "igdeDialog.h"
#include "../igdeTextAreaReference.h"



/**
 * \brief IGDE dialog supporting input of multiline values.
 */
class DE_DLL_EXPORT igdeDialogMultilineValue : public igdeDialog{
private:
	igdeTextAreaReference pEditValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	igdeDialogMultilineValue( igdeEnvironment &environment, const char *title, const char *message,
		int rows = 8, int columns = 50 );
	
	
	
protected:
	/** \brief Clean up dialog. */
	virtual ~igdeDialogMultilineValue();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Get value. */
	const decString &GetValue() const;
	
	/** \brief Set value. */
	void SetValue( const char *value );
	/*@}*/
};

#endif
