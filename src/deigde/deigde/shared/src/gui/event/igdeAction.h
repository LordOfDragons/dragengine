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

#ifndef _IGDEACTION_H_
#define _IGDEACTION_H_

#include "../resources/igdeHotKey.h"
#include "../resources/igdeIcon::Ref.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decPointerOrderedSet.h>


class igdeActionListener;


/**
 * \brief IGDE UI Action.
 * 
 * OnAction() is called if user interacts with an UI element triggering actions. Allows to
 * reuse actions across different UI elements. Furthermore typical element states are stored
 * with the elements using this action updating whenever the state information of the action
 * changes. Listeners are stored as pointer only. The owning element is responsible to keep
 * the reference alive as long as needed.
 * 
 * Subclass actions can implement Update() method to allow UI elements or other code to udpate
 * action parameters like for example enabled state. This way the update logic is present in
 * the action itself and UI widgets can request an update avoiding the need to change action
 * parameters "Up Ahead" all the time. Certain ToolKits can take advantage of this feature.
 */
class DE_DLL_EXPORT igdeAction : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeAction> Ref;
	
	
private:
	decString pText;
	decString pDescription;
	igdeHotKey pHotKey;
	deInputEvent::eKeyCodes pMnemonic;
	igdeIcon::Ref pIcon;
	bool pEnabled;
	bool pSelected;
	bool pDefault;
	decPointerOrderedSet pListeners;
	
	
	
protected:
	/** \text Constructors and Destructors */
	/*@{*/
	/** \brief Create action. */
	igdeAction();
	
	igdeAction( const char *text, const char *description = "" );
	
	igdeAction( const char *text, const char *description, const igdeHotKey &hotKey );
	
	igdeAction( const char *text, const char *description, deInputEvent::eKeyCodes mnemonic );
	
	igdeAction( const char *text, const char *description,
		deInputEvent::eKeyCodes mnemonic, const igdeHotKey &hotKey );
	
	igdeAction( const char *text, igdeIcon *icon, const char *description = "" );
	
	igdeAction( const char *text, igdeIcon *icon, const char *description,
		const igdeHotKey &hotKey );
	
	igdeAction( const char *text, igdeIcon *icon, const char *description,
		deInputEvent::eKeyCodes mnemonic );
	
	igdeAction( const char *text, igdeIcon *icon, const char *description,
		deInputEvent::eKeyCodes mnemonic, const igdeHotKey &hotKey );
	
	
	
protected:
	/**
	 * \brief Clean up action.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeAction();
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
	
	/** \brief Menu entry is selected. */
	inline bool GetSelected() const{ return pSelected; }
	
	/** \brief Set if menu entry is selected. */
	void SetSelected( bool selected );
	
	/** \brief Widget is the default widget if used in dialogs. */
	inline bool GetDefault() const{ return pDefault; }
	
	/** \brief Set if if widget is the default widget if used in dialogs. */
	void SetDefault( bool isdefault );
	
	
	
	/**
	 * \brief Add listener.
	 * 
	 * Listener is stored only as pointer. Caller is responsible to hold reference to
	 * action as long as the listener is not removed.
	 */
	void AddListener( igdeActionListener *listener );
	
	/**
	 * \brief Remove listener.
	 */
	void RemoveListener( igdeActionListener *listener );
	
	/** \brief Notify listeners parameters changed. */
	void NotifyParametersChanged();
	
	
	
	/**
	 * \brief Run action.
	 * 
	 * Called if user interacts with a UI element triggering actions.
	 */
	virtual void OnAction() = 0;
	
	/**
	 * \brief Request update of action parameters if required.
	 * 
	 * Called for example by UI elements to update action parameters.
	 * Default implementation does nothing.
	 */
	virtual void Update();
	/*@}*/
};

#endif
