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

#ifndef _CEWPACTION_H_
#define _CEWPACTION_H_

#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/layout/igdeContainerForm.h>

class igdeComboBox;

class ceWPTopic;
class ceConversationAction;



/**
 * \brief Conversation Action property window.
 */
class ceWPAction : public igdeContainerForm{
protected:
	ceWPTopic &pParentPanel;
	
	igdeCheckBoxReference pChkWaitForActor;
	igdeComboBoxReference pCBWaitForActorID;
	igdeTextFieldReference pEditDelay;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	ceWPAction( ceWPTopic &parentPanel );
	
protected:
	/** \brief Clean up panel. */
	virtual ~ceWPAction();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Parent panel. */
	inline ceWPTopic &GetParentPanel() const{ return pParentPanel; }
	
	/** \brief Create common action parameters. */
	void CreateGUICommon( igdeContainerForm &container );
	
	/** \brief Update common action parameters. */
	void UpdateCommonParams();
	
	/** \brief Update a combo box with the list of actor ids an alias ids. */
	void UpdateComboBoxWithActorIDList( igdeComboBox &combobox );
	
	/** \brief Update a combo box with the list of convo coord system ids an alias ids. */
	void UpdateComboBoxWithConvoCoordSysIDList( igdeComboBox &combobox );
	
	/** \brief Update actor id lists. */
	virtual void UpdateActorIDLists();
	
	/** \brief Conversation path changed. */
	virtual void OnConversationPathChanged();
	/*@}*/
};

#endif
