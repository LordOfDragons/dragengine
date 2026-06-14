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
#include "storage/igdeMetaPropertyStorageComplex.h"
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
	using ObjectList = decTObjectOrderedSet<deObject>;
	
	
	/** \brief Clipboard data. */
	class DE_DLL_EXPORT ClipboardData : public igdeTClipboardData<deObject::Ref>{
	public:
		using Ref = deTObjectReference<ClipboardData>;
		
		/** \brief Type name. */
		static constexpr const char *TypeName = "MetaProperty.Object";
		
		explicit inline ClipboardData(const deObject::Ref &value) : igdeTClipboardData<deObject::Ref>(TypeName, value){}
		explicit inline ClipboardData(deObject::Ref &&value) : igdeTClipboardData<deObject::Ref>(TypeName, value){}
		
	protected:
		/** \brief Clean up object. */
		~ClipboardData() override = default;
	};
	
	
	/** \brief Listener. */
	class DE_DLL_EXPORT Listener : public TListener<igdeMetaPropertyObject>{
	public:
		/** \brief List of allowed objects changed. */
		virtual void OnObjectsChanged(igdeMetaPropertyObject *property);
	};
	
	
private:
	ObjectList pObjects;
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
	/**
	 * \brief Object list to choose from.
	 * 
	 * After changing the list call NotifyObjectsChanged().
	 */
	inline ObjectList &GetObjects(){ return pObjects; }
	inline const ObjectList &GetObjects() const{ return pObjects; }
	
	
	/** \brief Listeners. */
	inline igdeTListenerList<Listener> &GetListeners(){ return pListeners; }
	inline const igdeTListenerList<Listener> &GetListeners() const{ return pListeners; }
	
	/** \brief Notify listeners about value changed. */
	void NotifyValueChanged(const ContextRef &context);
	
	/** \brief Notify listeners about objects changed. */
	void NotifyObjectsChanged();
	
	
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
	virtual const deObject::Ref &GetPropertyValue(const ContextRef &context) const = 0;
	
	/**
	 * \brief Set property value matching context.
	 *
	 * Implemented by subclass.
	 */
	virtual void SetPropertyValue(const ContextRef &context, const deObject::Ref &value) = 0;
	
	/**
	 * \brief Change property value matching context with undo support.
	 *
	 * If the context has an undo system the change is recorded as an undo action.
	 * Otherwise SetPropertyValue() is called directly.
	 */
	virtual deTObjectReference<igdeMetaPropertyObjectUndo> ChangePropertyValue(
		const ContextRef &context, const deObject::Ref &newValue,
		const char *undoInfo = nullptr, const char *undoInfoLong = nullptr);
	
	/**
	 * \brief Get object item information.
	 */
	virtual void GetObjectItemInfo(const ContextRef &context, const deObject::Ref &object,
		igdeMetaContextItemInfo &info) const = 0;
	
	
	/**
	 * \brief Create UI widget.
	 *
	 * This object is able to add itself to a widget holder in the appropriate way.
	 */
	deTObjectReference<igdeMetaPropertyWidget> CreateWidget(const ContextRef &context) override;
	/*@}*/
};


/**
 * \brief Object meta property using storage.
 */
class DE_DLL_EXPORT igdeMetaPropertyObjectStorage : public igdeMetaPropertyObject{
public:
	/** \brief Storage type. */
	using Storage = igdeMetaPropertyStorageComplex<deObject::Ref, igdeMetaPropertyObjectStorage>;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	
	/** \brief Create object meta property with label and description. */
	igdeMetaPropertyObjectStorage(const char *id, const char *name, const char *description);
	
protected:
	/** \brief Clean up object meta property. */
	~igdeMetaPropertyObjectStorage() override;
	
public:
	/*@}*/
	/** \brief Storage. */
	virtual Storage &GetStorage(const ContextRef &context) const = 0;
	
	
	const deObject::Ref &GetPropertyValue(const ContextRef &context) const override;
	void SetPropertyValue(const ContextRef &context, const deObject::Ref &value) override;
};

#endif
