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

#ifndef _CEUCAIFELSECASEADD_H_
#define _CEUCAIFELSECASEADD_H_

#include <deigde/undo/igdeUndo.h>
#include "ceCAIfElse.h"
#include "ceCAIfElseCase.h"
#include "ceConversationTopic.h"




/**
 * \brief Undo action if-else add case.
 */
class ceUCAIfElseCaseAdd : public igdeUndo{
private:
	ceConversationTopic::Ref pTopic;
	ceCAIfElse::Ref pIfElse;
	ceCAIfElseCase::Ref pCase;
	int pIndex;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<ceUCAIfElseCaseAdd> Ref;
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo object. */
	ceUCAIfElseCaseAdd(ceConversationTopic *topic, ceCAIfElse *ifelse, ceCAIfElseCase *ifcase, int index);
	
	/** \brief Clean up undo object. */
	virtual ~ceUCAIfElseCaseAdd();
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
