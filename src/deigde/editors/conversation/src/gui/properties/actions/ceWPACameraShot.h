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

#ifndef _CEWPACAMERASHOT_H_
#define _CEWPACAMERASHOT_H_

#include <deigde/gui/igdeComboBoxFilterReference.h>

#include "ceWPAction.h"

class ceCACameraShot;



/**
 * \brief Camera Shot Conversation Action property window.
 */
class ceWPACameraShot : public ceWPAction{
private:
	igdeComboBoxFilterReference pCBName;
	igdeTextFieldReference pEditDuration;
	igdeComboBoxFilterReference pCBCameraTarget;
	igdeComboBoxFilterReference pCBLookAtTarget;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	ceWPACameraShot( ceWPTopic &parentPanel );
	
protected:
	/** \brief Clean up panel. */
	virtual ~ceWPACameraShot();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Active action. */
	ceCACameraShot *GetAction() const;
	
	/** \brief Update active action. */
	void UpdateAction();
	
	/** Update the list of camera shots. */
	void UpdateCameraShotList();
	
	/** Update target list. */
	void UpdateTargetList();
	/*@}*/
};

#endif
