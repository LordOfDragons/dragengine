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

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/dialog/igdeDialog.h>

class ceStrip;
class decStringList;



/**
 * Dialog edit strip.
 */
class ceDialogEditStrip : public igdeDialog{
public:
	class Listener : public deObject{
	public:
		typedef deTObjectReference<Listener> Ref;
		
		Listener();
		virtual float DefaultDuration(  const decString &id );
		
	protected:
		virtual ~Listener();
	};
	
private:
	igdeComboBoxFilterReference pCBID;
	igdeTextFieldReference pEditPause;
	igdeTextFieldReference pEditDuration;
	igdeButtonReference pBtnResetDuration;
	Listener::Ref pListener;
	bool pAutoResetDuration;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create dialog. */
	ceDialogEditStrip( igdeEnvironment &environment, const char *windowTitle, const char *textLabel );
	
protected:
	/** Clean up dialog. */
	virtual ~ceDialogEditStrip();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Set entries in list of possible identifiers to select. */
	void SetIDList( const decStringList &list );
	
	/** Set identifier. */
	void SetID( const char *id );
	
	/** Set pause. */
	void SetPause( float pause );
	
	/** Set duration. */
	void SetDuration( float duration );
	
	/** Set from strip. */
	void SetFromStrip( const ceStrip &strip );
	
	/** Update strip. */
	void UpdateStrip( ceStrip &strip ) const;
	
	/** Create new strip from data. */
	ceStrip *CreateStrip() const;
	
	/** Set listener or nullptr. */
	void SetListener( const Listener::Ref &listener );
	
	/** Reset duration to default duration. */
	void ResetDuration();
	
	/** Automatically reset duraation on changing identifier. */
	inline bool GetAutoResetDuration() const{ return pAutoResetDuration; }
	
	/** Set automatically reset duraation on changing identifier. */
	void SetAutoResetDuration( bool autoResetDuration );
	
	
	
	/**
	 * Init running.
	 * 
	 * Called by Run() after dialog is created. Subclass can implement to init controls
	 * after the dialog is visible on screen.
	 */
	virtual void OnDialogShown();
	/*@}*/
};

#endif
