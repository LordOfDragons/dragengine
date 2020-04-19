/* 
 * Drag[en]gine IGDE Conversation Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _CEDIALOGPASTESNIPPET_H_
#define _CEDIALOGPASTESNIPPET_H_

#include "../../conversation/action/ceConversationActionList.h"

#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/igdeTextAreaReference.h>
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
	
	igdeTextAreaReference pEditSnippet;
	igdeComboBoxFilterReference pCBCameraShot1;
	igdeComboBoxFilterReference pCBCameraShot2;
	igdeComboBoxFilterReference pCBTarget1;
	igdeComboBoxFilterReference pCBTarget2;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	ceDialogPasteSnippet( igdeEnvironment &environment, ceConversation *conversation );
	
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
	void WordsFromText( ceStripList &wordList, const decUnicodeString &text );
	
	/**
	 * \brief List of action snippet.
	 * 
	 * Present after dialog returned successfully.
	 */
	inline const ceConversationActionList &GetActions() const{ return pActions; }
	
	
	
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
