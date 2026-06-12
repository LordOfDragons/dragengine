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
		
		explicit inline ClipboardData(const decStringSet &value) : igdeTClipboardData<decStringSet>(TypeName, value){}
		explicit inline ClipboardData(decStringSet &&value) : igdeTClipboardData<decStringSet>(TypeName, value){}
		
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
	class DE_DLL_EXPORT ActionAdd : public igdeAction{
	protected:
		igdeMetaPropertyStringSet &pProperty;
		const ContextRef pContext;
		igdeWidget &pOwner;
		
	public:
		using Ref = deTObjectReference<ActionAdd>;
		ActionAdd(igdeMetaPropertyStringSet &property, const ContextRef &context,
			igdeWidget &owner);
		void OnAction() override;
		void Update() override;
	};
	
	/** \brief Remove selected entries action. */
	class DE_DLL_EXPORT ActionRemove : public igdeAction{
	protected:
		igdeMetaPropertyStringSet &pProperty;
		const ContextRef pContext;
		igdeEnvironment &pEnvironment;
		
	public:
		using Ref = deTObjectReference<ActionRemove>;
		ActionRemove(igdeMetaPropertyStringSet &property, const ContextRef &context,
			igdeEnvironment &environment);
		void OnAction() override;
		void Update() override;
	};
	
	/** \brief Remove all entries action. */
	class DE_DLL_EXPORT ActionRemoveAll : public igdeAction{
	protected:
		igdeMetaPropertyStringSet &pProperty;
		const ContextRef pContext;
		igdeEnvironment &pEnvironment;
		
	public:
		using Ref = deTObjectReference<ActionRemoveAll>;
		ActionRemoveAll(igdeMetaPropertyStringSet &property, const ContextRef &context,
			igdeEnvironment &environment);
		void OnAction() override;
		void Update() override;
	};
	
	/** \brief Export to text action. */
	class DE_DLL_EXPORT ActionExportAsText : public igdeAction{
	protected:
		igdeMetaPropertyStringSet &pProperty;
		const ContextRef pContext;
		igdeWidget &pOwner;
		
	public:
		using Ref = deTObjectReference<ActionExportAsText>;
		ActionExportAsText(igdeMetaPropertyStringSet &property,
			const ContextRef &context, igdeWidget &owner);
		void OnAction() override;
		void Update() override;
	};
	
	/** \brief Import from text action. */
	class DE_DLL_EXPORT ActionImportFromText : public igdeAction{
	protected:
		igdeMetaPropertyStringSet &pProperty;
		const ContextRef pContext;
		igdeWidget &pOwner;
		
	public:
		using Ref = deTObjectReference<ActionImportFromText>;
		ActionImportFromText(igdeMetaPropertyStringSet &property,
			const ContextRef &context, igdeWidget &owner);
		void OnAction() override;
		void Update() override;
	};
	
	
private:
	int pRows;
	bool pMultiSelection;
	igdeTListenerList<Listener> pListeners;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create string set meta property with label and description. */
	igdeMetaPropertyStringSet(const char *id, const char *name, const char *description);
	
protected:
	/** \brief Clean up string set meta property. */
	~igdeMetaPropertyStringSet() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
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
	deTObjectReference<igdeMetaPropertyStringSetUndo> ChangePropertyValue(
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
	virtual void GetStringItemInfo(const decString &string, igdeMetaContextItemInfo &info) const;
	
	/**
	 * \brief Valid set of strings.
	 */
	virtual decStringSet GetValidStrings(const ContextRef &context) const;
	
	/**
	 * \brief Add context menu entries for adding new entries to the set.
	 * 
	 * Subclasses can override this method to add custom context menu entries.
	 * Default implementation calls AddDefaultContextMenuEntries().
	 */
	virtual void AddContextMenuEntriesAdd(igdeMenuCascade &contextMenu,
		const ContextRef &context, igdeWidget &owner);
	
	/**
	 * \brief Create action for target button.
	 * 
	 * Subclasses can override this method to create button action. If nullptr is returned
	 * the button is destroyed. Default implementation calls CreateDefaultButtonAction().
	 */
	virtual igdeAction::Ref CreateButtonAction(TargetButton target,
		const ContextRef &context, igdeWidget &owner);
	
	
	/**
	 * \brief Create UI widget.
	 *
	 * This object is able to add itself to a widget holder in the appropriate way.
	 */
	deTObjectReference<igdeMetaPropertyWidget> CreateWidget(const ContextRef &context) override;
	
	
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
	igdeAction::Ref CreateDefaultButtonAction(TargetButton target,
		const ContextRef &context, igdeWidget &owner);
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
