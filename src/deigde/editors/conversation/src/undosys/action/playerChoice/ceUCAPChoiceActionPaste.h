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

#ifndef _CEUCAPCHOICEACTIONPASTE_H_
#define _CEUCAPCHOICEACTIONPASTE_H_

#include "../ceUCActionPaste.h"

class ceCAPlayerChoice;
class ceCAPlayerChoiceOption;



/**
 * \brief Undo Action Player Choice Add Conversation Action.
 */
class ceUCAPChoiceActionPaste : public ceUCActionPaste{
private:
	ceCAPlayerChoice::Ref pPlayerChoice;
	ceCAPlayerChoiceOption::Ref pOption;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	ceUCAPChoiceActionPaste(ceConversationTopic *topic, ceCAPlayerChoice *playerChoice,
		ceCAPlayerChoiceOption *option, const ceConversationActionList &actions, int index);
protected:
	/** \brief Clean up undo. */
	~ceUCAPChoiceActionPaste() override;
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo. */
	void Undo() override;
	/** \brief Redo. */
	void Redo() override;
	/*@}*/
};

#endif
