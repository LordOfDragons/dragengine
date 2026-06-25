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

#ifndef _IGDEMETAPROPERTYSTRINGSET_H_
#define _IGDEMETAPROPERTYSTRINGSET_H_

#include "igdeMetaProperty.h"
#include "storage/igdeMetaPropertyStorageStringSet.h"
#include "../igdeTMetaData.h"
#include "../igdeMetaContextItemInfo.h"
#include "../../clipboard/igdeClipboardData.h"
#include "../../gui/event/igdeAction.h"

#include <dragengine/common/string/decStringSet.h>

class igdeMetaPropertyStringSetUndo;


/**
 * \brief String set meta property.
 *
 * Includes active string and string selection if supported.
 */
class DE_DLL_EXPORT igdeMetaPropertyStringSet : public igdeMetaProperty{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyStringSet>;
	
	/** \brief String reference. */
	using StringRef = igdeTMetaData<decString>::Ref;
	
	
	/** \brief Target button. */
	enum class TargetButton{
		/** \brief Add entry. */
		add,
		
		/** \brief Remove entry. */
		remove
	};
	
	
	/** \brief Clipboard data. */
	class DE_DLL_EXPORT ClipboardData : public igdeTClipboardData<decStringSet>{
	public:
		using Ref = deTObjectReference<ClipboardData>;
		
		/** \brief Type name. */
		static constexpr const char *TypeName = "MetaProperty.StringSet";
		
		explicit ClipboardData(const igdeMetaProperty &property, const decStringSet &value) :
			igdeTClipboardData<decStringSet>(property.GetClipboardDataTypeName(), value){}
		
		explicit ClipboardData(const igdeMetaProperty &property, decStringSet &&value) :
			igdeTClipboardData<decStringSet>(property.GetClipboardDataTypeName(), std::move(value)){}
		
		explicit ClipboardData(const char *typeName, const decStringSet &value) :
			igdeTClipboardData<decStringSet>(typeName, value){}
		
		explicit ClipboardData(const char *typeName, decStringSet &&value) :
			igdeTClipboardData<decStringSet>(typeName, std::move(value)){}
		
	protected:
		/** \brief Clean up object. */
		~ClipboardData() override = default;
	};
	
	
	/** \brief Listener. */
	class DE_DLL_EXPORT Listener : public TListener<igdeMetaPropertyStringSet>{
	public:
		/** \brief Active string changed. */
		virtual void OnActiveChanged(igdeMetaPropertyStringSet *property, const ContextRef &context);
		
		/** \brief String selection changed. */
		virtual void OnSelectionChanged(igdeMetaPropertyStringSet *property, const ContextRef &context);
	};
	
	
	/** \brief Add entries action. */
	class DE_DLL_EXPORT ActionAdd : public Action{
	private:
		igdeMetaPropertyStringSet &pPropertyStringSet;
		
	public:
		using Ref = deTObjectReference<ActionAdd>;
		ActionAdd(igdeMetaPropertyStringSet &property, igdeWidget &owner, const ContextRef &context = {});
		void OnAction() override;
		void Update() override;
		inline igdeMetaPropertyStringSet &GetPropertyStringSet() const{ return pPropertyStringSet; }
	};
	
	/** \brief Remove selected entries action. */
	class DE_DLL_EXPORT ActionRemove : public Action{
	private:
		igdeMetaPropertyStringSet &pPropertyStringSet;
		
	public:
		using Ref = deTObjectReference<ActionRemove>;
		ActionRemove(igdeMetaPropertyStringSet &property, igdeWidget &owner, const ContextRef &context = {});
		void OnAction() override;
		void Update() override;
		inline igdeMetaPropertyStringSet &GetPropertyStringSet() const{ return pPropertyStringSet; }
	};
	
	/** \brief Remove all entries action. */
	class DE_DLL_EXPORT ActionRemoveAll : public Action{
	private:
		igdeMetaPropertyStringSet &pPropertyStringSet;
		
	public:
		using Ref = deTObjectReference<ActionRemoveAll>;
		ActionRemoveAll(igdeMetaPropertyStringSet &property, igdeWidget &owner, const ContextRef &context = {});
		void OnAction() override;
		void Update() override;
		inline igdeMetaPropertyStringSet &GetPropertyStringSet() const{ return pPropertyStringSet; }
	};
	
	/** \brief Export to text action. */
	class DE_DLL_EXPORT ActionExportAsText : public Action{
	private:
		igdeMetaPropertyStringSet &pPropertyStringSet;
		
	public:
		using Ref = deTObjectReference<ActionExportAsText>;
		ActionExportAsText(igdeMetaPropertyStringSet &property, igdeWidget &owner, const ContextRef &context = {});
		void OnAction() override;
		void Update() override;
		inline igdeMetaPropertyStringSet &GetPropertyStringSet() const{ return pPropertyStringSet; }
	};
	
	/** \brief Import from text action. */
	class DE_DLL_EXPORT ActionImportFromText : public Action{
	private:
		igdeMetaPropertyStringSet &pPropertyStringSet;
		
	public:
		using Ref = deTObjectReference<ActionImportFromText>;
		ActionImportFromText(igdeMetaPropertyStringSet &property, igdeWidget &owner, const ContextRef &context = {});
		void OnAction() override;
		void Update() override;
		inline igdeMetaPropertyStringSet &GetPropertyStringSet() const{ return pPropertyStringSet; }
	};
	
	/** \brief Copy to clipboard action. */
	class ActionCopy : public Action{
	protected:
		igdeMetaPropertyStringSet &pPropertyStringSet;
		bool pSelection;
		
	public:
		using Ref = deTObjectReference<ActionCopy>;
		ActionCopy(igdeMetaPropertyStringSet &property, igdeWidget &owner, const ContextRef &context = {});
		void OnAction() override;
		void Update() override;
	};
	
	/** \brief Copy selection to clipboard action. */
	class ActionCopySelection : public ActionCopy{
	public:
		using Ref = deTObjectReference<ActionCopySelection>;
		ActionCopySelection(igdeMetaPropertyStringSet &property, igdeWidget &owner, const ContextRef &context = {});
	};
	
	/** \brief Cut to clipboard action. */
	class ActionCut : public ActionRemove{
	protected:
		ActionCopy::Ref pActionCopy;
		
	public:
		using Ref = deTObjectReference<ActionCut>;
		ActionCut(igdeMetaPropertyStringSet &property, igdeWidget &owner, const ContextRef &context = {});
		void OnAction() override;
	};
	
	/** \brief Paste from clipboard action. */
	class ActionPaste : public Action{
	protected:
		igdeMetaPropertyStringSet &pPropertyStringSet;
		bool pAppend;
		
	public:
		using Ref = deTObjectReference<ActionPaste>;
		ActionPaste(igdeMetaPropertyStringSet &property, igdeWidget &owner, const ContextRef &context = {});
		void OnAction() override;
		void Update() override;
	};
	
	/** \brief Paste and append from clipboard action. */
	class ActionPasteAppend : public ActionPaste{
	public:
		using Ref = deTObjectReference<ActionPasteAppend>;
		ActionPasteAppend(igdeMetaPropertyStringSet &property, igdeWidget &owner, const ContextRef &context = {});
	};
	
	
private:
	decStringSet pDefaultValue;
	int pRows;
	bool pMultiSelection;
	igdeTListenerList<Listener> pListeners;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create string set meta property with label and description. */
	igdeMetaPropertyStringSet(const char *id, const char *name, const char *description);
	
	/** \brief Create string set meta property with label, description, filter and undo info set from translation tag. */
	igdeMetaPropertyStringSet(const char *id, const char *translationTag);
	
protected:
	/** \brief Clean up string set meta property. */
	~igdeMetaPropertyStringSet() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Default value. */
	inline const decStringSet &GetDefaultValue() const{ return pDefaultValue; }
	
	/** \brief Set default value. */
	void SetDefaultValue(const decStringSet &value);
	
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
	virtual decStringSet GetPropertyValue(const ContextRef &context) const = 0;
	
	/**
	 * \brief Set property value matching context.
	 *
	 * Implemented by subclass.
	 */
	virtual void SetPropertyValue(const ContextRef &context, const decStringSet &value) = 0;
	
	/**
	 * \brief Change property value matching context with undo support.
	 *
	 * If the context has an undo system the change is recorded as an undo action.
	 * Otherwise SetPropertyValue() is called directly.
	 */
	virtual deTObjectReference<igdeMetaPropertyStringSetUndo> ChangePropertyValue(
		const ContextRef &context, const decStringSet &newValue,
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
	virtual decStringSet GetSelection(const ContextRef &context) const;
	
	/**
	 * \brief Set string selection.
	 */
	virtual void SetSelection(const ContextRef &context, const decStringSet &selection);
	
	/**
	 * \brief Get string item information.
	 */
	virtual void GetStringItemInfo(const ContextRef &context, const decString &string,
		igdeMetaContextItemInfo &info) const;
	
	/**
	 * \brief Valid set of strings.
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
 * \brief String set meta property using storage.
 */
class DE_DLL_EXPORT igdeMetaPropertyStringSetStorage : public igdeMetaPropertyStringSet{
public:
	/** \brief Storage type. */
	using Storage = igdeMetaPropertyStorageStringSet<igdeMetaPropertyStringSetStorage>;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create string set meta property with label and description. */
	igdeMetaPropertyStringSetStorage(const char *id, const char *name, const char *description);
	
	/** \brief Create meta property with label, description, filter and undo info set from translation tag. */
	igdeMetaPropertyStringSetStorage(const char *id, const char *translationTag);
	
protected:
	/** \brief Clean up string set meta property. */
	~igdeMetaPropertyStringSetStorage() override;
	
public:
	/*@}*/
	/** \brief Storage. */
	virtual Storage &GetStorage(const ContextRef &context) const = 0;
	
	
	decStringSet GetPropertyValue(const ContextRef &context) const override;
	void SetPropertyValue(const ContextRef &context, const decStringSet &value) override;
	StringRef GetActiveString(const ContextRef &context) const override;
	void SetActiveString(const ContextRef &context, const StringRef &activeString) override;
	decStringSet GetSelection(const ContextRef &context) const override;
	void SetSelection(const ContextRef &context, const decStringSet &selection) override;
};

#endif
