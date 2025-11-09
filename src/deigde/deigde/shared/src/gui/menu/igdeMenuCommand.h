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

#ifndef _IGDEMENUCOMMAND_H_
#define _IGDEMENUCOMMAND_H_

#include "../igdeWidget.h"
#include "../event/igdeActionListener.h"
#include "../event/igdeActionReference.h"
#include "../resources/igdeHotKey.h"
#include "../resources/igdeIconReference.h"


/**
 * \brief IGDE UI Command Menu Entry.
 * 
 * Calls OnAction() if user clicks the menu entry and it is enabled.
 */
class DE_DLL_EXPORT igdeMenuCommand : public igdeWidget, igdeActionListener{
public:
	/** \brief Strong reference. */
	typedef deTObjectReference<igdeMenuCommand> Ref;
	
	
private:
	decString pText;
	decString pDescription;
	igdeHotKey pHotKey;
	deInputEvent::eKeyCodes pMnemonic;
	igdeIconReference pIcon;
	bool pEnabled;
	igdeActionReference pAction;
	
	
	
public:
	/** \text Constructors and Destructors */
	/*@{*/
	/** \brief Create menu entry. */
	igdeMenuCommand( igdeEnvironment &environment );
	
	/** \brief Create menu entry. */
	igdeMenuCommand( igdeEnvironment &environment, igdeAction *action );
	
	
	
protected:
	/**
	 * \brief Clean up menu entry.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeMenuCommand();
	/*@}*/
	
	
	
public:
	/** \text Management */
	/*@{*/
	/** \brief Text shown in the menu entry. */
	inline const decString &GetText() const{ return pText; }
	
	/** \brief Set text shown in the menu entry. */
	void SetText( const char *text );
	
	/** \brief Description shown in tool tips. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description shown in tool tips. */
	void SetDescription( const char *description );
	
	/** \brief Hot-Key. */
	const igdeHotKey &GetHotKey() const{ return pHotKey; }
	
	/** \brief Set Hot-Key. */
	void SetHotKey( const igdeHotKey &hotKey );
	
	/** \brief Mnemonic key or ekcUndefined if not used. */
	inline deInputEvent::eKeyCodes GetMnemonic() const{ return pMnemonic; }
	
	/** \brief Set mnemonic key or ekcUndefined if not used. */
	void SetMnemonic( deInputEvent::eKeyCodes mnemonic );
	
	/** \brief Icon or NULL. */
	inline igdeIcon *GetIcon() const{ return pIcon; }
	
	/** \brief Set icon or NULL. */
	void SetIcon( igdeIcon *icon );
	
	/** \brief Menu entry is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if menu entry is enabled. */
	void SetEnabled( bool enabled );
	
	/** \brief Action or NULL. */
	inline igdeAction *GetAction() const{ return pAction; }
	
	/** \brief Set action or NULL. */
	void SetAction( igdeAction *action );
	
	
	
	/**
	 * \brief Run menu action.
	 * 
	 * Called if menu action is clicked by the user. Default implementation does nothing.
	 */
	virtual void OnAction();
	
	/** \brief Action parameters changed. */
	virtual void OnParameterChanged( igdeAction *action );
	
	/** \brief Action has been destroyed. */
	virtual void OnDestroyed( igdeAction *action );
	/*@}*/
	
	
	
	/**
	 * \text IGDE Internal Use Only
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	/*@{*/
	/**
	 * \brief Create native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void CreateNativeWidget();
	
	/**
	 * \brief Destroy native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void DestroyNativeWidget();
	
	
	
protected:
	/** \brief Text changed. */
	virtual void OnTextChanged();
	
	/** \brief Description changed. */
	virtual void OnDescriptionChanged();
	
	/** \brief Hot-key changed. */
	virtual void OnHotKeyChanged();
	
	/** \brief Mnemonic changed. */
	virtual void OnMnemonicChanged();
	
	/** \brief Icon changed. */
	virtual void OnIconChanged();
	
	/** \brief Enabled changed. */
	virtual void OnEnabledChanged();
	/*@}*/
};

#endif
