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

#ifndef _CECLIPBOARDDATATOPIC_H_
#define _CECLIPBOARDDATATOPIC_H_

#include "../conversation/topic/ceConversationTopicList.h"

#include <deigde/clipboard/igdeClipboardData.h>



/**
 * \brief Topic clipboard data.
 */
class ceClipboardDataTopic : public igdeClipboardData{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<ceClipboardDataTopic> Ref;
	
	/** \brief Type name. */
	static const char * const TYPE_NAME;
	
	
	
private:
	ceConversationTopicList pTopics;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create clipboard data. */
	ceClipboardDataTopic( const ceConversationTopicList &topics );
	
protected:
	/**
	 * \brief Clean up object.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~ceClipboardDataTopic();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Topics. */
	inline const ceConversationTopicList &GetTopics() const{ return pTopics; }
	/*@}*/
};

#endif
