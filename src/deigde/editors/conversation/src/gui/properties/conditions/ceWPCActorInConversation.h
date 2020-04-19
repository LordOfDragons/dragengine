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

#ifndef _CEWPCACTORINCONVERSATION_H_
#define _CEWPCACTORINCONVERSATION_H_

#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>

#include "ceWPCondition.h"

class ceCConditionActorInConversation;



/**
 * \brief Conversation condition actor in conversation property panel.
 */
class ceWPCActorInConversation : public ceWPCondition{
private:
	igdeComboBoxReference pCBActor;
	igdeCheckBoxReference pChkNegate;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	ceWPCActorInConversation( ceWPTopic &parentPanel );
	
protected:
	/** \brief Clean up view property window. */
	virtual ~ceWPCActorInConversation();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Active condition. */
	ceCConditionActorInConversation *GetCondition() const;
	
	/** \brief Update active condition. */
	void UpdateCondition();
	
	/** \brief Update actor list. */
	void UpdateActorList();
	/*@}*/
};

#endif
