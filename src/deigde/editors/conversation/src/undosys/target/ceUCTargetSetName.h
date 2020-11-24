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

#ifndef _CEUCTARGETSETNAME_H_
#define _CEUCTARGETSETNAME_H_

#include "../action/ceUndoCActionList.h"
#include "../../conversation/camerashot/ceCameraShotList.h"

#include <deigde/undo/igdeUndo.h>

class ceTarget;
class ceConversationActionList;
class ceConversationTopic;



/**
 * \brief Undo Action Target Set Name.
 */
class ceUCTargetSetName : public igdeUndo{
private:
	ceTarget *pTarget;
	
	decString pOldName;
	decString pNewName;
	
	ceUndoCActionList pActionList;
	ceCameraShotList pCameraShotList;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	ceUCTargetSetName( ceTarget *target, const char *newName );
protected:
	/** \brief Clean up undo. */
	virtual ~ceUCTargetSetName();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo. */
	virtual void Undo();
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
	
private:
	void pSetName( const char *oldName, const char *newName );
	void pAddActions( ceConversationTopic *topic, const ceConversationActionList &list );
};

#endif
