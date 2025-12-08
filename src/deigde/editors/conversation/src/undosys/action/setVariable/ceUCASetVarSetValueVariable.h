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

#ifndef _CEUCASETVARSETVALUEVARIABLE_H_
#define _CEUCASETVARSETVALUEVARIABLE_H_

#include <deigde/undo/igdeUndo.h>

class ceCASetVariable;
class ceConversationTopic;



/**
 * \brief Undo action set variable conversation action set value variable.
 */
class ceUCASetVarSetValueVariable : public igdeUndo{
private:
	ceConversationTopic::Ref pTopic;
	ceCASetVariable::Ref pAction;
	decString pOldName;
	decString pNewName;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<ceUCASetVarSetValueVariable> Ref;
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	ceUCASetVarSetValueVariable(ceConversationTopic *topic, ceCASetVariable *action, const char *newName);
	
protected:
	/** \brief Clean up undo. */
	virtual ~ceUCASetVarSetValueVariable();
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
