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

#ifndef _CEWPCTRIGGER_H_
#define _CEWPCTRIGGER_H_

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeTextFieldReference.h>

#include "ceWPCondition.h"

class ceCConditionTrigger;
class ceWPTopic;



/**
 * \brief Trigger conversation condition property window.
 */
class ceWPCTrigger : public ceWPCondition{
private:
	igdeComboBoxReference pCBTestMode;
	igdeTextFieldReference pEditTrigger;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	ceWPCTrigger( ceWPTopic &parentPanel );
	
protected:
	/** \brief Clean up panel. */
	virtual ~ceWPCTrigger();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Active condition. */
	ceCConditionTrigger *GetCondition() const;
	
	/** \brief Update active condition. */
	void UpdateCondition();
	/*@}*/
};

#endif
