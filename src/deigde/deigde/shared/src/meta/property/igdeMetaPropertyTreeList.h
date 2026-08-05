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

#ifndef _IGDEMETAPROPERTYTREELIST_H_
#define _IGDEMETAPROPERTYTREELIST_H_

#include "igdeMetaProperty.h"
#include "storage/igdeMetaPropertyStorageTreeList.h"

class igdeMetaContextItemInfo;

/**
 * \brief Tree list meta property.
 *
 * Represents a tree list of objects typically composing of various list or set types as well
 * as grouping objects. For this reason the tree list meta property does not store a tree value
 * but uses a walker for widgets to obtain the tree list content. The meta property stores
 * the active object and a set of expanded objects.
 */
class DE_DLL_EXPORT igdeMetaPropertyTreeList : public igdeMetaProperty{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyTreeList>;
	
	/** \brief List of objects. */
	using List = decTObjectOrderedSet<deObject>;
	
	/** \brief Object reference. */
	using ObjectRef = deTObjectReference<deObject>;
	
	/** \brief Set of expanded objects. */
	using ExpandedSet = decTObjectSet<deObject>;
	
	
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
	class DE_DLL_EXPORT Listener : public TListener<igdeMetaPropertyTreeList>{
	public:
		/** \brief Active object changed. */
		virtual void OnActiveChanged(igdeMetaPropertyTreeList *property, const ContextRef &context);
		
		/** \brief Object expanded state changed. */
		virtual void OnExpandedChanged(igdeMetaPropertyTreeList *property, const ContextRef &context);
		
		/** \brief Object item information changed. */
		virtual void OnObjectItemInfoChanged(igdeMetaPropertyTreeList *property, const ContextRef &context);
	};
	
	
	/** \brief Walker. */
	class DE_DLL_EXPORT Walker : public deObject{
	public:
		/** \brief Reference type. */
		using Ref = deTObjectReference<Walker>;
		
	public:
		/** \name Constructors and Destructors */
		/*@{*/
		/** \brief Create walker at first object of tree. */
		Walker();
		
	protected:
		/** \brief Clean up walker. */
		~Walker() override;
		/*@}*/
		
	public:
		/** \name Management */
		/*@{*/
		/** \brief Current object or nullptr. */
		virtual ObjectRef GetCurrent() const = 0;
		
		/**
		 * \brief Set current object.
		 * \throws deeInvalidParam if object is not in the tree.
		 */
		virtual void SetCurrent(const ObjectRef &object) = 0;
		
		/**
		 * \brief Move to first object in the tree.
		 * 
		 * GetCurrent() can return nullptr if the tree is empty.
		 */
		virtual void MoveTreeFirst() = 0;
		
		/** \brief Current object has a next sibling. */
		virtual bool HasNext() const = 0;
		
		/**
		 * \brief Move to next sibling of current object.
		 * \returns true if moved or false if HasNext() returns false.
		 */
		virtual bool MoveNext() = 0;
		
		/**
		 * \brief Move to last sibling of current object.
		 * \returns true if moved or false if HasNext() returns false.
		 */
		virtual bool MoveLast() = 0;
		
		/** \brief Current object has a previous sibling. */
		virtual bool HasPrevious() const = 0;
		
		/**
		 * \brief Move to previous sibling of current object.
		 * \returns true if moved or false if HasPrevious() returns false.
		 */
		virtual bool MovePrevious() = 0;
		
		/**
		 * \brief Move to first sibling of current object.
		 * \returns true if moved or false if HasPrevious() returns false.
		 */
		virtual bool MoveFirst() = 0;
		
		/** \brief Current object has children. */
		virtual bool HasChildren() const = 0;
		
		/**
		 * \brief Move to first child of current object.
		 * \returns true if moved or false if HasChildren() returns false.
		 */
		virtual bool MoveChildren() = 0;
		
		/** \brief Current object has parent. */
		virtual bool HasParent() const = 0;
		
		/**
		 * \brief Move up to parent of current object.
		 * \returns true if moved or false if HasParent() returns false.
		 */
		virtual bool MoveParent() = 0;
		/*@}*/
	};
	
	
	/**
	 * \brief Walker helper to simplify implementing walkers.
	 * 
	 * Type T has to be a list collection type for example decTObjectSet or decTList. When moving
	 * up or down in the tree set the list for the current level.
	 * 
	 * Subclass is responsible to implement these functions:
	 * - SetCurrent()
	 * - MoveTreeFirst()
	 * - HasChildren()
	 * - MoveChildren()
	 * - HasParent()
	 * - MoveParent()
	 * 
	 * The helper implements sibling handling using the list.
	 */
	template<typename T>
	class WalkerHelper : public Walker{
	private:
		const T *pList = nullptr;
		int pIndex = -1;
		
	protected:
		WalkerHelper(const T *list, int index = 0) : pList(list), pIndex(index){
			DEASSERT_NOTNULL(list)
		}
		
		~WalkerHelper() override = default;
		
	protected:
		inline const T *GetList() const{ return pList; }
		inline int GetIndex() const{ return pIndex; }
		
		void SetList(const T *list, int index){
			DEASSERT_NOTNULL(list)
			pList = list;
			pIndex = index;
		}
		
	public:
		ObjectRef GetCurrent() const override{
			return pIndex >= 0 && pIndex < pList->GetCount() ? pList->GetAt(pIndex) : ObjectRef();
		}
		
		bool HasNext() const override{
			return pIndex >= 0 && pIndex < pList->GetCount() - 1;
		}
		
		bool MoveNext() override{
			if(!HasNext()){
				return false;
			}
			pIndex++;
			return true;
		}
		
		bool MoveLast() override{
			if(!HasNext()){
				return false;
			}
			pIndex = pList->GetCount() - 1;
			return true;
		}
		
		bool HasPrevious() const override{
			return pIndex > 0 && pIndex < pList->GetCount();
		}
		
		bool MovePrevious() override{
			if(!HasPrevious()){
				return false;
			}
			pIndex--;
			return true;
		}
		
		bool MoveFirst() override{
			if(!HasPrevious()){
				return false;
			}
			pIndex = 0;
			return true;
		}
	};
	
	
private:
	int pRows = 4;
	bool pSorted = false;
	igdeTListenerList<Listener> pListeners;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create tree list meta property with label and description. */
	igdeMetaPropertyTreeList(const char *id, const char *name, const char *description);
	
	/** \brief Create tree list meta property with label, description, filter and undo info set from translation tag. */
	igdeMetaPropertyTreeList(const char *id, const char *translationTag);
	
protected:
	/** \brief Clean up tree list meta property. */
	~igdeMetaPropertyTreeList() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Rows. */
	inline int GetRows() const{ return pRows; }
	
	/** \brief Set rows. */
	void SetRows(int rows);
	
	/** \brief Sorted. */
	inline bool GetSorted() const{ return pSorted; }
	
	/** \brief Set sorted. */
	void SetSorted(bool sorted);
	
	
	/** \brief Listeners. */
	inline igdeTListenerList<Listener> &GetListeners(){ return pListeners; }
	inline const igdeTListenerList<Listener> &GetListeners() const{ return pListeners; }
	
	/** \brief Notify listeners about value change. */
	void NotifyValueChanged(const ContextRef &context);
	
	/** \brief Notify listeners about active object changed. */
	void NotifyActiveChanged(const ContextRef &context);
	
	/** \brief Notify listeners about expanded state changed. */
	void NotifyExpandedChanged(const ContextRef &context);
	
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
	 * This means calling GetActiveObject() nor SetActiveObject() throws an exception.
	 */
	virtual bool IsValid(const ContextRef &context) const = 0;
	
	/**
	 * \brief Get active object or nullptr if no active object.
	 */
	virtual ObjectRef GetActiveObject(const ContextRef &context) const = 0;
	
	/**
	 * \brief Set active object.
	 */
	virtual void SetActiveObject(const ContextRef &context, const ObjectRef &activeObject) = 0;
	
	/**
	 * \brief Get expanded objects.
	 */
	virtual ExpandedSet GetExpanded(const ContextRef &context) const;
	
	/**
	 * \brief Set expanded objects.
	 */
	virtual void SetExpanded(const ContextRef &context, const ExpandedSet &expanded);
	
	/**
	 * \brief Get object item information.
	 */
	virtual void GetObjectItemInfo(const ContextRef &context, const ObjectRef &object,
		igdeMetaContextItemInfo &info) const = 0;
	
	/**
	 * \brief Create tree walker.
	 */
	virtual Walker::Ref CreateWalker(const ContextRef &context) const = 0;
	
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
	/*@}*/
};


/**
 * \brief Typed tree list meta property.
 */
template<typename T>
class igdeMetaPropertyTreeListType : public igdeMetaPropertyTreeList{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyTreeListType<T>>;
	
	/** \brief Object type reference. */
	using ObjectTypeRef = deTObjectReference<T>;
	
	/** \brief Set of expanded objects. */
	using ExpandedTypeSet = decTObjectSet<T>;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create tree list meta property with label and description. */
	igdeMetaPropertyTreeListType(const char *id, const char *name, const char *description) :
	igdeMetaPropertyTreeList(id, name, description){
	}
	
	/** \brief Create tree list meta property with label, description, filter and undo info set from translation tag. */
	igdeMetaPropertyTreeListType(const char *id, const char *translationTag) :
	igdeMetaPropertyTreeList(id, translationTag){
	}
	
protected:
	/** \brief Clean up tree list meta property. */
	~igdeMetaPropertyTreeListType() override = default;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Convert list. */
	ExpandedSet ConvertSet(const ExpandedTypeSet &in) const{
		ExpandedSet out;
		in.Visit([&](const ObjectTypeRef &object){
			out.Add(object.Pointer());
		});
		return out;
	}
	
	/** \brief Convert set. */
	ExpandedTypeSet ConvertSet(const ExpandedSet &in) const{
		ExpandedTypeSet out;
		in.Visit([&](const ObjectRef &object){
			out.Add(object.DynamicCast<T>());
		});
		return out;
	}
	
	ObjectRef GetActiveObject(const ContextRef &context) const override{
		return GetActiveObjectType(context);
	}
	
	void SetActiveObject(const ContextRef &context, const ObjectRef &activeObject) override{
		SetActiveObjectType(context, activeObject.DynamicCast<T>());
	}
	
	ExpandedSet GetExpanded(const ContextRef &context) const override{
		return ConvertSet(GetExpandedType(context));
	}
	
	void SetExpanded(const ContextRef &context, const ExpandedSet &expanded) override{
		SetExpandedType(context, ConvertSet(expanded));
	}
	
	void GetObjectItemInfo(const ContextRef &context, const ObjectRef &object,
	igdeMetaContextItemInfo &info) const override{
		GetObjectItemInfoType(context, object.DynamicCast<T>(), info);
	}
	
	/**
	 * \brief Get active object or nullptr if no active object.
	 */
	virtual ObjectTypeRef GetActiveObjectType(const ContextRef &context) const = 0;
	
	/**
	 * \brief Set active object.
	 */
	virtual void SetActiveObjectType(const ContextRef &context, const ObjectTypeRef &activeObject) = 0;
	
	/**
	 * \brief Get expanded objects.
	 */
	virtual ExpandedTypeSet GetExpandedType(const ContextRef &context) const{
		return {};
	}
	
	/**
	 * \brief Set expanded objects.
	 */
	virtual void SetExpandedType(const ContextRef &context, const ExpandedTypeSet &expanded){
	}
	
	/**
	 * \brief Get object item information.
	 */
	virtual void GetObjectItemInfoType(const ContextRef &context, const ObjectTypeRef &object,
		igdeMetaContextItemInfo &info) const = 0;
	/*@}*/
};


/**
 * \brief Tree list meta property using storage.
 * 
 * Only stores active object and expanded state. The actual content is provided by walking.
 */
template<typename T>
class igdeMetaPropertyTreeListStorage : public igdeMetaPropertyTreeListType<T>{
public:
	/** \brief Storage type. */
	using Storage = igdeMetaPropertyStorageTreeList<T, igdeMetaPropertyTreeListStorage<T>>;
	
	/** \brief List type from base class. */
	using List = typename igdeMetaPropertyTreeList::List;
	
	/** \brief Object reference type from base class. */
	using ObjectRef = typename igdeMetaPropertyTreeList::ObjectRef;
	
	/** \brief Object type reference from base class. */
	using ObjectTypeRef = typename igdeMetaPropertyTreeListType<T>::ObjectTypeRef;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create tree list meta property with label and description. */
	igdeMetaPropertyTreeListStorage(const char *id, const char *name, const char *description) :
	igdeMetaPropertyTreeListType<T>(id, name, description){}
	
	/** \brief Create tree list meta property with label, description, filter and undo info set from translation tag. */
	igdeMetaPropertyTreeListStorage(const char *id, const char *translationTag) :
	igdeMetaPropertyTreeListType<T>(id, translationTag){}
	
protected:
	/** \brief Clean up tree list meta property. */
	~igdeMetaPropertyTreeListStorage() override = default;
	/*@}*/
	
	
public:
	/** \brief Storage. */
	virtual Storage &GetStorage(const igdeMetaProperty::ContextRef &context) const = 0;
	
	
	typename igdeMetaPropertyTreeListStorage<T>::ObjectTypeRef GetActiveObjectType(
	const igdeMetaProperty::ContextRef &context) const override{
		return GetStorage(context).GetActive();
	}
	
	void SetActiveObjectType(const igdeMetaProperty::ContextRef &context,
	const typename igdeMetaPropertyTreeListStorage<T>::ObjectTypeRef &activeObject) override{
		GetStorage(context).SetActive(activeObject);
	}
	
	typename igdeMetaPropertyTreeListStorage<T>::ExpandedTypeSet GetExpandedType(
	const igdeMetaProperty::ContextRef &context) const override{
		return GetStorage(context).GetExpanded();
	}
	
	void SetExpandedType(const igdeMetaProperty::ContextRef &context,
	const typename igdeMetaPropertyTreeListStorage<T>::ExpandedTypeSet &expanded) override{
		GetStorage(context).SetExpanded(expanded);
	}
};

#endif
