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

#ifndef _CEUCAASPEAKWORDSCALE_H_
#define _CEUCAASPEAKWORDSCALE_H_

#include <deigde/undo/igdeUndo.h>

class ceCAActorSpeak;
class ceConversationTopic;
class ceStrip;



/**
 * \brief Undo Action Actor Speak Conversation Action Set Word Duration.
 */
class ceUCAASpeakWordSetDuration : public igdeUndo{
private:
	ceConversationTopic *pTopic;
	ceCAActorSpeak *pActorSpeak;
	ceStrip *pWord;
	float pOldDuration;
	float pNewDuration;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	ceUCAASpeakWordSetDuration( ceConversationTopic *topic, ceCAActorSpeak *actorSpeak, ceStrip *word, float newDuration );
protected:
	/** \brief Clean up undo. */
	virtual ~ceUCAASpeakWordSetDuration();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Set new duration. */
	void SetNewDuration( float duration );
	
	/** \brief Undo. */
	virtual void Undo();
	/** \brief Redo. */
	virtual void Redo();
	/** \brief Progressive redo action. */
	void ProgressiveRedo();
	/*@}*/
};

#endif
