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

#ifndef _CEDIALOGPASTESNIPPET_H_
#define _CEDIALOGPASTESNIPPET_H_

#include "../../conversation/action/ceConversationActionList.h"

#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/dialog/igdeDialog.h>

class ceStripList;
class decUnicodeString;
class ceConversationAction;
class ceConversationFile;
class ceConversationTopic;
class ceConversation;



/**
 * \brief Dialog paste snippet.
 */
class ceDialogPasteSnippet : public igdeDialog{
private:
	ceConversation *pConversation;
	
	ceConversationActionList pActions;
	
	igdeTextArea::Ref pEditSnippet;
	igdeComboBoxFilter::Ref pCBCameraShot1;
	igdeComboBoxFilter::Ref pCBCameraShot2;
	igdeComboBoxFilter::Ref pCBTarget1;
	igdeComboBoxFilter::Ref pCBTarget2;
	igdeTextField::Ref pEditDelayCameraShot;
	igdeTextField::Ref pEditDelayActorSpeak;
	igdeTextField::Ref pEditScaleActorSpeak;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<ceDialogPasteSnippet> Ref;
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	ceDialogPasteSnippet(igdeEnvironment &environment, ceConversation *conversation);
	
protected:
	/** \brief Clean up dialog. */
	virtual ~ceDialogPasteSnippet();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Update camera shot lists. */
	void UpdateCameraShotLists();
	
	/** \brief Update target lists. */
	void UpdateTargetLists();
	
	/** \brief Generate actions. */
	void GenerateActions();
	
	/** \brief Words from text. */
	void WordsFromText(ceStripList &wordList, const decUnicodeString &text, float scale);
	
	/**
	 * \brief List of action snippet.
	 * 
	 * Present after dialog returned successfully.
	 */
	inline const ceConversationActionList &GetActions() const{return pActions;}
	
	
	
	/** \brief Accept dialog. */
	virtual bool Accept();
	
	
	
	/**
	 * \brief Init running.
	 * 
	 * Called by Run() after dialog is created. Subclass can implement to init controls
	 * after the dialog is visible on screen.
	 */
	virtual void OnDialogShown();
	/*@}*/
};

#endif
