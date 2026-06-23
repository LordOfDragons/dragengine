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

#ifndef _IGDEMETAPROPERTYSET_H_
#define _IGDEMETAPROPERTYSET_H_

#include "igdeMetaProperty.h"
#include "storage/igdeMetaPropertyStorageSet.h"
#include "../../clipboard/igdeClipboardData.h"
#include "../../gui/event/igdeAction.h"

#include <dragengine/common/collection/decTSet.h>

class igdeMetaContextItemInfo;
class igdeMetaPropertySetUndo;

/**
 * \brief Set meta property.
 *
 * Includes active object and object selection if supported.
 */
class DE_DLL_EXPORT igdeMetaPropertySet : public igdeMetaProperty{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertySet>;
	
	/** \brief Set of objects. */
	using Set = decTObjectSet<deObject>;
	
	/** \brief Object reference. */
	using ObjectRef = deTObjectReference<deObject>;
	
	
	/** \brief Target button. */
	enum class TargetButton{
		/** \brief Add entry. */
		add,
		
		/** \brief Remove entry. */
		remove
	};
	
	
	/** \brief Clipboard data. */
	class DE_DLL_EXPORT ClipboardData : public igdeTClipboardData<Set>{
	public:
		using Ref = deTObjectReference<ClipboardData>;
		
		/** \brief Type name. */
		static constexpr const char *TypeName = "MetaProperty.Set";
		
		explicit ClipboardData(const igdeMetaProperty &property, const Set &value) :
			igdeTClipboardData<Set>(property.GetClipboardDataTypeName(), value){}
		
		explicit ClipboardData(const igdeMetaProperty &property, Set &&value) :
			igdeTClipboardData<Set>(property.GetClipboardDataTypeName(), std::move(value)){}
		
		explicit ClipboardData(const char *typeName, const Set &value) :
			igdeTClipboardData<Set>(typeName, value){}
		
		explicit ClipboardData(const char *typeName, Set &&value) :
			igdeTClipboardData<Set>(typeName, std::move(value)){}
		
	protected:
		/** \brief Clean up object. */
		~ClipboardData() override = default;
	};
	
	
	/** \brief Listener. */
	class DE_DLL_EXPORT Listener : public TListener<igdeMetaPropertySet>{
	public:
		/** \brief Active object changed. */
		virtual void OnActiveChanged(igdeMetaPropertySet *property, const ContextRef &context);
		
		/** \brief Object selection changed. */
		virtual void OnSelectionChanged(igdeMetaPropertySet *property, const ContextRef &context);
	};
	
	
	/** \brief Add entries action. */
	class DE_DLL_EXPORT ActionAdd : public Action{
	private:
		igdeMetaPropertySet &pPropertySet;
		
	public:
		using Ref = deTObjectReference<ActionAdd>;
		ActionAdd(igdeMetaPropertySet &property, igdeWidget &owner, const ContextRef &context = {});
		void OnAction() override;
		void Update() override;
		inline igdeMetaPropertySet &GetPropertySet() const{ return pPropertySet; }
	};
	
	/** \brief Remove selected entries action. */
	class DE_DLL_EXPORT ActionRemove : public Action{
	private:
		igdeMetaPropertySet &pPropertySet;
		
	public:
		using Ref = deTObjectReference<ActionRemove>;
		ActionRemove(igdeMetaPropertySet &property, igdeWidget &owner, const ContextRef &context = {});
		void OnAction() override;
		void Update() override;
		inline igdeMetaPropertySet &GetPropertySet() const{ return pPropertySet; }
	};
	
	/** \brief Remove all entries action. */
	class DE_DLL_EXPORT ActionRemoveAll : public Action{
	private:
		igdeMetaPropertySet &pPropertySet;
		
	public:
		using Ref = deTObjectReference<ActionRemoveAll>;
		ActionRemoveAll(igdeMetaPropertySet &property, igdeWidget &owner, const ContextRef &context = {});
		void OnAction() override;
		void Update() override;
		inline igdeMetaPropertySet &GetPropertySet() const{ return pPropertySet; }
	};
	
	
private:
	Set pDefaultValue;
	int pRows;
	bool pMultiSelection;
	igdeTListenerList<Listener> pListeners;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object meta property with label and description. */
	igdeMetaPropertySet(const char *id, const char *name, const char *description);
	
	/** \brief Create object meta property with label, description, filter and undo info set from translation tag. */
	igdeMetaPropertySet(const char *id, const char *translationTag);
	
protected:
	/** \brief Clean up object meta property. */
	~igdeMetaPropertySet() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Default value. */
	inline const Set &GetDefaultValue() const{ return pDefaultValue; }
	
	/** \brief Set default value. */
	void SetDefaultValue(const Set &value);
	
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
	
	/** \brief Notify listeners about active object changed. */
	void NotifyActiveChanged(const ContextRef &context);
	
	/** \brief Notify listeners about object selection changed. */
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
	virtual Set GetPropertyValue(const ContextRef &context) const = 0;
	
	/**
	 * \brief Set property value matching context.
	 *
	 * Implemented by subclass.
	 */
	virtual void SetPropertyValue(const ContextRef &context, const Set &value) = 0;
	
	/**
	 * \brief Change property value matching context with undo support.
	 *
	 * If the context has an undo system the change is recorded as an undo action.
	 * Otherwise SetPropertyValue() is called directly.
	 */
	virtual deTObjectReference<igdeMetaPropertySetUndo> ChangePropertyValue(
		const ContextRef &context, const Set &newValue,
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
	virtual Set GetSelection(const ContextRef &context) const;
	
	/**
	 * \brief Set object selection.
	 */
	virtual void SetSelection(const ContextRef &context, const Set &selection);
	
	/**
	 * \brief Get object item information.
	 */
	virtual void GetObjectItemInfo(const ContextRef &context, const ObjectRef &object,
		igdeMetaContextItemInfo &info) const = 0;
	
	/**
	 * \brief Valid set of objects.
	 */
	virtual Set GetValidObjects(const ContextRef &context) const;
	
	/**
	 * \brief Create copy of object if possible.
	 * 
	 * Subclass can override this method to create a copy of the object. The set parameter
	 * contains all existing objects. This set can be used to enforce rules, for example unique
	 * name or limit counts. If nullptr is returned the object cannot be copied.
	 */
	virtual ObjectRef CopyObject(const ContextRef &context, const Set &existingObjects,
		const ObjectRef &object) const;
	
	/**
	 * \brief Create action for target button.
	 * 
	 * Subclasses can override this method to create button action. If nullptr is returned
	 * the button is destroyed.
	 */
	virtual Action::Ref CreateButtonAction(TargetButton target, igdeWidget &owner);
	
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
	 */
	void AddDefaultContextMenuEntries(igdeMenuCascade &contextMenu,
		const ContextRef &context, igdeWidget &owner);
	
	/** \brief Create default button action. */
	Action::Ref CreateDefaultButtonAction(TargetButton target, igdeWidget &owner);
	/*@}*/
};



/**
 * \brief Set meta property with deObject type.
 */
template<typename T>
class igdeMetaPropertySetType : public igdeMetaPropertySet{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertySetType<T>>;
	
	/** \brief Set of choices. */
	using SetType = decTObjectSet<T>;
	
	/** \brief Object type reference. */
	using ObjectTypeRef = deTObjectReference<T>;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create set meta property with label and description. */
	igdeMetaPropertySetType(const char *id, const char *name, const char *description) :
	igdeMetaPropertySet(id, name, description){}
	
	/** \brief Create meta property with label, description, filter and undo info set from translation tag. */
	igdeMetaPropertySetType(const char *id, const char *translationTag) :
	igdeMetaPropertySet(id, translationTag){}
	
protected:
	/** \brief Clean up set meta property. */
	~igdeMetaPropertySetType() override = default;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Convert set. */
	Set ConvertSet(const SetType &in) const{
		Set out;
		in.Visit([&](const ObjectTypeRef &object){
			out.Add(object);
		});
		return out;
	}
	
	/** Convert set. */
	SetType ConvertSet(const Set &in) const{
		SetType out;
		in.Visit([&](const ObjectRef &object){
			out.Add(object.DynamicCast<T>());
		});
		return out;
	}
	
	Set GetPropertyValue(const ContextRef &context) const override{
		return ConvertSet(GetPropertyValueType(context));
	}
	
	void SetPropertyValue(const ContextRef &context, const Set &value) override{
		SetPropertyValueType(context, ConvertSet(value));
	}
	
	/**
	 * \brief Change property value matching context with undo support.
	 *
	 * If the context has an undo system the change is recorded as an undo action.
	 * Otherwise SetPropertyValue() is called directly.
	 */
	deTObjectReference<igdeMetaPropertySetUndo> ChangePropertyValueType(
	const ContextRef &context, const SetType &newValue,
	const char *undoInfo = nullptr, const char *undoInfoLong = nullptr){
		return ChangePropertyValue(context, ConvertSet(newValue), undoInfo, undoInfoLong);
	}
	
	ObjectRef GetActiveObject(const ContextRef &context) const override{
		return GetActiveObjectType(context);
	}
	
	void SetActiveObject(const ContextRef &context, const ObjectRef &activeObject) override{
		SetActiveObjectType(context, activeObject.DynamicCast<T>());
	}
	
	Set GetSelection(const ContextRef &context) const override{
		return ConvertSet(GetSelectionType(context));
	}
	
	void SetSelection(const ContextRef &context, const Set &selection) override{
		SetSelectionType(context, ConvertSet(selection));
	}
	
	void GetObjectItemInfo(const ContextRef &context, const ObjectRef &object,
	igdeMetaContextItemInfo &info) const override{
		GetObjectItemInfoType(context, object.DynamicCast<T>(), info);
	}
	
	ObjectRef CopyObject(const ContextRef &context, const Set &existingObjects,
	const ObjectRef &object) const override{
		return CopyObjectType(context, ConvertSet(existingObjects), object.DynamicCast<T>());
	}
	
	
	/**
	 * \brief Get property value matching context.
	 *
	 * Implemented by subclass.
	 */
	virtual SetType GetPropertyValueType(const ContextRef &context) const = 0;
	
	/**
	 * \brief Set property value matching context.
	 *
	 * Implemented by subclass.
	 */
	virtual void SetPropertyValueType(const ContextRef &context, const SetType &value) = 0;
	
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
	virtual SetType GetSelectionType(const ContextRef &context) const{
		auto active = GetActiveObjectType(context);
		return active ? SetType(devctag, active) : SetType();
	}
	
	/**
	 * \brief Set object selection.
	 */
	virtual void SetSelectionType(const ContextRef &context, const SetType &selection){
	}
	
	/**
	 * \brief Get object item information.
	 */
	virtual void GetObjectItemInfoType(const ContextRef &context, const ObjectTypeRef &object,
		igdeMetaContextItemInfo &info) const = 0;
	
	/**
	 * \brief Create copy of object if possible.
	 * 
	 * Subclass can override this method to create a copy of the object. The set parameter
	 * contains all existing objects. This set can be used to enforce rules, for example unique
	 * name or limit counts. If nullptr is returned the object cannot be copied.
	 */
	virtual ObjectTypeRef CopyObjectType(const ContextRef &context,
	const SetType &existingObjects, const ObjectTypeRef &object) const{
		return {};
	}
	/*@}*/
};


/**
 * \brief Set meta property using storage.
 */
template<typename T>
class igdeMetaPropertySetStorage : public igdeMetaPropertySetType<T>{
public:
	/** \brief Storage type. */
	using Storage = igdeMetaPropertyStorageSet<T, igdeMetaPropertySetStorage<T>>;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create set meta property with label and description. */
	igdeMetaPropertySetStorage(const char *id, const char *name, const char *description) :
	igdeMetaPropertySetType<T>(id, name, description){}
	
	/** \brief Create meta property with label, description, filter and undo info set from translation tag. */
	igdeMetaPropertySetStorage(const char *id, const char *translationTag) :
	igdeMetaPropertySetType<T>(id, translationTag){}
	
protected:
	/** \brief Clean up set meta property. */
	~igdeMetaPropertySetStorage() override = default;
	
public:
	/*@}*/
	/** \brief Storage. */
	virtual Storage &GetStorage(const typename igdeMetaPropertySetStorage<T>::ContextRef &context) const = 0;
	
	
	typename igdeMetaPropertySetStorage<T>::SetType GetPropertyValueType(
	const typename igdeMetaPropertySetStorage<T>::ContextRef &context) const override{
		return GetStorage(context).GetValue();
	}
	
	void SetPropertyValueType(const typename igdeMetaPropertySetStorage<T>::ContextRef &context,
	const typename igdeMetaPropertySetStorage<T>::SetType &value) override{
		GetStorage(context).SetValue(value);
	}
	
	typename igdeMetaPropertySetStorage<T>::ObjectTypeRef GetActiveObjectType(
	const typename igdeMetaPropertySetStorage<T>::ContextRef &context) const override{
		return GetStorage(context).GetActive();
	}
	
	void SetActiveObjectType(const typename igdeMetaPropertySetStorage<T>::ContextRef &context,
	const typename igdeMetaPropertySetStorage<T>::ObjectTypeRef &activeObject) override{
		GetStorage(context).SetActive(activeObject);
	}
	
	typename igdeMetaPropertySetStorage<T>::SetType GetSelectionType(
	const typename igdeMetaPropertySetStorage<T>::ContextRef &context) const override{
		return GetStorage(context).GetSelection();
	}
	
	void SetSelectionType(const typename igdeMetaPropertySetStorage<T>::ContextRef &context,
	const typename igdeMetaPropertySetStorage<T>::SetType &selection) override{
		GetStorage(context).SetSelection(selection);
	}
};

#endif
