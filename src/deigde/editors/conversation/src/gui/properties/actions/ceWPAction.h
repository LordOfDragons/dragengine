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

#ifndef _CEWPACTION_H_
#define _CEWPACTION_H_

#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/layout/igdeContainerForm.h>

class igdeComboBox;

class ceWPTopic;
class ceConversationAction;



/**
 * \brief Conversation Action property window.
 */
class ceWPAction : public igdeContainerForm{
protected:
	ceWPTopic &pParentPanel;
	
	igdeCheckBoxReference pChkWaitForActor, pChkWaitSpeakOnly;
	igdeComboBoxReference pCBWaitForActorID;
	igdeTextFieldReference pEditDelay;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	ceWPAction( ceWPTopic &parentPanel );
	
protected:
	/** \brief Clean up panel. */
	virtual ~ceWPAction();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Parent panel. */
	inline ceWPTopic &GetParentPanel() const{ return pParentPanel; }
	
	/** \brief Create common action parameters. */
	void CreateGUICommon( igdeContainerForm &container );
	
	/** \brief Update common action parameters. */
	void UpdateCommonParams();
	
	/** \brief Update a combo box with the list of actor ids an alias ids. */
	void UpdateComboBoxWithActorIDList( igdeComboBox &combobox );
	
	/** \brief Update a combo box with the list of convo coord system ids an alias ids. */
	void UpdateComboBoxWithConvoCoordSysIDList( igdeComboBox &combobox );
	
	/** \brief Update actor id lists. */
	virtual void UpdateActorIDLists();
	
	/** \brief Conversation path changed. */
	virtual void OnConversationPathChanged();
	/*@}*/
};

#endif
