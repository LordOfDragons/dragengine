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

#ifndef _CEWPCACTORCOMMAND_H_
#define _CEWPCACTORCOMMAND_H_

#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeTextField.h>

#include "ceWPCondition.h"

class ceCConditionActorCommand;



/**
 * \brief Actor command conversation condition property window.
 */
class ceWPCActorCommand : public ceWPCondition{
public:
	typedef deTObjectReference<ceWPCActorCommand> Ref;

private:
	igdeComboBox::Ref pCBActorID;
	igdeTextField::Ref pEditCommand;
	igdeButton::Ref pBtnCommand;
	igdeCheckBox::Ref pChkNegate;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	ceWPCActorCommand(ceWPTopic &parentPanel);
	
protected:
	/** \brief Clean up panel. */
	~ceWPCActorCommand() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Active condition or nullptr if none is active. */
	ceCConditionActorCommand *GetCondition() const;
	
	/** \brief Update active condition. */
	void UpdateCondition();
	
	/** \brief Update actor id list. */
	void UpdateActorIDList();
	/*@}*/
};

#endif
