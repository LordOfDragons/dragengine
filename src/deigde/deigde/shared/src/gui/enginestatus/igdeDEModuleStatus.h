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

#ifndef _IGDEDEMODULESTATUS_H_
#define _IGDEDEMODULESTATUS_H_

#include "../igdeComboBoxReference.h"
#include "../igdeCheckBoxReference.h"
#include "../igdeTextAreaReference.h"
#include "../igdeTextFieldReference.h"
#include "../layout/igdeContainerFlow.h"


class igdeDialogEngine;


/**
 * \brief Panel showing the status of a module in the game engine.
 */
class igdeDEModuleStatus : public igdeContainerFlow{
	igdeDialogEngine &pDialogEngine;
	
	igdeComboBoxReference pCBModule;
	igdeTextFieldReference pEditType;
	igdeTextAreaReference pEditDescription;
	igdeTextFieldReference pEditAuthor;
	igdeTextFieldReference pEditVersion;
	igdeTextFieldReference pEditPattern;
	igdeTextFieldReference pEditDefaultExtension;
	igdeTextFieldReference pEditDirName;
	igdeCheckBoxReference pChkFallback;
	igdeTextAreaReference pEditStatus;
	
	igdeTextFieldReference pEditLibName;
	igdeTextFieldReference pEditLibSize;
	igdeTextFieldReference pEditLibHash;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	igdeDEModuleStatus( igdeDialogEngine &dialogEngine );
	
	
	
protected:
	/** \brief Cleans up panel. */
	virtual ~igdeDEModuleStatus();
	
	
	
public:
	/** \name Management */
	/*@{*/
	void UpdateModuleStatus();
	void UpdateModulesList();
};

#endif
