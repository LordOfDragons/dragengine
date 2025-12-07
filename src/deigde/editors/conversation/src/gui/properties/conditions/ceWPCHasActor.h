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

#ifndef _CEWPCHASACTOR_H_
#define _CEWPCHASACTOR_H_

#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>

#include "ceWPCondition.h"

class ceCConditionHasActor;



/**
 * \brief Has Actor Conversation Condition property window.
 */
class ceWPCHasActor : public ceWPCondition{
private:
	igdeComboBox::Ref pCBActor;
	igdeCheckBox::Ref pChkNegate;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	ceWPCHasActor(ceWPTopic &parentPanel);
	
protected:
	/** \brief Clean up panel. */
	~ceWPCHasActor() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Active condition. */
	ceCConditionHasActor *GetCondition() const;
	
	/** \brief Update active condition. */
	void UpdateCondition();
	
	/** \brief Update actor list. */
	void UpdateActorList();
	/*@}*/
};

#endif
