/* 
 * Drag[en]gine IGDE Conversation Editor
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

#ifndef _CEWPCACTORCOMMAND_H_
#define _CEWPCACTORCOMMAND_H_

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeTextFieldReference.h>

#include "ceWPCondition.h"

class ceCConditionActorCommand;



/**
 * \brief Actor command conversation condition property window.
 */
class ceWPCActorCommand : public ceWPCondition{
private:
	igdeComboBoxReference pCBActorID;
	igdeTextFieldReference pEditCommand;
	igdeButtonReference pBtnCommand;
	igdeCheckBoxReference pChkNegate;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	ceWPCActorCommand( ceWPTopic &parentPanel );
	
protected:
	/** \brief Clean up panel. */
	virtual ~ceWPCActorCommand();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Active condition or NULL if none is active. */
	ceCConditionActorCommand *GetCondition() const;
	
	/** \brief Update active condition. */
	void UpdateCondition();
	
	/** \brief Update actor id list. */
	void UpdateActorIDList();
	/*@}*/
};

#endif
