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

#ifndef _IGDEACTIONSELECTFILE_H_
#define _IGDEACTIONSELECTFILE_H_

#include <stddef.h>

#include "igdeAction.h"
#include "../filedialog/igdeFilePatternList.h"
#include "../../environment/igdeEnvironment.h"


class igdeTextField;


/**
 * \brief IGDE UI Action Select File.
 * 
 * Select file using file dialog or resource browser if suitable. Requires a target
 * igdeTextField to obtain/store the file path.
 */
class DE_DLL_EXPORT igdeActionSelectFile : public igdeAction{
private:
	igdeEnvironment &pEnvironment;
	igdeFilePatternList pFilePatterns;
	bool pUseGameVFS;
	igdeTextField &pTextField;
	
	
	
public:
	/** \text Constructors and Destructors */
	/*@{*/
	/** \brief Create action. */
	igdeActionSelectFile( igdeEnvironment &environment,
		igdeEnvironment::eFilePatternListTypes resourceType, igdeTextField &textField,
		bool useGameVFS = true );
	
	igdeActionSelectFile( igdeEnvironment &environment,
		const igdeFilePatternList &filePatterns, igdeTextField &textField,
		bool useGameVFS = true );
	
	
	
protected:
	/**
	 * \brief Clean up action.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeActionSelectFile();
	/*@}*/
	
	
	
public:
	/** \text Management */
	/*@{*/
	/** \brief Environment. */
	inline igdeEnvironment &GetEnvironment() const{ return pEnvironment; }
	
	/** \brief File pattern list. */
	inline const igdeFilePatternList &GetFilePatterns() const{ return pFilePatterns; }
	
	/** \brief Use game virtual file system or native file system. */
	inline bool GetUseGameVFS() const{ return pUseGameVFS; }
	
	/** \brief Text field. */
	inline igdeTextField &GetTextField() const{ return pTextField; }
	
	
	
	/**
	 * \brief Run action.
	 * 
	 * Select resource file from dialog of resource browser. Current path is taken from the
	 * text field. If user selected a file and it is different updates the text field and
	 * notifies listeners about the change.
	 */
	virtual void OnAction();
	
	/**
	 * \brief Prepare file to init dialog with.
	 * 
	 * \note Implementation is allowed to change \em path to modify the initial value.
	 */
	virtual void PrepareFile( decString &path );
	
	/**
	 * \brief Accept file.
	 * 
	 * Check if selected file is a useable one. Can be used to ensure users can not select
	 * wrong files unsuitable for their needs. Default returns true.
	 * 
	 * \note Implementation is allowed to change \em path to modify the stored value.
	 */
	virtual bool AcceptFile( decString &path );
	
	/**
	 * \brief Default filename if text is empty.
	 * 
	 * Default implementation uses empty path.
	 */
	virtual decString DefaultPath();
	/*@}*/
};

#endif
