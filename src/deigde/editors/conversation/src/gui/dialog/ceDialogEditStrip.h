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

#ifndef _CEDIALOGEDITSTRIP_H_
#define _CEDIALOGEDITSTRIP_H_

#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/dialog/igdeDialog.h>

class ceStrip;
class decStringList;



/**
 * Dialog edit strip.
 */
class ceDialogEditStrip : public igdeDialog{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<ceDialogEditStrip> Ref;
	
	
	class Listener : public deObject{
	public:
		typedef deTObjectReference<Listener> Ref;
	
	
		Listener();
		virtual float DefaultDuration(const decString &id);
		
	protected:
		virtual ~Listener();
	};
	
private:
	igdeComboBoxFilter::Ref pCBID;
	igdeTextField::Ref pEditPause;
	igdeTextField::Ref pEditDuration;
	igdeButton::Ref pBtnResetDuration;
	Listener::Ref pListener;
	bool pAutoResetDuration;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create dialog. */
	ceDialogEditStrip(igdeEnvironment &environment, const char *windowTitle, const char *textLabel);
	
protected:
	/** Clean up dialog. */
	virtual ~ceDialogEditStrip();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Set entries in list of possible identifiers to select. */
	void SetIDList(const decStringList &list);
	
	/** Set identifier. */
	void SetID(const char *id);
	
	/** Set pause. */
	void SetPause(float pause);
	
	/** Set duration. */
	void SetDuration(float duration);
	
	/** Set from strip. */
	void SetFromStrip(const ceStrip &strip);
	
	/** Update strip. */
	void UpdateStrip(ceStrip &strip) const;
	
	/** Create new strip from data. */
	ceStrip *CreateStrip() const;
	
	/** Set listener or nullptr. */
	void SetListener(const Listener::Ref &listener);
	
	/** Reset duration to default duration. */
	void ResetDuration();
	
	/** Automatically reset duraation on changing identifier. */
	inline bool GetAutoResetDuration() const{return pAutoResetDuration;}
	
	/** Set automatically reset duraation on changing identifier. */
	void SetAutoResetDuration(bool autoResetDuration);
	
	
	
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
