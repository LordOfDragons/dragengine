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

#ifndef _IGDEMETAPROPERTYLIST_H_
#define _IGDEMETAPROPERTYLIST_H_

#include "igdeMetaProperty.h"
#include "storage/igdeMetaPropertyStorageList.h"
#include "../../clipboard/igdeClipboardData.h"
#include "../../gui/event/igdeAction.h"
#include "../../gui/model/igdeListItem.h"

class igdeMetaContextItemInfo;
class igdeMetaPropertyListUndo;

/**
 * \brief List meta property.
 * 
 * Includes active object and object selection if supported.
 */
class DE_DLL_EXPORT igdeMetaPropertyList : public igdeMetaProperty{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyList>;
	
	/** \brief List of objects. */
	using List = decTObjectOrderedSet<deObject>;
	
	/** \brief Object reference. */
	using ObjectRef = deTObjectReference<deObject>;
	
	/** \brief Selection of objects. */
	using SelectionSet = decTObjectSet<deObject>;
	
	
	/** \brief Target button. */
	enum class TargetButton{
		/** \brief Add entry. */
		add,
		
		/** \brief Remove entry. */
		remove,
		
		/** \brief Move entry up. */
		moveUp,
		
		/** \brief Move entry down. */
		moveDown
	};
	
	
	/** \brief Clipboard data. */
	class DE_DLL_EXPORT ClipboardData : public igdeTClipboardData<List>{
	public:
		using Ref = deTObjectReference<ClipboardData>;
		
		/** \brief Type name. */
		static constexpr const char *TypeName = "MetaProperty.List";
		
		explicit ClipboardData(const igdeMetaProperty &property, const List &value) :
			igdeTClipboardData<List>(property.GetClipboardDataTypeName(), value){}
		
		explicit ClipboardData(const igdeMetaProperty &property, List &&value) :
			igdeTClipboardData<List>(property.GetClipboardDataTypeName(), std::move(value)){}
		
		explicit ClipboardData(const char *typeName, const List &value) :
			igdeTClipboardData<List>(typeName, value){}
		
		explicit ClipboardData(const char *typeName, List &&value) :
			igdeTClipboardData<List>(typeName, std::move(value)){}
		
	protected:
		/** \brief Clean up object. */
		~ClipboardData() override = default;
	};
	
	
	/** \brief Listener. */
	class DE_DLL_EXPORT Listener : public TListener<igdeMetaPropertyList>{
	public:
		/** \brief Active object changed. */
		virtual void OnActiveChanged(igdeMetaPropertyList *property, const ContextRef &context);
		
		/** \brief Object selection changed. */
		virtual void OnSelectionChanged(igdeMetaPropertyList *property, const ContextRef &context);
		
		/** \brief Object item information changed. */
		virtual void OnObjectItemInfoChanged(igdeMetaPropertyList *property, const ContextRef &context);
	};
	
	
	/** \brief Duplicate selected entries action. */
	class DE_DLL_EXPORT ActionDuplicate : public Action{
	private:
		igdeMetaPropertyList &pPropertyList;
		
	public:
		using Ref = deTObjectReference<ActionDuplicate>;
		ActionDuplicate(igdeMetaPropertyList &property, igdeWidget &owner, const ContextRef &context = {});
		void OnAction() override;
		void Update() override;
		
		inline igdeMetaPropertyList &GetPropertyList() const{ return pPropertyList; }
	};
	
	/** \brief Remove selected entries action. */
	class DE_DLL_EXPORT ActionRemove : public Action{
	private:
		igdeMetaPropertyList &pPropertyList;
		
	public:
		using Ref = deTObjectReference<ActionRemove>;
		ActionRemove(igdeMetaPropertyList &property, igdeWidget &owner, const ContextRef &context = {});
		void OnAction() override;
		void Update() override;
		
		inline igdeMetaPropertyList &GetPropertyList() const{ return pPropertyList; }
	};
	
	/** \brief Remove all entries action. */
	class DE_DLL_EXPORT ActionRemoveAll : public Action{
	private:
		igdeMetaPropertyList &pPropertyList;
		
	public:
		using Ref = deTObjectReference<ActionRemoveAll>;
		ActionRemoveAll(igdeMetaPropertyList &property, igdeWidget &owner, const ContextRef &context = {});
		void OnAction() override;
		void Update() override;
		
		inline igdeMetaPropertyList &GetPropertyList() const{ return pPropertyList; }
	};
	
	/** \brief Move entry up action. */
	class DE_DLL_EXPORT ActionMoveUp : public Action{
	private:
		igdeMetaPropertyList &pPropertyList;
		bool pTop;
		
	public:
		using Ref = deTObjectReference<ActionMoveUp>;
		ActionMoveUp(igdeMetaPropertyList &property, igdeWidget &owner, const ContextRef &context = {});
		void OnAction() override;
		void Update() override;
		
		inline igdeMetaPropertyList &GetPropertyList() const{ return pPropertyList; }
		inline bool GetTop() const{ return pTop; }
		void SetTop(bool top);
	};
	
	/** \brief Move entry top action. */
	class DE_DLL_EXPORT ActionMoveTop : public ActionMoveUp{
	public:
		using Ref = deTObjectReference<ActionMoveTop>;
		ActionMoveTop(igdeMetaPropertyList &property, igdeWidget &owner, const ContextRef &context = {});
	};
	
	/** \brief Move entry down action. */
	class DE_DLL_EXPORT ActionMoveDown : public Action{
	private:
		igdeMetaPropertyList &pPropertyList;
		bool pBottom;
		
	public:
		using Ref = deTObjectReference<ActionMoveDown>;
		ActionMoveDown(igdeMetaPropertyList &property, igdeWidget &owner, const ContextRef &context = {});
		void OnAction() override;
		void Update() override;
		
		inline igdeMetaPropertyList &GetPropertyList() const{ return pPropertyList; }
		inline bool GetBottom() const{ return pBottom; }
		void SetBottom(bool bottom);
	};
	
	/** \brief Move entry bottom action. */
	class DE_DLL_EXPORT ActionMoveBottom : public ActionMoveDown{
	public:
		using Ref = deTObjectReference<ActionMoveBottom>;
		ActionMoveBottom(igdeMetaPropertyList &property, igdeWidget &owner, const ContextRef &context = {});
	};
	
	/** \brief Copy to clipboard action. */
	class DE_DLL_EXPORT ActionCopy : public Action{
	protected:
		igdeMetaPropertyList &pPropertyList;
		
	public:
		using Ref = deTObjectReference<ActionCopy>;
		ActionCopy(igdeMetaPropertyList &property, igdeWidget &owner, const ContextRef &context = {});
		void OnAction() override;
		void Update() override;
	};
	
	/** \brief Cut to clipboard action. */
	class DE_DLL_EXPORT ActionCut : public ActionRemove{
	protected:
		ActionCopy::Ref pActionCopy;
		
	public:
		using Ref = deTObjectReference<ActionCut>;
		ActionCut(igdeMetaPropertyList &property, igdeWidget &owner, const ContextRef &context = {});
		void OnAction() override;
	};
	
	/** \brief Paste from clipboard action. */
	class DE_DLL_EXPORT ActionPaste : public Action{
	protected:
		igdeMetaPropertyList &pPropertyList;
		
	public:
		using Ref = deTObjectReference<ActionPaste>;
		ActionPaste(igdeMetaPropertyList &property, igdeWidget &owner, const ContextRef &context = {});
		void OnAction() override;
		void Update() override;
	};
	
	
private:
	int pRows = 4;
	bool pMultiSelection = false;
	bool pSorted = false;
	igdeTListenerList<Listener> pListeners;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	
	/** \brief Create object meta property with label and description. */
	igdeMetaPropertyList(const char *id, const char *name, const char *description);
	
	/** \brief Create object meta property with label, description, filter and undo info set from translation tag. */
	igdeMetaPropertyList(const char *id, const char *translationTag);
	
protected:
	/** \brief Clean up object meta property. */
	~igdeMetaPropertyList() override;
	
public:
	/*@}*/
	
	
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
	
	/** \brief Sorted. */
	inline bool GetSorted() const{ return pSorted; }
	
	/** \brief Set sorted. */
	void SetSorted(bool sorted);
	
	
	/** \brief Listeners. */
	inline igdeTListenerList<Listener> &GetListeners(){ return pListeners; }
	inline const igdeTListenerList<Listener> &GetListeners() const{ return pListeners; }
	
	/** \brief Notify listeners about value changed. */
	void NotifyValueChanged(const ContextRef &context);
	
	/** \brief Notify listeners about active object changed. */
	void NotifyActiveChanged(const ContextRef &context);
	
	/** \brief Notify listeners about object selection changed. */
	void NotifySelectionChanged(const ContextRef &context);
	
	/** \brief Notify listeners about object information changed. */
	void NotifyObjectItemInfoChanged(const ContextRef &context);
	
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
	virtual List GetPropertyValue(const ContextRef &context) const = 0;
	
	/**
	 * \brief Set property value matching context.
	 *
	 * Implemented by subclass.
	 */
	virtual void SetPropertyValue(const ContextRef &context, const List &value) = 0;
	
	/**
	 * \brief Change property value matching context with undo support.
	 *
	 * If the context has an undo system the change is recorded as an undo action.
	 * Otherwise SetPropertyValue() is called directly.
	 */
	virtual deTObjectReference<igdeMetaPropertyListUndo> ChangePropertyValue(
		const ContextRef &context, const List &newValue,
		const char *undoInfo = nullptr, const char *undoInfoLong = nullptr);
	
	/**
	 * \brief Get active object or nullptr if no active object.
	 */
	virtual ObjectRef GetActiveObject(const ContextRef &context) const = 0;
	
	/**
	 * \brief Set active object.
	 */
	virtual void SetActiveObject(const ContextRef &context, const ObjectRef &activeObject) = 0;
	
	/**
	 * \brief Get object selection.
	 */
	virtual SelectionSet GetSelection(const ContextRef &context) const;
	
	/**
	 * \brief Set object selection.
	 */
	virtual void SetSelection(const ContextRef &context, const SelectionSet &selection);
	
	/**
	 * \brief Get object item information.
	 */
	virtual void GetObjectItemInfo(const ContextRef &context, const ObjectRef &object,
		igdeMetaContextItemInfo &info) const = 0;
	
	/**
	 * \brief Create action for target button.
	 * 
	 * Subclasses can override this method to create button action. If nullptr is returned
	 * the button is destroyed.
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
	 * \brief Create copy of object if possible.
	 * 
	 * Subclass can override this method to create a copy of the object. The list parameter
	 * contains all existing objects. This list can be used to enforce rules, for example unique
	 * name or limit counts. If nullptr is returned the object cannot be copied.
	 */
	virtual ObjectRef CopyObject(const ContextRef &context, const List &existingObjects,
		const ObjectRef &object) const;
	
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
	 * - ActionDuplicate
	 * - ActionRemove
	 * - ActionRemoveAll
	 * - ActionMoveUp
	 * - ActionMoveDown
	 * - ActionMoveTop
	 * - ActionMoveBottom
	 */
	void AddDefaultContextMenuEntries(igdeMenuCascade &contextMenu,
		const ContextRef &context, igdeWidget &owner);
	
	/** \brief Create default button action. */
	Action::Ref CreateDefaultButtonAction(TargetButton target, igdeWidget &owner);
	/*@}*/
};



/**
 * \brief Typed list meta property.
 */
template<typename T, typename ListType = decTObjectOrderedSet<T>>
class igdeMetaPropertyListType : public igdeMetaPropertyList{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyListType<T>>;
	
	/** \brief Object type reference. */
	using ObjectTypeRef = deTObjectReference<T>;
	
	/** \brief Selection of object type reference. */
	using SelectionSetType = decTObjectSet<T>;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list meta property with label and description. */
	igdeMetaPropertyListType(const char *id, const char *name, const char *description) :
	igdeMetaPropertyList(id, name, description){
	}
	
	/** \brief Create meta property with label, description, filter and undo info set from translation tag. */
	igdeMetaPropertyListType(const char *id, const char *translationTag) :
	igdeMetaPropertyList(id, translationTag){
	}
	
protected:
	/** \brief Clean up list meta property. */
	~igdeMetaPropertyListType() override = default;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** Convert list. */
	List ConvertList(const ListType &in) const{
		List out;
		in.Visit([&](const ObjectTypeRef &object){
			out.Add(object.Pointer());
		});
		return out;
	}
	
	/** Convert list. */
	ListType ConvertList(const List &in) const{
		ListType out;
		in.Visit([&](const ObjectRef &object){
			out.Add(object.DynamicCast<T>());
		});
		return out;
	}
	
	/** Convert set. */
	SelectionSet ConvertSet(const SelectionSetType &in) const{
		SelectionSet out;
		in.Visit([&](const ObjectTypeRef &object){
			out.Add(object.Pointer());
		});
		return out;
	}
	
	/** Convert set. */
	SelectionSetType ConvertSet(const SelectionSet &in) const{
		SelectionSetType out;
		in.Visit([&](const ObjectRef &object){
			out.Add(object.DynamicCast<T>());
		});
		return out;
	}
	
	List GetPropertyValue(const ContextRef &context) const override{
		return ConvertList(GetPropertyValueType(context));
	}
	
	void SetPropertyValue(const ContextRef &context, const List &value) override{
		SetPropertyValueType(context, ConvertList(value));
	}
	
	/**
	 * \brief Change property value matching context with undo support.
	 *
	 * If the context has an undo system the change is recorded as an undo action.
	 * Otherwise SetPropertyValue() is called directly.
	 */
	deTObjectReference<igdeMetaPropertyListUndo> ChangePropertyValueType(
	const ContextRef &context, const ListType &newValue,
	const char *undoInfo = nullptr, const char *undoInfoLong = nullptr){
		return ChangePropertyValue(context, ConvertList(newValue), undoInfo, undoInfoLong);
	}
	
	ObjectRef GetActiveObject(const ContextRef &context) const override{
		return GetActiveObjectType(context);
	}
	
	void SetActiveObject(const ContextRef &context, const ObjectRef &activeObject) override{
		SetActiveObjectType(context, activeObject.DynamicCast<T>());
	}
	
	SelectionSet GetSelection(const ContextRef &context) const override{
		return ConvertSet(GetSelectionType(context));
	}
	
	void SetSelection(const ContextRef &context, const SelectionSet &selection) override{
		SetSelectionType(context, ConvertSet(selection));
	}
	
	void GetObjectItemInfo(const ContextRef &context, const ObjectRef &object,
	igdeMetaContextItemInfo &info) const override{
		GetObjectItemInfoType(context, object.DynamicCast<T>(), info);
	}
	
	ObjectRef CopyObject(const ContextRef &context, const List &existingObjects,
	const ObjectRef &object) const override{
		return CopyObjectType(context, ConvertList(existingObjects), object.DynamicCast<T>());
	}
	
	/**
	 * \brief Get property value matching context.
	 * 
	 * Implemented by subclass.
	 */
	virtual ListType GetPropertyValueType(const ContextRef &context) const = 0;
	
	/**
	 * \brief Set property value matching context.
	 * 
	 * Implemented by subclass.
	 */
	virtual void SetPropertyValueType(const ContextRef &context, const ListType &value) = 0;
	
	/**
	 * \brief Get active object or nullptr if no active object.
	 */
	virtual ObjectTypeRef GetActiveObjectType(const ContextRef &context) const = 0;
	
	/**
	 * \brief Set active object.
	 */
	virtual void SetActiveObjectType(const ContextRef &context, const ObjectTypeRef &activeObject) = 0;
	
	/**
	 * \brief Get object selection.
	 */
	virtual SelectionSetType GetSelectionType(const ContextRef &context) const{
		auto active = GetActiveObjectType(context);
		return active ? SelectionSetType(devctag, active) : SelectionSetType();
	}
	
	/**
	 * \brief Set object selection.
	 */
	virtual void SetSelectionType(const ContextRef &context, const SelectionSetType &selection){
	}
	
	/**
	 * \brief Get object item information.
	 */
	virtual void GetObjectItemInfoType(const ContextRef &context, const ObjectTypeRef &object,
		igdeMetaContextItemInfo &info) const = 0;
	
	/**
	 * \brief Create copy of object if possible.
	 * 
	 * Subclass can override this method to create a copy of the object. The list parameter
	 * contains all existing objects. This list can be used to enforce rules, for example unique
	 * name or limit counts. If nullptr is returned the object cannot be copied.
	 */
	virtual ObjectTypeRef CopyObjectType(const ContextRef &context,
	const ListType &existingObjects, const ObjectTypeRef &object) const{
		return {};
	}
	/*@}*/
};


/**
 * \brief List meta property using storage.
 */
template<typename T, typename ListType = decTObjectOrderedSet<T>>
class igdeMetaPropertyListStorage : public igdeMetaPropertyListType<T, ListType>{
public:
	/** \brief Storage type. */
	using Storage = igdeMetaPropertyStorageList<T, igdeMetaPropertyListStorage<T, ListType>, ListType>;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list meta property with label and description. */
	igdeMetaPropertyListStorage(const char *id, const char *name, const char *description) :
	igdeMetaPropertyListType<T, ListType>(id, name, description){}
	
	/** \brief Create meta property with label, description, filter and undo info set from translation tag. */
	igdeMetaPropertyListStorage(const char *id, const char *translationTag) :
	igdeMetaPropertyListType<T, ListType>(id, translationTag){}
	
protected:
	/** \brief Clean up list meta property. */
	~igdeMetaPropertyListStorage() override = default;
	
public:
	/*@}*/
	/** \brief Storage. */
	virtual Storage &GetStorage(const igdeMetaProperty::ContextRef &context) const = 0;
	
	
	ListType GetPropertyValueType(
	const igdeMetaProperty::ContextRef &context) const override{
		return GetStorage(context).GetValue();
	}
	
	void SetPropertyValueType(const igdeMetaProperty::ContextRef &context,
	const ListType &value) override{
		GetStorage(context).SetValue(value);
	}
	
	typename igdeMetaPropertyListType<T, ListType>::ObjectTypeRef GetActiveObjectType(
	const igdeMetaProperty::ContextRef &context) const override{
		return GetStorage(context).GetActive();
	}
	
	void SetActiveObjectType(const igdeMetaProperty::ContextRef &context,
	const typename igdeMetaPropertyListType<T, ListType>::ObjectTypeRef &activeObject) override{
		GetStorage(context).SetActive(activeObject);
	}
	
	typename igdeMetaPropertyListType<T, ListType>::SelectionSetType GetSelectionType(
	const igdeMetaProperty::ContextRef &context) const override{
		return GetStorage(context).GetSelection();
	}
	
	void SetSelectionType(const igdeMetaProperty::ContextRef &context,
	const typename igdeMetaPropertyListType<T, ListType>::SelectionSetType &selection) override{
		GetStorage(context).SetSelection(selection);
	}
};

#endif
