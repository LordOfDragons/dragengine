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

#ifndef _CEWPCONDITION_H_
#define _CEWPCONDITION_H_

#include <deigde/gui/layout/igdeContainerForm.h>

class ceWPTopic;
class ceConversationCondition;
class igdeComboBox;


/**
 * \brief Conversation condition property window.
 */
class ceWPCondition : public igdeContainerForm{
protected:
	ceWPTopic &pParentPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	ceWPCondition( ceWPTopic &parentPanel );
	
protected:
	/** \brief Clean up property window. */
	virtual ~ceWPCondition();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Parent Panel. */
	inline ceWPTopic &GetParentPanel() const{ return pParentPanel; }
	
	/** \brief Update combo box with the list of actor ids an alias ids. */
	void UpdateComboBoxWithActorIDList( igdeComboBox &combobox );
	/*@}*/
};

#endif
