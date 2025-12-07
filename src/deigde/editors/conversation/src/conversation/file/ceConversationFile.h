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

#ifndef _CECONVERSATIONFILE_H_
#define _CECONVERSATIONFILE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

#include "../topic/ceConversationTopicList.h"

class ceConversation;



/**
 * \brief Conversation File.
 */
class ceConversationFile : public deObject{
private:
	ceConversation *pConversation;
	
	decString pID;
	
	ceConversationTopicList pTopics;
	ceConversationTopic *pActiveTopic;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<ceConversationFile> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create conversation file. */
	ceConversationFile(const char *id = "Group");
	
	/** \brief Create copy of conversation file. */
	ceConversationFile(const ceConversationFile &file);
	
	/** \brief Clean up conversation file. */
	virtual ~ceConversationFile();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent conversation or \em NULL if not set. */
	inline ceConversation *GetConversation() const{return pConversation;}
	
	/** \brief Set parent conversation or \em NULL if not set. */
	void SetConversation(ceConversation *conversation);
	
	/** \brief Identifier. */
	inline const decString &GetID() const{return pID;}
	
	/** \brief Set identifier. */
	void SetID(const char *id);
	/*@}*/
	
	
	
	/** \name Topics */
	/*@{*/
	/** \brief Topics. */
	inline const ceConversationTopicList &GetTopicList() const{return pTopics;}
	
	/** \brief Add topic. */
	void AddTopic(ceConversationTopic *topic);
	
	/** \brief Remove topic. */
	void RemoveTopic(ceConversationTopic *topic);
	
	/** \brief Remove all topics. */
	void RemoveAllTopics();
	
	/** \brief Active topic or \em NULL if none is active. */
	inline ceConversationTopic *GetActiveTopic() const{return pActiveTopic;}
	
	/** \brief Set active topic or \em NULL if none is active. */
	void SetActiveTopic(ceConversationTopic *topic);
	/*@}*/
};

#endif
