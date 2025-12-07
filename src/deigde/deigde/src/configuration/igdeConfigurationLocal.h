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

#ifndef _IGDECONFIGURATIONLOCAL_H_
#define _IGDECONFIGURATIONLOCAL_H_

#include <deigde/utils/igdeRecentFiles.h>

class igdeWindowMain;



/**
 * \brief IGDE Local Configuration.
 */
class igdeConfigurationLocal{
private:
	igdeWindowMain &pWindowMain;
	
	igdeRecentFiles pRecentEditorFiles;
	
	bool pPreventSaving;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create local configuration. */
	igdeConfigurationLocal(igdeWindowMain &windowMain);
	
	/** \brief Clean up local configuration. */
	~igdeConfigurationLocal();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline igdeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	
	
	/** \brief Recent editor files. */
	inline igdeRecentFiles &GetRecentEditorFiles(){ return pRecentEditorFiles; }
	inline const igdeRecentFiles &GetRecentEditorFiles() const{ return pRecentEditorFiles; }
	
	
	
	/** \brief Prevent saving. */
	inline bool GetPreventSaving() const{ return pPreventSaving; }
	
	/** \brief Set prevent saving. */
	void SetPreventSaving(bool preventSaving);
	
	/** \brief Load the configuration. */
	void LoadConfiguration();
	
	/** \brief Save configuration. */
	void SaveConfiguration();
	/*@}*/
	
	
	
private:
	void pReset();
};

#endif
