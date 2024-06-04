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

#ifndef _CEUCTOPICSETID_H_
#define _CEUCTOPICSETID_H_

#include "../action/ceUndoCActionList.h"

#include <deigde/undo/igdeUndo.h>

class ceConversationTopic;
class ceConversation;
class ceConversationActionList;



/**
 * \brief Undo action topic set id.
 */
class ceUCTopicSetID : public igdeUndo{
private:
	ceConversationTopic *pTopic;
	
	decString pOldID;
	decString pNewID;
	
	ceUndoCActionList pSnippets;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo action. */
	ceUCTopicSetID( const ceConversation &conversation,
		ceConversationTopic *topic, const char *newID );
	
	/** \brief Clean up undo action. */
	virtual ~ceUCTopicSetID();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo action. */
	virtual void Undo();
	
	/** \brief Redo action. */
	virtual void Redo();
	/*@}*/
	
	
	
private:
	void pSetID( const char *id );
	
	void pAddSnippets( ceConversationTopic *topic, const char *matchGroupID,
		const char *matchTopicID, const ceConversationActionList &actions );
};

#endif
