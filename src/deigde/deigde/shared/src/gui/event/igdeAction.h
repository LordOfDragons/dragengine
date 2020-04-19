/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEACTION_H_
#define _IGDEACTION_H_

#include "../resources/igdeHotKey.h"
#include "../resources/igdeIconReference.h"

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
class igdeAction : public deObject{
private:
	decString pText;
	decString pDescription;
	igdeHotKey pHotKey;
	deInputEvent::eKeyCodes pMnemonic;
	igdeIconReference pIcon;
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
