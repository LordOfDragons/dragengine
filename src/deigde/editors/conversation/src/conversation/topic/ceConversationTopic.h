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

#ifndef _CECONVERSATIONTOPIC_H_
#define _CECONVERSATIONTOPIC_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

#include "../action/ceConversationActionList.h"
#include "../action/ceConversationActionReference.h"
#include "../condition/ceConversationConditionReference.h"

class ceConversationFile;
class ceConversationCondition;


/**
 * \brief Conversation topic.
 */
class ceConversationTopic : public deObject{
public:
	typedef deTObjectReference<ceConversationTopic> Ref;
	
	
	
private:
	ceConversationFile *pFile;
	
	decString pID;
	ceConversationActionList pActions;
	ceConversationActionReference pActiveAction;
	ceConversationConditionReference pActiveCondition;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create conversation topic. */
	ceConversationTopic( const char *id = "Topic" );
	
	/** \brief Create copy of conversation topic. */
	ceConversationTopic( const ceConversationTopic &topic );
	
	/** \brief Clean up conversation topic. */
	virtual ~ceConversationTopic();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent file or \em NULL if not set. */
	inline ceConversationFile *GetFile() const{ return pFile; }
	
	/** \brief Set parent file or \em NULL if not set. */
	void SetFile( ceConversationFile *file );
	
	
	
	/** \brief Identifier. */
	inline const decString &GetID() const{ return pID; }
	
	/** \brief Set identifier. */
	void SetID( const char *id );
	
	/** \brief Actions. */
	inline ceConversationActionList &GetActionList(){ return pActions; }
	inline const ceConversationActionList &GetActionList() const{ return pActions; }
	
	/** \brief Active action or \em NULL if none is active. */
	inline ceConversationAction *GetActiveAction() const{ return pActiveAction; }
	
	/** \brief Active condition or \em NULL if none is active. */
	inline ceConversationCondition *GetActiveCondition() const{ return pActiveCondition; }
	
	/** \brief Set active element. */
	void SetActive( ceConversationAction *action, ceConversationCondition *condition );
	
	
	
	/** \brief Notify listeners action structure changed. */
	void NotifyActionStructureChanged( ceConversationAction *action );
	
	/** \brief Notify listeners action changed. */
	void NotifyActionChanged( ceConversationAction *action );
	
	/** \brief Notify listeners condition structure changed. */
	void NotifyConditionStructureChanged( ceConversationAction *action );
	
	/** \brief Notify listeners condition changed. */
	void NotifyConditionChanged( ceConversationAction *action, ceConversationCondition *condition );
	/*@}*/
};

#endif
