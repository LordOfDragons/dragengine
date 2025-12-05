/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _CECONVERSATIONTOPIC_H_
#define _CECONVERSATIONTOPIC_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

#include "../action/ceConversationActionList.h"
#include "../action/ceConversationAction.h"
#include "../condition/ceConversationCondition.h"

class decStringSet;
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
	ceConversationAction::Ref pActiveAction;
	ceConversationCondition::Ref pActiveCondition;
	
	
	
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
	
	/** Find missing words. */
	void FindMissingWords( decStringSet &missingWords ) const;
	void FindMissingWords( const ceConversationActionList &actions, decStringSet &missingWords ) const;
	
	
	
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
