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

public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeActionSelectFile> Ref;
	
	
private:
	igdeEnvironment &pEnvironment;
	igdeFilePatternList pFilePatterns;
	bool pUseGameVFS;
	igdeTextField &pTextField;
	
	
	
public:
	/** \text Constructors and Destructors */
	/*@{*/
	/** \brief Create action. */
	igdeActionSelectFile(igdeEnvironment &environment,
		igdeEnvironment::eFilePatternListTypes resourceType, igdeTextField &textField,
		bool useGameVFS = true);
	
	igdeActionSelectFile(igdeEnvironment &environment,
		const igdeFilePatternList &filePatterns, igdeTextField &textField,
		bool useGameVFS = true);
	
	
	
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
	virtual void PrepareFile(decString &path);
	
	/**
	 * \brief Accept file.
	 * 
	 * Check if selected file is a useable one. Can be used to ensure users can not select
	 * wrong files unsuitable for their needs. Default returns true.
	 * 
	 * \note Implementation is allowed to change \em path to modify the stored value.
	 */
	virtual bool AcceptFile(decString &path);
	
	/**
	 * \brief Default filename if text is empty.
	 * 
	 * Default implementation uses empty path.
	 */
	virtual decString DefaultPath();
	/*@}*/
};

#endif
