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

#ifndef _CEDIALOGEDITSTRIP_H_
#define _CEDIALOGEDITSTRIP_H_

#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/dialog/igdeDialog.h>

class ceStrip;
class decStringList;



/**
 * \brief Dialog edit strip.
 */
class ceDialogEditStrip : public igdeDialog{
private:
	igdeComboBoxFilterReference pCBID;
	igdeTextFieldReference pEditPause;
	igdeTextFieldReference pEditDuration;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	ceDialogEditStrip( igdeEnvironment &environment, const char *windowTitle, const char *textLabel );
	
protected:
	/** \brief Clean up dialog. */
	virtual ~ceDialogEditStrip();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Set entries in list of possible identifiers to select. */
	void SetIDList( const decStringList &list );
	
	/** \brief Set identifier. */
	void SetID( const char *id );
	
	/** \brief Set pause. */
	void SetPause( float pause );
	
	/** \brief Set duration. */
	void SetDuration( float duration );
	
	/** \brief Set from strip. */
	void SetFromStrip( const ceStrip &strip );
	
	/** \brief Update strip. */
	void UpdateStrip( ceStrip &strip ) const;
	
	/** \brief Create new strip from data. */
	ceStrip *CreateStrip() const;
	
	
	
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
