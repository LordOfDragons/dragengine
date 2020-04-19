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

#ifndef _CEWPAPLAYERCHOICE_H_
#define _CEWPAPLAYERCHOICE_H_

#include <deigde/gui/igdeButtonReference.h>

#include "ceWPAction.h"

class ceCAPlayerChoice;
class ceCAPlayerChoiceOption;



/**
 * \brief Player choice conversation action property view.
 */
class ceWPAPlayerChoice : public ceWPAction{
private:
	igdeTextFieldReference pEditVarName;
	igdeTextFieldReference pEditOptionText;
	igdeButtonReference pBtnOptionText;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create view. */
	ceWPAPlayerChoice( ceWPTopic &parentPanel );
	
protected:
	/** \brief Clean up view. */
	virtual ~ceWPAPlayerChoice();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Active action or \em NULL. */
	ceCAPlayerChoice *GetAction() const;
	
	/** \brief Active option or \em NULL. */
	ceCAPlayerChoiceOption *GetActiveOption() const;
	
	/** \brief Update action. */
	void UpdateAction();
	/*@}*/
};

#endif
