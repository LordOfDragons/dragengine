/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _IGDEMETAPROPERTYSTRINGLIST_H_
#define _IGDEMETAPROPERTYSTRINGLIST_H_

#include "igdeMetaProperty.h"
#include "storage/igdeMetaPropertyStorageStringList.h"
#include "../igdeTMetaData.h"
#include "../igdeMetaContextItemInfo.h"
#include "../../clipboard/igdeClipboardData.h"
#include "../../gui/event/igdeAction.h"

#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/string/decStringSet.h>

class igdeMetaPropertyStringListUndo;


/**
 * \brief String list meta property.
 *
 * Ordered list of strings with reordering support. Includes active string
 * and string selection if supported.
 */
class DE_DLL_EXPORT igdeMetaPropertyStringList : public igdeMetaProperty{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyStringList>;
	
	/** \brief String reference. */
	using StringRef = igdeTMetaData<decString>::Ref;
	
	
	/** \brief Target button. */
	enum class TargetButton{
		/** \brief Add entry. */
		add,
		
		/** \brief Remove entry. */
		remove,
		
		/** \brief Move entry up. */
		moveUp,
		
		/** \brief Move entry down. */
		moveDown,
		
		/** \brief Move entry to top. */
		moveTop,
		
		/** \brief Move entry to bottom. */
		moveBottom,
	};
	
	
	/** \brief Clipboard data. */
	class DE_DLL_EXPORT ClipboardData : public igdeTClipboardData<decStringList>{
	public:
		using Ref = deTObjectReference<ClipboardData>;
		
		/** \brief Type name. */
		static constexpr const char *TypeName = "MetaProperty.StringList";
		
		explicit inline ClipboardData(const decStringList &value) : igdeTClipboardData<decStringList>(TypeName, value){}
		explicit inline ClipboardData(decStringList &&value) : igdeTClipboardData<decStringList>(TypeName, value){}
		
	protected:
		/** \brief Clean up object. */
		~ClipboardData() override = default;
	};
	
	
	/** \brief Listener. */
	class DE_DLL_EXPORT Listener : public TListener<igdeMetaPropertyStringList>{
	public:
		/** \brief Active string changed. */
		virtual void OnActiveChanged(igdeMetaPropertyStringList *property, const ContextRef &context);
		
		/** \brief String selection changed. */
		virtual void OnSelectionChanged(igdeMetaPropertyStringList *property, const ContextRef &context);
	};
	
	
	/** \brief Add entry action. */
	class DE_DLL_EXPORT ActionAdd : public Action{
	private:
		igdeMetaPropertyStringList &pPropertyStringList;
		
	public:
		using Ref = deTObjectReference<ActionAdd>;
		ActionAdd(igdeMetaPropertyStringList &property, igdeWidget &owner, const ContextRef &context = {});
		void OnAction() override;
		void Update() override;
		inline igdeMetaPropertyStringList &GetPropertyStringList() const{ return pPropertyStringList; }
	};
	
	/** \brief Remove selected entry action. */
	class DE_DLL_EXPORT ActionRemove : public Action{
	private:
		igdeMetaPropertyStringList &pPropertyStringList;
		
	public:
		using Ref = deTObjectReference<ActionRemove>;
		ActionRemove(igdeMetaPropertyStringList &property, igdeWidget &owner, const ContextRef &context = {});
		void OnAction() override;
		void Update() override;
		inline igdeMetaPropertyStringList &GetPropertyStringList() const{ return pPropertyStringList; }
	};
	
	/** \brief Remove all entries action. */
	class DE_DLL_EXPORT ActionRemoveAll : public Action{
	private:
		igdeMetaPropertyStringList &pPropertyStringList;
		
	public:
		using Ref = deTObjectReference<ActionRemoveAll>;
		ActionRemoveAll(igdeMetaPropertyStringList &property, igdeWidget &owner, const ContextRef &context = {});
		void OnAction() override;
		void Update() override;
		inline igdeMetaPropertyStringList &GetPropertyStringList() const{ return pPropertyStringList; }
	};
	
	/** \brief Move entry up action. */
	class DE_DLL_EXPORT ActionMoveUp : public Action{
	private:
		igdeMetaPropertyStringList &pPropertyStringList;
		bool pTop;
		
	public:
		using Ref = deTObjectReference<ActionMoveUp>;
		ActionMoveUp(igdeMetaPropertyStringList &property, igdeWidget &owner, const ContextRef &context = {});
		void OnAction() override;
		void Update() override;
		
		inline igdeMetaPropertyStringList &GetPropertyStringList() const{ return pPropertyStringList; }
		inline bool GetTop() const{ return pTop; }
		void SetTop(bool top);
	};
	
	/** \brief Move entry to top action. */
	class DE_DLL_EXPORT ActionMoveTop : public ActionMoveUp{
	public:
		using Ref = deTObjectReference<ActionMoveTop>;
		ActionMoveTop(igdeMetaPropertyStringList &property, igdeWidget &owner, const ContextRef &context = {});
	};
	
	/** \brief Move entry down action. */
	class DE_DLL_EXPORT ActionMoveDown : public Action{
	private:
		igdeMetaPropertyStringList &pPropertyStringList;
		bool pBottom;
		
	public:
		using Ref = deTObjectReference<ActionMoveDown>;
		ActionMoveDown(igdeMetaPropertyStringList &property, igdeWidget &owner, const ContextRef &context = {});
		void OnAction() override;
		void Update() override;
		
		inline igdeMetaPropertyStringList &GetPropertyStringList() const{ return pPropertyStringList; }
		inline bool GetBottom() const{ return pBottom; }
		void SetBottom(bool bottom);
	};
	
	/** \brief Move entry to bottom action. */
	class DE_DLL_EXPORT ActionMoveBottom : public ActionMoveDown{
	public:
		using Ref = deTObjectReference<ActionMoveBottom>;
		ActionMoveBottom(igdeMetaPropertyStringList &property, igdeWidget &owner, const ContextRef &context = {});
	};
	
	/** \brief Export to text action. */
	class DE_DLL_EXPORT ActionExportAsText : public Action{
	private:
		igdeMetaPropertyStringList &pPropertyStringList;
		
	public:
		using Ref = deTObjectReference<ActionExportAsText>;
		ActionExportAsText(igdeMetaPropertyStringList &property, igdeWidget &owner, const ContextRef &context = {});
		void OnAction() override;
		void Update() override;
		inline igdeMetaPropertyStringList &GetPropertyStringList() const{ return pPropertyStringList; }
	};
	
	/** \brief Import from text action. */
	class DE_DLL_EXPORT ActionImportFromText : public Action{
	private:
		igdeMetaPropertyStringList &pPropertyStringList;

	public:
		using Ref = deTObjectReference<ActionImportFromText>;
		ActionImportFromText(igdeMetaPropertyStringList &property, igdeWidget &owner, const ContextRef &context = {});
		void OnAction() override;
		void Update() override;
		inline igdeMetaPropertyStringList &GetPropertyStringList() const{ return pPropertyStringList; }
	};
	
	
private:
	decStringList pDefaultValue;
	int pRows;
	bool pMultiSelection;
	igdeTListenerList<Listener> pListeners;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create string list meta property with label and description. */
	igdeMetaPropertyStringList(const char *id, const char *name, const char *description);
	
protected:
	/** \brief Clean up string list meta property. */
	~igdeMetaPropertyStringList() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Default value. */
	inline const decStringList &GetDefaultValue() const{ return pDefaultValue; }
	
	/** \brief Set default value. */
	void SetDefaultValue(const decStringList &value);
	
	/** \brief Rows. */
	inline int GetRows() const{ return pRows; }
	
	/** \brief Set rows. */
	void SetRows(int rows);
	
	/** \brief Multi selection. */
	inline bool GetMultiSelection() const{ return pMultiSelection; }
	
	/** \brief Set multi selection. */
	void SetMultiSelection(bool multiSelection);
	
	
	/** \brief Listeners. */
	inline igdeTListenerList<Listener> &GetListeners(){ return pListeners; }
	inline const igdeTListenerList<Listener> &GetListeners() const{ return pListeners; }
	
	/** \brief Notify listeners about value changed. */
	void NotifyValueChanged(const ContextRef &context);
	
	/** \brief Notify listeners about active string changed. */
	void NotifyActiveChanged(const ContextRef &context);
	
	/** \brief Notify listeners about string selection changed. */
	void NotifySelectionChanged(const ContextRef &context);
	
	
	/**
	 * \brief Capture context.
	 *
	 * This returns an immutable context always returning the necessary state for getting or setting the property.
	 */
	virtual ContextRef Capture(const ContextRef &context) const = 0;
	
	/**
	 * \brief Property is valid.
	 *
	 * This means calling GetPropertyValue() nor SetPropertyValue() throws an exception.
	 */
	virtual bool IsValid(const ContextRef &context) const = 0;
	
	/**
	 * \brief Get property value matching context.
	 *
	 * Implemented by subclass.
	 */
	virtual decStringList GetPropertyValue(const ContextRef &context) const = 0;
	
	/**
	 * \brief Set property value matching context.
	 *
	 * Implemented by subclass.
	 */
	virtual void SetPropertyValue(const ContextRef &context, const decStringList &value) = 0;
	
	/**
	 * \brief Change property value matching context with undo support.
	 *
	 * If the context has an undo system the change is recorded as an undo action.
	 * Otherwise SetPropertyValue() is called directly.
	 */
	virtual deTObjectReference<igdeMetaPropertyStringListUndo> ChangePropertyValue(
		const ContextRef &context, const decStringList &newValue,
		const char *undoInfo = nullptr, const char *undoInfoLong = nullptr);
	
	/**
	 * \brief Get active string or empty string if no active string.
	 */
	virtual StringRef GetActiveString(const ContextRef &context) const = 0;
	
	/**
	 * \brief Set active string.
	 */
	virtual void SetActiveString(const ContextRef &context, const StringRef &activeString) = 0;
	
	/**
	 * \brief Get string selection.
	 */
	virtual decStringList GetSelection(const ContextRef &context) const;
	
	/**
	 * \brief Set string selection.
	 */
	virtual void SetSelection(const ContextRef &context, const decStringList &selection);
	
	/**
	 * \brief Get string item information.
	 */
	virtual void GetStringItemInfo(const ContextRef &context, const decString &string,
		igdeMetaContextItemInfo &info) const;
	
	/**
	 * \brief Valid set of strings for adding.
	 */
	virtual decStringSet GetAllowedStrings(const ContextRef &context) const;
	
	/**
	 * \brief Create action for target button.
	 *
	 * Subclasses can override this method to create button action. If nullptr is returned
	 * the button is destroyed. Default implementation calls CreateDefaultButtonAction().
	 */
	virtual Action::Ref CreateButtonAction(TargetButton target, igdeWidget &owner);
	
	/**
	 * \brief Add context menu entries.
	 *
	 * Subclasses can override this method to add custom context menu entries.
	 */
	void AddContextMenuEntries(igdeMenuCascade &contextMenu,
		const ContextRef &context, igdeWidget &owner) override;
	
	
	/**
	 * \brief Create UI widget.
	 *
	 * This object is able to add itself to a widget holder in the appropriate way.
	 */
	deTObjectReference<igdeMetaPropertyWidget> CreateWidget() override;
	
	
	/**
	 * \brief Add default context menu entries.
	 *
	 * Add first context menu entries to add list entries then call AddDefaultContextMenuEntries().
	 * This adds the following actions:
	 * - ActionRemove
	 * - ActionRemoveAll
	 * - ActionMoveUp
	 * - ActionMoveDown
	 * - ActionMoveTop
	 * - ActionMoveBottom
	 * - ActionExportAsText
	 * - ActionImportFromText
	 */
	void AddDefaultContextMenuEntries(igdeMenuCascade &contextMenu,
		const ContextRef &context, igdeWidget &owner);
	
	/** \brief Create default button action. */
	Action::Ref CreateDefaultButtonAction(TargetButton target, igdeWidget &owner);
	/*@}*/
};


/**
 * \brief String list meta property using storage.
 */
class DE_DLL_EXPORT igdeMetaPropertyStringListStorage : public igdeMetaPropertyStringList{
public:
	/** \brief Storage type. */
	using Storage = igdeMetaPropertyStorageStringList<igdeMetaPropertyStringListStorage>;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create string list meta property with label and description. */
	igdeMetaPropertyStringListStorage(const char *id, const char *name, const char *description);
	
protected:
	/** \brief Clean up string list meta property. */
	~igdeMetaPropertyStringListStorage() override;
	
public:
	/*@}*/
	/** \brief Storage. */
	virtual Storage &GetStorage(const ContextRef &context) const = 0;
	
	
	decStringList GetPropertyValue(const ContextRef &context) const override;
	void SetPropertyValue(const ContextRef &context, const decStringList &value) override;
	StringRef GetActiveString(const ContextRef &context) const override;
	void SetActiveString(const ContextRef &context, const StringRef &activeString) override;
	decStringList GetSelection(const ContextRef &context) const override;
	void SetSelection(const ContextRef &context, const decStringList &selection) override;
};

#endif
