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
