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

#ifndef _IGDENATIVEBEOSCOMBOBOXFILTER_H_
#define _IGDENATIVEBEOSCOMBOBOXFILTER_H_

#include "igdeNativeBeOSComboBox.h"

class igdeComboBoxFilter;


/**
 * FOX Native filter combo box.
 */
class igdeNativeBeOSComboBoxFilter : public igdeNativeBeOSComboBox{
public:
	enum eEventsFilter{
		eeFilterChanged = 'fchg'
	};
	
protected:
// 	FXLabel *pLabelFilter;
// 	FXTextField *pEditFilter;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeBeOSComboBoxFilter( igdeComboBoxFilter &owner, const igdeGuiTheme &guitheme );
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeBeOSComboBoxFilter();
	
	/** \brief Create native widget. */
	static igdeNativeBeOSComboBoxFilter* CreateNativeWidget( igdeComboBoxFilter &owner );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual void UpdateFilterString();
	virtual void UpdateRowCount();
	
	virtual void MessageReceived( BMessage *message );
	/*@}*/
};

typedef igdeNativeBeOSComboBoxFilter igdeNativeComboBoxFilter;

#endif
