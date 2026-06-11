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
#include "../igdeTMetaData.h"
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
	
	
	/** \brief Listener. */
	class DE_DLL_EXPORT Listener : public TListener<igdeMetaPropertyList>{
	public:
		/** \brief Active object changed. */
		virtual void OnActiveChanged(igdeMetaPropertyList *property, const igdeMetaContext::Ref &context);
		
		/** \brief Object selection changed. */
		virtual void OnSelectionChanged(igdeMetaPropertyList *property, const igdeMetaContext::Ref &context);
	};
	
	
	/** \brief Remove selected entries action. */
	class DE_DLL_EXPORT ActionRemove : public igdeAction{
		igdeMetaPropertyList &pProperty;
		const igdeMetaContext::Ref pContext;
		
	public:
		using Ref = deTObjectReference<ActionRemove>;
		ActionRemove(igdeMetaPropertyList &property, const igdeMetaContext::Ref &context,
			igdeEnvironment &environment);
		void OnAction() override;
		void Update() override;
	};
	
	/** \brief Remove all entries action. */
	class DE_DLL_EXPORT ActionRemoveAll : public igdeAction{
		igdeMetaPropertyList &pProperty;
		const igdeMetaContext::Ref pContext;
		
	public:
		using Ref = deTObjectReference<ActionRemoveAll>;
		ActionRemoveAll(igdeMetaPropertyList &property, const igdeMetaContext::Ref &context,
			igdeEnvironment &environment);
		void OnAction() override;
		void Update() override;
	};
	
	/** \brief Move entry up action. */
	class DE_DLL_EXPORT ActionMoveUp : public igdeAction{
		igdeMetaPropertyList &pProperty;
		const igdeMetaContext::Ref pContext;
		bool pTop;
		
	public:
		using Ref = deTObjectReference<ActionMoveUp>;
		ActionMoveUp(igdeMetaPropertyList &property, const igdeMetaContext::Ref &context,
			igdeEnvironment &environment);
		void OnAction() override;
		void Update() override;
		
		inline bool IsTop() const{ return pTop; }
		void SetTop(bool top);
	};
	
	/** \brief Move entry top action. */
	class DE_DLL_EXPORT ActionMoveTop : public ActionMoveUp{
	public:
		using Ref = deTObjectReference<ActionMoveTop>;
		ActionMoveTop(igdeMetaPropertyList &property, const igdeMetaContext::Ref &context,
			igdeEnvironment &environment);
	};
	
	/** \brief Move entry down action. */
	class DE_DLL_EXPORT ActionMoveDown : public igdeAction{
		igdeMetaPropertyList &pProperty;
		const igdeMetaContext::Ref pContext;
		bool pBottom;
		
	public:
		using Ref = deTObjectReference<ActionMoveDown>;
		ActionMoveDown(igdeMetaPropertyList &property, const igdeMetaContext::Ref &context,
			igdeEnvironment &environment);
		void OnAction() override;
		void Update() override;
		
		inline bool IsBottom() const{ return pBottom; }
		void SetBottom(bool bottom);
	};
	
	/** \brief Move entry bottom action. */
	class DE_DLL_EXPORT ActionMoveBottom : public ActionMoveDown{
	public:
		using Ref = deTObjectReference<ActionMoveBottom>;
		ActionMoveBottom(igdeMetaPropertyList &property, const igdeMetaContext::Ref &context,
			igdeEnvironment &environment);
	};
	
	
private:
	int pRows;
	bool pMultiSelection;
	igdeTListenerList<Listener> pListeners;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	
	/** \brief Create object meta property with label and description. */
	igdeMetaPropertyList(const char *id, const char *name, const char *description);
	
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
	virtual List GetPropertyValue(const igdeMetaContext::Ref &context) const = 0;
	
	/**
	 * \brief Set property value matching context.
	 *
	 * Implemented by subclass.
	 */
	virtual void SetPropertyValue(const igdeMetaContext::Ref &context, const List &value) = 0;
	
	/**
	 * \brief Change property value matching context with undo support.
	 *
	 * If the context has an undo system the change is recorded as an undo action.
	 * Otherwise SetPropertyValue() is called directly.
	 */
	deTObjectReference<igdeMetaPropertyListUndo> ChangePropertyValue(
		const igdeMetaContext::Ref &context, const List &newValue);
	
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
	virtual List GetSelection(const igdeMetaContext::Ref &context) const;
	
	/**
	 * \brief Set object selection.
	 */
	virtual void SetSelection(const igdeMetaContext::Ref &context, const List &selection);
	
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
	 * - ActionMoveUp
	 * - ActionMoveDown
	 * - ActionMoveTop
	 * - ActionMoveBottom
	 */
	void AddDefaultContextMenuEntries(igdeMenuCascade &contextMenu,
		const igdeMetaContext::Ref &context, igdeWidget &owner);
	
	/** \brief Create default button action. */
	igdeAction::Ref CreateDefaultButtonAction(TargetButton target,
		const igdeMetaContext::Ref &context, igdeWidget &owner);
	/*@}*/
};



template<typename T, typename Enable = void>
class igdeMetaPropertyListType;


/**
 * \brief List meta property with deObject type.
 */
template<typename T>
class DE_DLL_EXPORT igdeMetaPropertyListType<T,
	typename std::enable_if<std::is_base_of<deObject, T>::value>::type>
	: public igdeMetaPropertyList{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyListType<T>>;
	
	/** \brief List of choices. */
	using ListType = decTObjectOrderedSet<T>;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list meta property with label and description. */
	igdeMetaPropertyListType(const char *id, const char *name, const char *description) :
	igdeMetaPropertyList(id, name, description){
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
		in.Visit([&](const deTObjectReference<T> &object){
			out.Add(object);
		});
		return out;
	}
	
	/** Convert list. */
	ListType ConvertList(const List &in) const{
		ListType out;
		in.Visit([&](const deObject::Ref &object){
			out.Add(object.DynamicCast<T>());
		});
		return out;
	}
	
	List GetPropertyValue(const igdeMetaContext::Ref &context) const override{
		return ConvertList(GetPropertyValueType(context));
	}
	
	void SetPropertyValue(const igdeMetaContext::Ref &context, const List &value) override{
		SetPropertyValueType(context, ConvertList(value));
	}
	
	/**
	 * \brief Change property value matching context with undo support.
	 *
	 * If the context has an undo system the change is recorded as an undo action.
	 * Otherwise SetPropertyValue() is called directly.
	 */
	deTObjectReference<igdeMetaPropertyListUndo> ChangePropertyValueType(
	const igdeMetaContext::Ref &context, const ListType &newValue){
		return ChangePropertyValue(context, ConvertList(newValue));
	}
	
	deObject::Ref GetActiveObject(const igdeMetaContext::Ref &context) const override{
		return GetActiveObjectType(context);
	}
	
	void SetActiveObject(const igdeMetaContext::Ref &context, const deObject::Ref &activeObject) override{
		SetActiveObjectType(context, activeObject.DynamicCast<T>());
	}
	
	List GetSelection(const igdeMetaContext::Ref &context) const override{
		return ConvertList(GetSelectionType(context));
	}
	
	void SetSelection(const igdeMetaContext::Ref &context, const List &selection) override{
		SetSelectionType(context, ConvertList(selection));
	}
	
	void GetObjectItemInfo(const deObject::Ref &object, igdeMetaContextItemInfo &info) const override{
		GetObjectItemInfoType(object.DynamicCast<T>(), info);
	}
	
	
	/**
	 * \brief Get property value matching context.
	 * 
	 * Implemented by subclass.
	 */
	virtual ListType GetPropertyValueType(const igdeMetaContext::Ref &context) const = 0;
	
	/**
	 * \brief Set property value matching context.
	 * 
	 * Implemented by subclass.
	 */
	virtual void SetPropertyValueType(const igdeMetaContext::Ref &context, const ListType &value) = 0;
	
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
	virtual ListType GetSelectionType(const igdeMetaContext::Ref &context) const{
		auto active = GetActiveObjectType(context);
		return active ? ListType(devctag, active) : ListType();
	}
	
	/**
	 * \brief Set object selection.
	 */
	virtual void SetSelectionType(const igdeMetaContext::Ref &context, const ListType &selection){
	}
	
	/**
	 * \brief Get object item information.
	 */
	virtual void GetObjectItemInfoType(const deTObjectReference<T> &object, igdeMetaContextItemInfo &info) const = 0;
	/*@}*/
};



/**
 * \brief List meta property with arbitrary type.
 */
template<typename T>
class DE_DLL_EXPORT igdeMetaPropertyListType<T,
	typename std::enable_if<!std::is_base_of<deObject, T>::value>::type>
	: public igdeMetaPropertyList{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyListType<T>>;
	
	/** \brief List of choices. */
	using ListType = decTOrderedSet<T>;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list meta property with label and description. */
	igdeMetaPropertyListType(const char *id, const char *name, const char *description) :
	igdeMetaPropertyList(id, name, description){
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
		in.Visit([&](const T &object){
			out.Add(igdeTMetaData<T>::Ref::New(object));
		});
		return out;
	}
	
	/** Convert list. */
	ListType ConvertList(const List &in) const{
		ListType out;
		in.Visit([&](const deObject::Ref &object){
			out.Add(object.DynamicCast<igdeTMetaData<T>>()->GetData());
		});
		return out;
	}
	
	List GetPropertyValue(const igdeMetaContext::Ref &context) const override{
		return ConvertList(GetPropertyValueType(context));
	}
	
	void SetPropertyValue(const igdeMetaContext::Ref &context, const List &value) override{
		SetPropertyValueType(context, ConvertList(value));
	}
	
	/**
	 * \brief Change property value matching context with undo support.
	 *
	 * If the context has an undo system the change is recorded as an undo action.
	 * Otherwise SetPropertyValue() is called directly.
	 */
	deTObjectReference<igdeMetaPropertyListUndo> ChangePropertyValueType(
	const igdeMetaContext::Ref &context, const ListType &newValue){
		return ChangePropertyValue(context, ConvertList(newValue));
	}
	
	deObject::Ref GetActiveObject(const igdeMetaContext::Ref &context) const override{
		return igdeTMetaData<T>::Ref::New(GetActiveObjectType(context));
	}
	
	void SetActiveObject(const igdeMetaContext::Ref &context, const deObject::Ref &activeObject) override{
		SetActiveObjectType(context, activeObject.DynamicCast<igdeTMetaData<T>>()->GetData());
	}
	
	List GetSelection(const igdeMetaContext::Ref &context) const override{
		return ConvertList(GetSelectionType(context));
	}
	
	void SetSelection(const igdeMetaContext::Ref &context, const List &selection) override{
		SetSelectionType(context, ConvertList(selection));
	}
	
	void GetObjectItemInfo(const deObject::Ref &object, igdeMetaContextItemInfo &info) const override{
		GetObjectItemInfoType(object.DynamicCast<igdeTMetaData<T>>()->GetData(), info);
	}
	
	
	/**
	 * \brief Get property value matching context.
	 * 
	 * Implemented by subclass.
	 */
	virtual ListType GetPropertyValueType(const igdeMetaContext::Ref &context) const = 0;
	
	/**
	 * \brief Set property value matching context.
	 * 
	 * Implemented by subclass.
	 */
	virtual void SetPropertyValueType(const igdeMetaContext::Ref &context, const ListType &value) = 0;
	
	/**
	 * \brief Get active object or nullptr if no active object.
	 */
	virtual T GetActiveObjectType(const igdeMetaContext::Ref &context) const = 0;
	
	/**
	 * \brief Set active object.
	 */
	virtual void SetActiveObjectType(const igdeMetaContext::Ref &context, const T &activeObject) = 0;
	
	/**
	 * \brief Get object selection.
	 */
	virtual ListType GetSelectionType(const igdeMetaContext::Ref &context) const{
		auto active = GetActiveObjectType(context);
		return active ? ListType(devctag, active) : ListType();
	}
	
	/**
	 * \brief Set object selection.
	 */
	virtual void SetSelectionType(const igdeMetaContext::Ref &context, const ListType &selection){
	}
	
	/**
	 * \brief Get object item information.
	 */
	virtual void GetObjectItemInfoType(const T &object, igdeMetaContextItemInfo &info) const = 0;
	/*@}*/
};

#endif
