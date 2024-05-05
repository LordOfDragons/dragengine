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

#ifndef _PROJIGDEMODULE_H_
#define _PROJIGDEMODULE_H_

#include <deigde/module/igdeEditorModule.h>



/**
 * \brief DEIGDE Project Editor.
 */
class projIGDEModule : public igdeEditorModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	projIGDEModule( igdeEnvironment &environment );
	
	/** \brief Clean up module. */
	virtual ~projIGDEModule();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Start module. */
	virtual void Start();
	
	/**
	 * \brief Process command line arguments.
	 * 
	 * This method can be called multiple times. Module has to process arguments from the
	 * first argument in the list and has to stop at the first unsupported argument. The
	 * consumed arguments have to be removed from the beginning of the list. If the module
	 * wishes to close the application it has to return false. Return true to continue
	 * processing command line arguments.
	 */
	virtual bool ProcessCommandLine( decUnicodeStringList &arguments );
	/*@}*/
};

#endif
