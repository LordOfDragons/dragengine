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

#ifndef _IGDEEDITEDITPATH_H_
#define _IGDEEDITEDITPATH_H_

#include "../igdeButtonReference.h"
#include "../igdeTextFieldReference.h"
#include "../event/igdeAction.h"
#include "../event/igdeActionContextMenu.h"
#include "../event/igdeActionContextMenuReference.h"
#include "../event/igdeActionReference.h"
#include "../event/igdeActionSelectFile.h"
#include "../filedialog/igdeFilePatternList.h"
#include "../event/igdeTextFieldListener.h"
#include "../layout/igdeContainerFlow.h"
#include "../../environment/igdeEnvironment.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>

class igdeEditPathListener;
class igdeMenuCascade;
class igdeUIHelper;


/**
 * \brief IGDE UI Edit Path.
 * 
 * Composed widget to edit path using text field and button. The main widget is the text
 * widget. The button is used to bring up file dialogs or other types of dialogs allowing
 * to select a path. The file type information is used to figure out all possible sources
 * the path can be selected with. If more than one source exists a popup menu is used to
 * allow the user to select the source to use to locate the path.
 */
class DE_DLL_EXPORT igdeEditPath : public igdeContainerFlow{
protected:
	/** \brief Text field listener. */
	class DE_DLL_EXPORT cListenerTextField : public igdeTextFieldListener{
	protected:
		igdeEditPath &pEditPath;
		
	public:
		cListenerTextField( igdeEditPath &editPath );
		virtual ~cListenerTextField();
		virtual void OnTextChanged( igdeTextField *textField );
	};
	
	/** \brief Button action. */
	class DE_DLL_EXPORT cActionButton : public igdeAction{
	protected:
		igdeEditPath &pEditPath;
		
	public:
		cActionButton( igdeEditPath &editPath, const char *description );
		virtual ~cActionButton();
		virtual void OnAction();
		virtual void Update();
	};
	
	/** \brief Button menu action. */
	class DE_DLL_EXPORT cActionButtonMenu : public igdeActionContextMenu{
	protected:
		igdeEditPath &pEditPath;
		
	public:
		cActionButtonMenu( igdeEditPath &editPath, const char *description );
		virtual ~cActionButtonMenu();
		virtual void AddContextMenuEntries( igdeMenuCascade &contextMenu );
		virtual void Update();
	};
	
	/** \brief Select file using file dialog action. */
	class DE_DLL_EXPORT cActionSelectFileDialog : public igdeActionSelectFile{
	protected:
		igdeEditPath &pEditPath;
		bool pUseRelativePath;
		
	public:
		cActionSelectFileDialog( igdeEditPath &editPath, igdeTextField &textField );
		virtual ~cActionSelectFileDialog();
		virtual void PrepareFile( decString &path );
		virtual bool AcceptFile( decString &path );
		virtual decString DefaultPath();
	};
	
	/** \brief Select skin resource using skin selection dialog. */
	class DE_DLL_EXPORT cActionSkinSelectDialog : public igdeAction{
	protected:
		igdeEditPath &pEditPath;
		
	public:
		cActionSkinSelectDialog( igdeEditPath &editPath );
		virtual ~cActionSkinSelectDialog();
		virtual void OnAction();
	};
	
	/** \brief Browse file action. */
	class DE_DLL_EXPORT cActionBrowseFile : public igdeAction{
	protected:
		igdeEditPath &pEditPath;
		
	public:
		cActionBrowseFile( igdeEditPath &editPath );
		virtual ~cActionBrowseFile();
		virtual void OnAction();
		virtual void Update();
	};
	
	/** \brief Convert to absolute path. */
	class DE_DLL_EXPORT cActionConvertAbsolute : public igdeAction{
	protected:
		igdeEditPath &pEditPath;
		
	public:
		cActionConvertAbsolute( igdeEditPath &editPath );
		virtual ~cActionConvertAbsolute();
		virtual void OnAction();
		virtual void Update();
	};
	
	/** \brief Convert to relative path. */
	class DE_DLL_EXPORT cActionConvertRelative : public igdeAction{
	protected:
		igdeEditPath &pEditPath;
		
	public:
		cActionConvertRelative( igdeEditPath &editPath );
		virtual ~cActionConvertRelative();
		virtual void OnAction();
		virtual void Update();
	};
	
	
	
private:
	igdeEnvironment::eFilePatternListTypes pResourceType;
	igdeFilePatternList pCustomPatternList;
	
	decObjectOrderedSet pSelectPathActions;
	decString pDefaultPath;
	bool pAutoValidatePath;
	bool pUseGameVFS;
	decString pBasePath;
	
	igdeActionReference pActionButton;
	igdeActionContextMenuReference pActionButtonMenu;
	
	igdeTextFieldReference pText;
	igdeButtonReference pButton;
	igdeButtonReference pButtonMenu;
	
	decObjectOrderedSet pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create edit point. */
	igdeEditPath( igdeUIHelper &helper, igdeEnvironment::eFilePatternListTypes resourceType,
		const char *description = "", bool useGameVFS = true );
	
	igdeEditPath( igdeUIHelper &helper, const igdeFilePatternList &filePatterns,
		const char *description = "", bool useGameVFS = true );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeEditPath();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Resource type. */
	inline igdeEnvironment::eFilePatternListTypes GetResourceType() const{ return pResourceType; }
	
	/**
	 * \brief Set resource type.
	 * 
	 * If resource type is different than the current type SetSelectPathActions() is called.
	 */
	void SetResourceType( igdeEnvironment::eFilePatternListTypes resourceType );
	
	/**
	 * \brief Custom pattern list overruling GetResourceType if not empty.
	 * 
	 * If you change the content call SetSelectPathActions() to make the changes effective.
	 */
	inline igdeFilePatternList &GetCustomPatternList(){ return pCustomPatternList; }
	inline const igdeFilePatternList &GetCustomPatternList() const{ return pCustomPatternList; }
	
	
	
	/** \brief Path. */
	const decString &GetPath() const;
	
	/**
	 * \brief Set path.
	 * 
	 * Validates path if changed.
	 */
	void SetPath( const char *path );
	
	/** \brief Clear path. */
	void ClearPath();
	
	/** \brief Absolute path. */
	decString GetAbsolutePath() const;
	
	/** \brief Absolute path. */
	decString AsAbsolutePath(const decString &path) const;
	
	/** \brief Relative path. */
	decString GetRelativePath() const;
	
	/** \brief Relative path. */
	decString AsRelativePath(const decString &path) const;
	
	/** \brief Path is absolute. */
	bool IsAbsolutePath() const;
	
	/** \brief Path is absolute. */
	bool IsAbsolutePath(const decString &path) const;
	
	/** \brief Path is relative. */
	bool IsRelativePath() const;
	
	/** \brief Path is relative. */
	bool IsRelativePath(const decString &path) const;
	
	/** \brief Widget is enabled. */
	bool GetEnabled() const;
	
	/** \brief Set if widget is enabled. */
	void SetEnabled( bool enabled );
	
	/** \brief Description shown in tool tips. */
	const decString &GetDescription() const;
	
	/** \brief Set description shown in tool tips. */
	void SetDescription( const char *description );
	
	/** \brief Default path. */
	inline const decString &GetDefaultPath() const{ return pDefaultPath; }
	
	/** \brief Set default path. */
	void SetDefaultPath( const char *path );
	
	/** \brief Path is valid. */
	bool IsPathValid() const;
	
	/** \brief Auto validate path if changed. */
	inline bool GetAutoValidatePath() const{ return pAutoValidatePath; }
	
	/** \brief Set if path is auto validated if changed. */
	void SetAutoValidatePath( bool autoValidatePath );
	
	/** \brief Validate path. */
	void ValidatePath();
	
	/** \brief Use game virtual file system or native file system. */
	inline bool GetUseGameVFS() const{ return pUseGameVFS; }
	
	/**
	 * \brief Base path or empty string.
	 * 
	 * If base is set relative path can be entered.
	 */
	inline const decString &GetBasePath() const{ return pBasePath; }
	
	/**
	 * \brief Set base path or empty string.
	 * 
	 * If base is set relative path can be entered.
	 */
	void SetBasePath( const char *path );
	
	/** \brief Focus widget. */
	void Focus();
	
	
	
	/** \brief Number of select path actions. */
	int GetSelectPathActionCount() const;
	
	/** \brief Get select path action at index. */
	igdeAction *GetSelectPathActionAt( int index ) const;
	
	/** \brief Add action providing support to select path. */
	void AddSelectPathAction( igdeAction *action );
	
	/** \brief Remove action providing support to select path. */
	void RemoveSelectPathAction( igdeAction *action );
	
	/** \brief Remove all actions providing support to select path. */
	void RemoveAllSelectPathActions();
	
	/** \brief Call Update() on all select path actions. */
	void UpdateSelectPathActions();
	
	/**
	 * \brief Set select path actions.
	 * 
	 * Default implementation uses the resource type to all possible actions.
	 * 
	 * \warning Called by the constructor.
	 */
	virtual void SetSelectPathActions();
	
	
	
	/**
	 * \brief Add context menu entries.
	 * 
	 * Called by menu button action.
	 */
	virtual void AddContextMenuEntries( igdeMenuCascade &contextMenu );
	
	/** \brief Convert to absolute path if possible. */
	void ToAbsolutePath();
	
	/** \brief Convert to relative path if possible. */
	void ToRelativePath();
	
	/** \brief Open path in file system browser. */
	void BrowsePath();
	
	
	
	/** \brief Add listener. */
	void AddListener( igdeEditPathListener *listener );
	
	/** \brief Remove listener. */
	void RemoveListener( igdeEditPathListener *listener );
	
	/** \brief Notify listeners path changed. */
	virtual void NotifyEditPathChanged();
	/*@}*/
};

#endif
