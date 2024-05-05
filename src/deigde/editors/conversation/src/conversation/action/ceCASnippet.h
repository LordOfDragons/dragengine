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

#ifndef _CECASNIPPET_H_
#define _CECASNIPPET_H_

#include "ceConversationAction.h"

#include <dragengine/common/string/decString.h>



/**
 * \brief Snippet Conversation Action.
 * Process a list of actions from another topic.
 */
class ceCASnippet : public ceConversationAction{
private:
	decString pFile;
	decString pTopic;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new snippet conversation action. */
	ceCASnippet();
	/** Creates a new snippet conversation action. */
	ceCASnippet( const ceCASnippet &snippet );
	/** Cleans up the snippet conversation action. */
	virtual ~ceCASnippet();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the file to look for. */
	inline const decString &GetFile() const{ return pFile; }
	/** Sets the file to look for. */
	void SetFile( const char *file );
	/** Retrieves the topic to look for. */
	inline const decString &GetTopic() const{ return pTopic; }
	/** Sets the topic to look for. */
	void SetTopic( const char *topic );
	
	/** Create a copy of this action. */
    virtual ceConversationAction *CreateCopy() const;
	/*@}*/
};

#endif
