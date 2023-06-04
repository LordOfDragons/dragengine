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

#ifndef _IGDEEDITEDITDIRECTORY_H_
#define _IGDEEDITEDITDIRECTORY_H_

#include "../igdeButtonReference.h"
#include "../igdeTextFieldReference.h"
#include "../event/igdeAction.h"
#include "../event/igdeActionReference.h"
#include "../event/igdeActionSelectDirectory.h"
#include "../event/igdeActionSelectDirectoryReference.h"
#include "../event/igdeTextFieldListener.h"
#include "../layout/igdeContainerFlow.h"

#include <dragengine/common/string/decString.h>

class igdeEditDirectoryListener;
class igdeMenuCascade;
class igdeUIHelper;


/**
 * \brief IGDE UI Edit Directory.
 * 
 * Composed widget to edit directory using text field and button. The main widget is the text
 * widget. The button is used to bring up directory dialogs to select directory.
 */
class DE_DLL_EXPORT igdeEditDirectory : public igdeContainerFlow{
protected:
	/** \brief Text field listener. */
	class DE_DLL_EXPORT cListenerTextField : public igdeTextFieldListener{
	protected:
		igdeEditDirectory &pEditDirectory;
		
	public:
		cListenerTextField( igdeEditDirectory &editDirectory );
		virtual ~cListenerTextField();
		virtual void OnTextChanged( igdeTextField *textField );
	};
	
	/** \brief Button action. */
	class DE_DLL_EXPORT cActionButton : public igdeActionSelectDirectory{
	protected:
		igdeEditDirectory &pEditDirectory;
		
	public:
		cActionButton( igdeEditDirectory &editDirectory, igdeTextField &textField, const char *description );
		virtual ~cActionButton();
		virtual decString DefaultDirectory();
		virtual void Update();
	};
	
	
	
private:
	decString pDefaultDirectory;
	bool pAutoValidateDirectory;
	bool pUseGameVFS;
	
	igdeActionSelectDirectoryReference pActionButton;
	
	igdeTextFieldReference pText;
	igdeButtonReference pButton;
	
	decObjectOrderedSet pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create edit directory. */
	igdeEditDirectory( igdeUIHelper &helper, const char *description = "", bool useGameVFS = true );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeEditDirectory();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Directory. */
	const decString &GetDirectory() const;
	
	/**
	 * \brief Set directory.
	 * 
	 * Validates directory if changed.
	 */
	void SetDirectory( const char *directory );
	
	/** \brief Clear path. */
	void ClearDirectory();
	
	/** \brief Widget is enabled. */
	bool GetEnabled() const;
	
	/** \brief Set if widget is enabled. */
	void SetEnabled( bool enabled );
	
	/** \brief Description shown in tool tips. */
	const decString &GetDescription() const;
	
	/** \brief Set description shown in tool tips. */
	void SetDescription( const char *description );
	
	/** \brief Default path. */
	inline const decString &GetDefaultDirectory() const{ return pDefaultDirectory; }
	
	/** \brief Set default path. */
	void SetDefaultDirectory( const char *path );
	
	/** \brief Directory is valid. */
	bool IsDirectoryValid() const;
	
	/** \brief Auto validate path if changed. */
	inline bool GetAutoValidateDirectory() const{ return pAutoValidateDirectory; }
	
	/** \brief Set if path is auto validated if changed. */
	void SetAutoValidateDirectory( bool autoValidateDirectory );
	
	/** \brief Validate path. */
	void ValidateDirectory();
	
	/** \brief Use game virtual file system or native file system. */
	inline bool GetUseGameVFS() const{ return pUseGameVFS; }
	
	/** \brief Focus widget. */
	void Focus();
	
	
	
	/** \brief Add listener. */
	void AddListener( igdeEditDirectoryListener *listener );
	
	/** \brief Remove listener. */
	void RemoveListener( igdeEditDirectoryListener *listener );
	
	/** \brief Notify listeners path changed. */
	virtual void NotifyEditDirectoryChanged();
	/*@}*/
};

#endif
