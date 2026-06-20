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

#ifndef _IGDEMETAPROPERTYOBJECT_H_
#define _IGDEMETAPROPERTYOBJECT_H_

#include "igdeMetaProperty.h"
#include "storage/igdeMetaPropertyStorageObject.h"
#include "../igdeTMetaData.h"
#include "../../clipboard/igdeClipboardData.h"
#include "../../gui/model/igdeListItem.h"

class igdeMetaContextItemInfo;
class igdeMetaPropertyObjectUndo;


/**
 * \brief Object meta property.
 */
class DE_DLL_EXPORT igdeMetaPropertyObject : public igdeMetaProperty{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyObject>;
	
	/** \brief List of objects. */
	using ObjectList = igdeTMetaData<decTObjectOrderedSet<deObject>>::Ref;
	
	/** \brief Object reference. */
	using ObjectRef = deTObjectReference<deObject>;
	
	
	/** \brief Clipboard data. */
	class DE_DLL_EXPORT ClipboardData : public igdeTClipboardData<ObjectRef>{
	public:
		using Ref = deTObjectReference<ClipboardData>;
		
		/** \brief Type name. */
		static constexpr const char *TypeName = "MetaProperty.Object";
		
		explicit inline ClipboardData(const ObjectRef &value) : igdeTClipboardData<ObjectRef>(TypeName, value){}
		explicit inline ClipboardData(ObjectRef &&value) : igdeTClipboardData<ObjectRef>(TypeName, value){}
		
	protected:
		/** \brief Clean up object. */
		~ClipboardData() override = default;
	};
	
	
	/** \brief Listener. */
	class DE_DLL_EXPORT Listener : public TListener<igdeMetaPropertyObject>{
	public:
		/** \brief List of allowed objects changed. */
		virtual void OnAllowedObjectsChanged(igdeMetaPropertyObject *property, const ContextRef &context);
		
		/** \brief Object item information changed. */
		virtual void OnObjectItemInfoChanged(igdeMetaPropertyObject *property, const ContextRef &context);
	};
	
	
private:
	ObjectRef pDefaultValue;
	bool pSorted = false;
	igdeTListenerList<Listener> pListeners;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	
	/** \brief Create object meta property with label and description. */
	igdeMetaPropertyObject(const char *id, const char *name, const char *description);
	
protected:
	/** \brief Clean up object meta property. */
	~igdeMetaPropertyObject() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Default value. */
	inline const ObjectRef &GetDefaultValue() const{ return pDefaultValue; }
	
	/** \brief Set default value. */
	void SetDefaultValue(const ObjectRef &value);
	
	/** \brief Sorted objects. */
	inline bool GetSorted() const{ return pSorted; }
	
	/** \brief Set sorted objects. */
	void SetSorted(bool sorted);
	
	/** \brief Listeners. */
	inline igdeTListenerList<Listener> &GetListeners(){ return pListeners; }
	inline const igdeTListenerList<Listener> &GetListeners() const{ return pListeners; }
	
	/** \brief Notify listeners about value changed. */
	void NotifyValueChanged(const ContextRef &context);
	
	/** \brief Notify listeners about objects changed. */
	void NotifyAllowedObjectsChanged(const ContextRef &context);
	
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
	virtual ObjectRef GetPropertyValue(const ContextRef &context) const = 0;
	
	/**
	 * \brief Set property value matching context.
	 *
	 * Implemented by subclass.
	 */
	virtual void SetPropertyValue(const ContextRef &context, const ObjectRef &value) = 0;
	
	/**
	 * \brief Change property value matching context with undo support.
	 *
	 * If the context has an undo system the change is recorded as an undo action.
	 * Otherwise SetPropertyValue() is called directly.
	 */
	virtual deTObjectReference<igdeMetaPropertyObjectUndo> ChangePropertyValue(
		const ContextRef &context, const ObjectRef &newValue,
		const char *undoInfo = nullptr, const char *undoInfoLong = nullptr);
	
	/**
	 * \brief Get object item information.
	 */
	virtual void GetObjectItemInfo(const ContextRef &context, const ObjectRef &object,
		igdeMetaContextItemInfo &info) const = 0;
	
	/** \brief Allowed objects. */
	virtual ObjectList GetPropertyAllowedObjects(const ContextRef &context) const = 0;
	
	
	/**
	 * \brief Create UI widget.
	 *
	 * This object is able to add itself to a widget holder in the appropriate way.
	 */
	deTObjectReference<igdeMetaPropertyWidget> CreateWidget() override;
	/*@}*/
};



/**
 * \brief Typed object meta property.
 */
template<typename T>
class igdeMetaPropertyObjectType : public igdeMetaPropertyObject{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyObjectType<T>>;
	
	/** \brief List of objects. */
	using ObjectTypeList = igdeTMetaData<decTObjectOrderedSet<T>>::Ref;
	
	/** \brief Object type reference. */
	using ObjectTypeRef = deTObjectReference<T>;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object meta property with label and description. */
	igdeMetaPropertyObjectType(const char *id, const char *name, const char *description) :
	igdeMetaPropertyObject(id, name, description){
	}
	
protected:
	/** \brief Clean up object meta property. */
	~igdeMetaPropertyObjectType() override = default;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	ObjectRef GetPropertyValue(const ContextRef &context) const override{
		return GetPropertyValueType(context).template StaticCast<deObject>();
	}
	
	void SetPropertyValue(const ContextRef &context, const ObjectRef &value) override{
		SetPropertyValueType(context, value.DynamicCast<T>());
	}
	
	void GetObjectItemInfo(const ContextRef &context, const ObjectRef &object,
	igdeMetaContextItemInfo &info) const override{
		GetObjectItemInfoType(context, object.DynamicCast<T>(), info);
	}
	
	/** \brief Allowed objects. */
	ObjectList GetPropertyAllowedObjects(const ContextRef &context) const override{
		const auto &objects = GetPropertyAllowedObjectsType(context);
		if(!objects){
			return {};
		}
		
		decTObjectOrderedSet<deObject> list;
		objects->GetData().Visit([&](const ObjectRef &object){
			list.Add(object);
		});
		return ObjectList::New(list);
	}
	
	
	/**
	 * \brief Change property value matching context with undo support.
	 *
	 * If the context has an undo system the change is recorded as an undo action.
	 * Otherwise SetPropertyValue() is called directly.
	 */
	deTObjectReference<igdeMetaPropertyObjectUndo> ChangePropertyValueType(
	const ContextRef &context, const ObjectTypeRef &newValue,
	const char *undoInfo = nullptr, const char *undoInfoLong = nullptr){
		return ChangePropertyValue(context, newValue, undoInfo, undoInfoLong);
	}
	
	/** \brief Default value. */
	ObjectTypeRef GetDefaultValueType() const{
		return GetDefaultValue().DynamicCast<T>();
	}
	
	/** \brief Set default value. */
	void SetDefaultValueType(const ObjectTypeRef &value){
		SetDefaultValue(value);
	}
	
	
	/**
	 * \brief Get property value matching context.
	 * 
	 * Implemented by subclass.
	 */
	virtual ObjectTypeRef GetPropertyValueType(const ContextRef &context) const = 0;
	
	/**
	 * \brief Set property value matching context.
	 * 
	 * Implemented by subclass.
	 */
	virtual void SetPropertyValueType(const ContextRef &context, const ObjectTypeRef &value) = 0;
	
	/** \brief Get object item information. */
	virtual void GetObjectItemInfoType(const ContextRef &context, const ObjectTypeRef &object,
		igdeMetaContextItemInfo &info) const = 0;
	
	/** \brief Allowed objects. */
	virtual ObjectTypeList GetPropertyAllowedObjectsType(const ContextRef &context) const = 0;
	/*@}*/
};


/**
 * \brief Object meta property using storage.
 */
template<typename T>
class igdeMetaPropertyObjectStorage : public igdeMetaPropertyObjectType<T>{
public:
	/** \brief Storage type. */
	using Storage = igdeMetaPropertyStorageObject<T, igdeMetaPropertyObjectStorage<T>>;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	
	/** \brief Create object meta property with label and description. */
	igdeMetaPropertyObjectStorage(const char *id, const char *name, const char *description) :
	igdeMetaPropertyObjectType<T>(id, name, description){}
	
protected:
	/** \brief Clean up object meta property. */
	~igdeMetaPropertyObjectStorage() override = default;
	
public:
	/*@}*/
	/** \brief Storage. */
	virtual Storage &GetStorage(const igdeMetaProperty::ContextRef &context) const = 0;
	
	typename igdeMetaPropertyObjectStorage<T>::ObjectTypeRef GetPropertyValueType(
	const igdeMetaProperty::ContextRef &context) const override{
		return GetStorage(context).GetValue();
	}
	
	void SetPropertyValueType(const igdeMetaProperty::ContextRef &context,
	const typename igdeMetaPropertyObjectStorage<T>::ObjectTypeRef &value) override{
		GetStorage(context).SetValue(value);
	}
};

#endif
