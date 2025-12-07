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

#ifndef _IGDEACTIONEXTERNOPEN_H_
#define _IGDEACTIONEXTERNOPEN_H_

#include <stddef.h>

#include "igdeAction.h"

class igdeEnvironment;


/**
 * \brief IGDE UI Action System Open.
 * 
 * Open file or url using system specific application. For example directory path is opened
 * with a file manager, registered file types are opened with their preferred application
 * and URL are openned with their preferred application.
 */
class DE_DLL_EXPORT igdeActionExternOpen : public igdeAction{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeActionExternOpen> Ref;
	
	
private:
	igdeEnvironment &pEnvironment;
	decString pPath;
	bool pEnsureExists;
	
	
public:
	/** \text Constructors and Destructors */
	/*@{*/
	/** \brief Create action. */
	igdeActionExternOpen(igdeEnvironment &environment, const char *text,
		igdeIcon *icon, const char *description, const char *path = "");
	
	
protected:
	/**
	 * \brief Clean up action.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	~igdeActionExternOpen() override;
	/*@}*/
	
	
public:
	/** \text Management */
	/*@{*/
	/** \brief Environment. */
	inline igdeEnvironment &GetEnvironment() const{return pEnvironment;}
	
	/** \brief Path to open or empty string if not set. */
	inline const decString &GetPath() const{return pPath;}
	
	/** \brief Set path to open or empty string if not set. */
	void SetPath(const char *path);
	
	/** \brief Ensure path exists before opening it. */
	inline bool GetEnsureExists() const{return pEnsureExists;}
	
	/** \brief Set to ensure path exists before opening it. */
	void SetEnsureExists(bool ensureExists);
	
	
	/**
	 * \brief Run action.
	 * 
	 * Called if user interacts with a UI element triggering actions.
	 */
	void OnAction() override;
	
	/**
	 * \brief Request update of action parameters if required.
	 * 
	 * Called for example by UI elements to update action parameters.
	 * Default implementation does nothing.
	 */
	void Update() override;
	/*@}*/
};

#endif
