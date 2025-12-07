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

#ifndef _CECACOMMENT_H_
#define _CECACOMMENT_H_

#include "ceConversationAction.h"

#include <dragengine/common/string/decString.h>



/**
 * \brief Comment conversation action ignored during game play.
 */
class ceCAComment : public ceConversationAction{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<ceCAComment> Ref;
	
	
private:
	decString pComment;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new conversation action. */
	ceCAComment();
	/** \brief Creates a copy of a conversation action. */
	ceCAComment(const ceCAComment &action);
	/** \brief Cleans up the conversation action. */
	~ceCAComment() override;
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the comment. */
	inline const decString &GetComment() const{ return pComment; }
	/** \brief Sets the comment. */
	void SetComment(const char *comment);
	
	/** \brief Create a copy of this action. */
    ceConversationAction *CreateCopy() const override;
	/*@}*/
};

#endif
