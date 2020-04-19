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

#ifndef _IGDEDECONSOLE_H_
#define _IGDEDECONSOLE_H_

#include "../igdeComboBoxReference.h"
#include "../igdeTextFieldReference.h"
#include "../igdeTextAreaReference.h"
#include "../layout/igdeContainerFlow.h"

#include <dragengine/common/string/unicode/decUnicodeLineBuffer.h>


class igdeDialogEngine;


/**
 * \brief Panel allowing to send commands directly to the module using the module console ability of the engine.
 */
class igdeDEConsole : public igdeContainerFlow{
private:
	igdeDialogEngine &pDialogEngine;
	decUnicodeLineBuffer pLog;
	
	igdeComboBoxReference pCBModule;
	igdeTextFieldReference pEditCommand;
	igdeTextAreaReference pEditLogs;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	igdeDEConsole( igdeDialogEngine &dialogEngine );
	
	
	
protected:
	/** \brief Cleans up panel. */
	virtual ~igdeDEConsole();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	void UpdateModulesList();
	void AddToConsole( decUnicodeString &text );
	void ClearConsole();
	
	/** \brief Send command. */
	void SendCommand();
	/*@{*/
};

#endif
