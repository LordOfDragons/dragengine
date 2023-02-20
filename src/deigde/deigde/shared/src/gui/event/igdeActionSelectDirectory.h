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

#ifndef _IGDEACTIONSELECTDIRECTORY_H_
#define _IGDEACTIONSELECTDIRECTORY_H_

#include <stddef.h>

#include "igdeAction.h"
#include "../../environment/igdeEnvironment.h"


class igdeTextField;


/**
 * \brief IGDE UI Action Select Directory.
 * 
 * Select file using directory dialog. Requires a target igdeTextField to obtain/store the file path.
 */
class DE_DLL_EXPORT igdeActionSelectDirectory : public igdeAction{
private:
	igdeEnvironment &pEnvironment;
	bool pUseGameVFS;
	igdeTextField &pTextField;
	
	
	
public:
	/** \text Constructors and Destructors */
	/*@{*/
	/** \brief Create action. */
	igdeActionSelectDirectory( igdeEnvironment &environment, igdeTextField &textField, bool useGameVFS = true );
	
	
	
protected:
	/**
	 * \brief Clean up action.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeActionSelectDirectory();
	/*@}*/
	
	
	
public:
	/** \text Management */
	/*@{*/
	/** \brief Environment. */
	inline igdeEnvironment &GetEnvironment() const{ return pEnvironment; }
	
	/** \brief Use game virtual file system or native file system. */
	inline bool GetUseGameVFS() const{ return pUseGameVFS; }
	
	/** \brief Text field. */
	inline igdeTextField &GetTextField() const{ return pTextField; }
	
	
	
	/**
	 * \brief Run action.
	 * 
	 * Select resource directory from dialog. Current directory is taken from the text field.
	 * If user selected a directory and it is different updates the text field and notifies
	 * listeners about the change.
	 */
	virtual void OnAction();
	
	/**
	 * \brief Accept directory.
	 * 
	 * Check if selected directory is a useable one. Can be used to ensure users can not select
	 * wrong directories unsuitable for their needs. Default returns true.
	 * 
	 * \note Implementation is allowed to change \em path to modify the stored value.
	 */
	virtual bool AcceptDirectory( decString &directory );
	
	/**
	 * \brief Default directory if text is empty.
	 * 
	 * Default implementation uses root directory for game file system and working directory
	 * for native file system.
	 */
	virtual decString DefaultDirectory();
	/*@}*/
};

#endif
