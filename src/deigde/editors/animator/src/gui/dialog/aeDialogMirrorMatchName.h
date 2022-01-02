/* 
 * Drag[en]gine IGDE Animator Editor
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#ifndef _AEDIALOGMIRRORMATCHNAME_H_
#define _AEDIALOGMIRRORMATCHNAME_H_

#include "../../animator/rule/aeRuleMirror.h"

#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/dialog/igdeDialog.h>


/**
 * Dialog animator rule mirror match name.
 */
class aeDialogMirrorMatchName : public igdeDialog{
private:
	igdeComboBoxReference pCBType;
	igdeTextFieldReference pEditFirst;
	igdeTextFieldReference pEditSecond;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create dialog. */
	aeDialogMirrorMatchName( igdeEnvironment &environment, const char *windowTitle );
	
protected:
	/** Clean up dialog. */
	virtual ~aeDialogMirrorMatchName();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Set from match name. */
	void Set( const aeRuleMirror::cMatchName &matchName );
	
	/** Create match name. */
	aeRuleMirror::cMatchName::Ref CreateMatchName() const;
	
	/**
	 * Init running. Called by Run() after dialog is created. Subclass can implement to
	 * init controls after the dialog is visible on screen.
	 */
	virtual void OnDialogShown();
	
	/** Accept dialog. */
	virtual bool Accept();
	/*@}*/
};

#endif
