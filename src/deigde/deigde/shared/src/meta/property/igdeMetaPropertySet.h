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
#include "../igdeTMetaData.h"
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
		
		explicit inline ClipboardData(const Set &value) : igdeTClipboardData<Set>(TypeName, value){}
		explicit inline ClipboardData(Set &&value) : igdeTClipboardData<Set>(TypeName, value){}
		
	protected:
		/** \brief Clean up object. */
		~ClipboardData() override = default;
	};
	
	
	/** \brief Listener. */
	class DE_DLL_EXPORT Listener : public TListener<igdeMetaPropertySet>{
	public:
		/** \brief Active object changed. */
		virtual void OnActiveChanged(igdeMetaPropertySet *property, const igdeMetaContext::Ref &context);
		
		/** \brief Object selection changed. */
		virtual void OnSelectionChanged(igdeMetaPropertySet *property, const igdeMetaContext::Ref &context);
	};
	
	
	/** \brief Remove selected entries action. */
	class DE_DLL_EXPORT ActionRemove : public igdeAction{
	protected:
		igdeMetaPropertySet &pProperty;
		const igdeMetaContext::Ref pContext;
		igdeEnvironment &pEnvironment;
		
	public:
		using Ref = deTObjectReference<ActionRemove>;
		ActionRemove(igdeMetaPropertySet &property, const igdeMetaContext::Ref &context,
			igdeEnvironment &environment);
		void OnAction() override;
		void Update() override;
	};
	
	/** \brief Remove all entries action. */
	class DE_DLL_EXPORT ActionRemoveAll : public igdeAction{
	protected:
		igdeMetaPropertySet &pProperty;
		const igdeMetaContext::Ref pContext;
		igdeEnvironment &pEnvironment;
		
	public:
		using Ref = deTObjectReference<ActionRemoveAll>;
		ActionRemoveAll(igdeMetaPropertySet &property, const igdeMetaContext::Ref &context,
			igdeEnvironment &environment);
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
	/** \brief Create object meta property with label and description. */
	igdeMetaPropertySet(const char *id, const char *name, const char *description);
	
protected:
	/** \brief Clean up object meta property. */
	~igdeMetaPropertySet() override;
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
	void NotifyValueChanged(const igdeMetaContext::Ref &context);
	
	/** \brief Notify listeners about active object changed. */
	void NotifyActiveChanged(const igdeMetaContext::Ref &context);
	
	/** \brief Notify listeners about object selection changed. */
	void NotifySelectionChanged(const igdeMetaContext::Ref &context);
	
	
	/**
	 * \brief Capture context.
	 *
	 * This returns an immutable context always returning the necessary state for getting or setting the property.
	 */
	virtual igdeMetaContext::Ref Capture(const igdeMetaContext::Ref &context) const = 0;
	
	/**
	 * \brief Property is valid.
	 *
	 * This means calling GetPropertyValue() nor SetPropertyValue() throws an exception.
	 */
	virtual bool IsValid(const igdeMetaContext::Ref &context) const = 0;
	
	/**
	 * \brief Get property value matching context.
	 *
	 * Implemented by subclass.
	 */
	virtual Set GetPropertyValue(const igdeMetaContext::Ref &context) const = 0;
	
	/**
	 * \brief Set property value matching context.
	 *
	 * Implemented by subclass.
	 */
	virtual void SetPropertyValue(const igdeMetaContext::Ref &context, const Set &value) = 0;
	
	/**
	 * \brief Change property value matching context with undo support.
	 *
	 * If the context has an undo system the change is recorded as an undo action.
	 * Otherwise SetPropertyValue() is called directly.
	 */
	deTObjectReference<igdeMetaPropertySetUndo> ChangePropertyValue(
		const igdeMetaContext::Ref &context, const Set &newValue,
		const char *undoInfo = nullptr, const char *undoInfoLong = nullptr);
	
	/**
	 * \brief Get active object or nullptr if no active object.
	 */
	virtual deObject::Ref GetActiveObject(const igdeMetaContext::Ref &context) const = 0;
	
	/**
	 * \brief Set active object.
	 */
	virtual void SetActiveObject(const igdeMetaContext::Ref &context, const deObject::Ref &activeObject) = 0;
	
	/**
	 * \brief Get object selection.
	 */
	virtual Set GetSelection(const igdeMetaContext::Ref &context) const;
	
	/**
	 * \brief Set object selection.
	 */
	virtual void SetSelection(const igdeMetaContext::Ref &context, const Set &selection);
	
	/**
	 * \brief Get object item information.
	 */
	virtual void GetObjectItemInfo(const deObject::Ref &object, igdeMetaContextItemInfo &info) const = 0;
	
	/**
	 * \brief Create action for target button.
	 * 
	 * Subclasses can override this method to create button action. If nullptr is returned
	 * the button is destroyed.
	 */
	virtual igdeAction::Ref CreateButtonAction(TargetButton target,
		const igdeMetaContext::Ref &context, igdeWidget &owner);
	
	/**
	 * \brief Create UI widget.
	 *
	 * This object is able to add itself to a widget holder in the appropriate way.
	 */
	deTObjectReference<igdeMetaPropertyWidget> CreateWidget(const igdeMetaContext::Ref &context) override;
	
	
	/**
	 * \brief Add default context menu entries.
	 * 
	 * Add first context menu entries to add list entries then call AddDefaultContextMenuEntries().
	 * This adds the following actions:
	 * - ActionRemove
	 * - ActionRemoveAll
	 */
	void AddDefaultContextMenuEntries(igdeMenuCascade &contextMenu,
		const igdeMetaContext::Ref &context, igdeWidget &owner);
	
	/** \brief Create default button action. */
	igdeAction::Ref CreateDefaultButtonAction(TargetButton target,
		const igdeMetaContext::Ref &context, igdeWidget &owner);
	/*@}*/
};



/**
 * \brief Set meta property with deObject type.
 */
template<typename T, typename = typename std::enable_if<std::is_base_of<deObject, T>::value>::type>
class DE_DLL_EXPORT igdeMetaPropertySetType : public igdeMetaPropertySet{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertySetType<T>>;
	
	/** \brief Set of choices. */
	using SetType = decTObjectSet<T>;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create set meta property with label and description. */
	igdeMetaPropertySetType(const char *id, const char *name, const char *description) :
	igdeMetaPropertySet(id, name, description){
	}
	
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
		in.Visit([&](const deTObjectReference<T> &object){
			out.Add(object);
		});
		return out;
	}
	
	/** Convert set. */
	SetType ConvertSet(const Set &in) const{
		SetType out;
		in.Visit([&](const deObject::Ref &object){
			out.Add(object.DynamicCast<T>());
		});
		return out;
	}
	
	Set GetPropertyValue(const igdeMetaContext::Ref &context) const override{
		return ConvertSet(GetPropertyValueType(context));
	}
	
	void SetPropertyValue(const igdeMetaContext::Ref &context, const Set &value) override{
		SetPropertyValueType(context, ConvertSet(value));
	}
	
	/**
	 * \brief Change property value matching context with undo support.
	 *
	 * If the context has an undo system the change is recorded as an undo action.
	 * Otherwise SetPropertyValue() is called directly.
	 */
	deTObjectReference<igdeMetaPropertySetUndo> ChangePropertyValueType(
	const igdeMetaContext::Ref &context, const SetType &newValue){
		return ChangePropertyValue(context, ConvertSet(newValue));
	}
	
	deObject::Ref GetActiveObject(const igdeMetaContext::Ref &context) const override{
		return GetActiveObjectType(context);
	}
	
	void SetActiveObject(const igdeMetaContext::Ref &context, const deObject::Ref &activeObject) override{
		SetActiveObjectType(context, activeObject.DynamicCast<T>());
	}
	
	Set GetSelection(const igdeMetaContext::Ref &context) const override{
		return ConvertSet(GetSelectionType(context));
	}
	
	void SetSelection(const igdeMetaContext::Ref &context, const Set &selection) override{
		SetSelectionType(context, ConvertSet(selection));
	}
	
	void GetObjectItemInfo(const deObject::Ref &object, igdeMetaContextItemInfo &info) const override{
		GetObjectItemInfoType(object.DynamicCast<T>(), info);
	}
	
	
	/**
	 * \brief Get property value matching context.
	 *
	 * Implemented by subclass.
	 */
	virtual SetType GetPropertyValueType(const igdeMetaContext::Ref &context) const = 0;
	
	/**
	 * \brief Set property value matching context.
	 *
	 * Implemented by subclass.
	 */
	virtual void SetPropertyValueType(const igdeMetaContext::Ref &context, const SetType &value) = 0;
	
	/**
	 * \brief Get active object or nullptr if no active object.
	 */
	virtual deTObjectReference<T> GetActiveObjectType(const igdeMetaContext::Ref &context) const = 0;
	
	/**
	 * \brief Set active object.
	 */
	virtual void SetActiveObjectType(const igdeMetaContext::Ref &context, const deTObjectReference<T>::Ref &activeObject) = 0;
	
	/**
	 * \brief Get object selection.
	 */
	virtual SetType GetSelectionType(const igdeMetaContext::Ref &context) const{
		auto active = GetActiveObjectType(context);
		return active ? SetType(devctag, active) : SetType();
	}
	
	/**
	 * \brief Set object selection.
	 */
	virtual void SetSelectionType(const igdeMetaContext::Ref &context, const SetType &selection){
	}
	
	/**
	 * \brief Get object item information.
	 */
	virtual void GetObjectItemInfoType(const deTObjectReference<T> &object, igdeMetaContextItemInfo &info) const = 0;
	/*@}*/
};

#endif
