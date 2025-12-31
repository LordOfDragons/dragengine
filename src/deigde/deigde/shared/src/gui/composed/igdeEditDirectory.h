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

#ifndef _IGDEEDITEDITDIRECTORY_H_
#define _IGDEEDITEDITDIRECTORY_H_

#include "../igdeButton.h"
#include "../igdeTextField.h"
#include "../event/igdeAction.h"
#include "../event/igdeActionSelectDirectory.h"
#include "../event/igdeTextFieldListener.h"
#include "../layout/igdeContainerFlow.h"

#include <dragengine/common/string/decString.h>
#include <dragengine/common/collection/decTOrderedSet.h>

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

public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeEditDirectory> Ref;
	
	
protected:
	/** \brief Text field listener. */
	class DE_DLL_EXPORT cListenerTextField : public igdeTextFieldListener{
	protected:
		igdeEditDirectory &pEditDirectory;
		
	public:
		typedef deTObjectReference<cListenerTextField> Ref;
		explicit cListenerTextField(igdeEditDirectory &editDirectory);
		
	protected:
		virtual ~cListenerTextField();
		
	public:
		virtual void OnTextChanged(igdeTextField *textField);
	};
	
	/** \brief Button action. */
	class DE_DLL_EXPORT cActionButton : public igdeActionSelectDirectory{
	protected:
		igdeEditDirectory &pEditDirectory;
		
	public:
		typedef deTObjectReference<cActionButton> Ref;
		cActionButton(igdeEditDirectory &editDirectory, igdeTextField &textField, const char *description);
		
	protected:
		virtual ~cActionButton();
		
	public:
		virtual decString DefaultDirectory();
		virtual void Update();
	};
	
	
	
private:
	decString pDefaultDirectory;
	bool pAutoValidateDirectory;
	bool pUseGameVFS;
	
	igdeActionSelectDirectory::Ref pActionButton;
	
	igdeTextField::Ref pText;
	igdeButton::Ref pButton;
	
	decTObjectOrderedSet<igdeEditDirectoryListener> pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create edit directory. */
	igdeEditDirectory(igdeUIHelper &helper, const char *description = "", bool useGameVFS = true);
	
	
	
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
	void SetDirectory(const char *directory);
	
	/** \brief Clear path. */
	void ClearDirectory();
	
	/** \brief Widget is enabled. */
	bool GetEnabled() const;
	
	/** \brief Set if widget is enabled. */
	void SetEnabled(bool enabled);
	
	/** \brief Description shown in tool tips. */
	const decString &GetDescription() const;
	
	/** \brief Set description shown in tool tips. */
	void SetDescription(const char *description);
	
	/** \brief Default path. */
	inline const decString &GetDefaultDirectory() const{ return pDefaultDirectory; }
	
	/** \brief Set default path. */
	void SetDefaultDirectory(const char *path);
	
	/** \brief Directory is valid. */
	bool IsDirectoryValid() const;
	
	/** \brief Auto validate path if changed. */
	inline bool GetAutoValidateDirectory() const{ return pAutoValidateDirectory; }
	
	/** \brief Set if path is auto validated if changed. */
	void SetAutoValidateDirectory(bool autoValidateDirectory);
	
	/** \brief Validate path. */
	void ValidateDirectory();
	
	/** \brief Use game virtual file system or native file system. */
	inline bool GetUseGameVFS() const{ return pUseGameVFS; }
	
	/** \brief Focus widget. */
	void Focus();
	
	
	
	/** \brief Add listener. */
	void AddListener(igdeEditDirectoryListener *listener);
	
	/** \brief Remove listener. */
	void RemoveListener(igdeEditDirectoryListener *listener);
	
	/** \brief Notify listeners path changed. */
	virtual void NotifyEditDirectoryChanged();
	/*@}*/
};

#endif
