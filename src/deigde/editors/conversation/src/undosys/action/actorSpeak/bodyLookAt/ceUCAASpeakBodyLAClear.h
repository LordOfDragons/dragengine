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

#ifndef _CEUCAASPEAKBODYLACLEAR_H_
#define _CEUCAASPEAKBODYLACLEAR_H_

#include "../../../../conversation/strip/ceStripList.h"

#include <deigde/undo/igdeUndo.h>

class ceCAActorSpeak;
class ceConversationTopic;



/**
 * \brief Undo action actor speak conversation action clear body look-at.
 */
class ceUCAASpeakBodyLAClear : public igdeUndo{
private:
	ceConversationTopic *pTopic;
	ceCAActorSpeak *pActorSpeak;
	ceStripList pOldStrips;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo action. */
	ceUCAASpeakBodyLAClear( ceConversationTopic *topic, ceCAActorSpeak *actorSpeak );
	
	/** \brief Clean up undo action. */
	virtual ~ceUCAASpeakBodyLAClear();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo action. */
	virtual void Undo();
	
	/** \brief Redo action. */
	virtual void Redo();
	/*@}*/
};

#endif
