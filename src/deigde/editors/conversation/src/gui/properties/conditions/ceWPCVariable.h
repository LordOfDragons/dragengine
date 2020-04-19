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

#ifndef _CEWPCVARIABLE_H_
#define _CEWPCVARIABLE_H_

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeTextFieldReference.h>

#include "ceWPCondition.h"

class ceCConditionVariable;
class ceWPTopic;



/**
 * \brief Variable Conversation Condition property window.
 */
class ceWPCVariable : public ceWPCondition{
private:
	igdeComboBoxReference pCBOperator;
	igdeTextFieldReference pEditVariable;
	igdeTextFieldReference pEditTestValue;
	igdeTextFieldReference pEditTestVariable;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	ceWPCVariable( ceWPTopic &parentPanel );
	
protected:
	/** \brief Clean up panel. */
	virtual ~ceWPCVariable();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Active condition. */
	ceCConditionVariable *GetCondition() const;
	
	/** \brief Update active condition. */
	void UpdateCondition();
	/*@}*/
};

#endif
