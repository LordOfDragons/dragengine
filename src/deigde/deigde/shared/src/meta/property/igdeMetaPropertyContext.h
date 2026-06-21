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

#ifndef _IGDEMETAPROPERTYCONTEXT_H_
#define _IGDEMETAPROPERTYCONTEXT_H_

#include "igdeMetaProperty.h"
#include "storage/igdeMetaPropertyStorageComplex.h"


/**
 * \brief Context meta property.
 * 
 * Shows properties of a context. Context meta properties have no label nor description.
 * They are like a group meta property but with dynamic content.
 */
class DE_DLL_EXPORT igdeMetaPropertyContext : public igdeMetaProperty{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyContext>;
	
	
	/** \brief Listener. */
	class DE_DLL_EXPORT Listener : public TListener<igdeMetaPropertyContext>{
	};
	
	
private:
	igdeTListenerList<Listener> pListeners;
	ContextRef pDefaultValue;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	
	/** \brief Create context meta property. */
	explicit igdeMetaPropertyContext(const char *id,
		const char *label = "", const char *description = "");
	
	/** \brief Create context meta property with label, description, filter and undo info set from translation tag. */
	igdeMetaPropertyContext(const char *id, const char *translationTag);
	
protected:
	/** \brief Clean up context meta property. */
	~igdeMetaPropertyContext() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Default context. */
	inline const ContextRef &GetDefaultValue() const{ return pDefaultValue; }
	
	/** \brief Set default context. */
	void SetDefaultValue(const ContextRef &value);
	
	
	/** \brief Listeners. */
	inline igdeTListenerList<Listener> &GetListeners(){ return pListeners; }
	inline const igdeTListenerList<Listener> &GetListeners() const{ return pListeners; }
	
	/** \brief Notify listeners about value changed. */
	void NotifyValueChanged(const ContextRef &context);
	
	
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
	virtual const ContextRef &GetPropertyValue(const ContextRef &context) const = 0;
	
	/**
	 * \brief Set property value matching context.
	 *
	 * Implemented by subclass.
	 */
	virtual void SetPropertyValue(const ContextRef &context, const ContextRef &value) = 0;
	
	
	/**
	 * \brief Create UI widget.
	 * 
	 * This object is able to add itself to a widget holder in the appropriate way.
	 */
	deTObjectReference<igdeMetaPropertyWidget> CreateWidget() override;
	/*@}*/
};


/**
 * \brief Context meta property using storage.
 */
class DE_DLL_EXPORT igdeMetaPropertyContextStorage : public igdeMetaPropertyContext{
public:
	/** \brief Storage type. */
	using Storage = igdeMetaPropertyStorageComplex<ContextRef, igdeMetaPropertyContextStorage>;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	
	/** \brief Create context meta property. */
	explicit igdeMetaPropertyContextStorage(const char *id,
		const char *label = "", const char *description = "");
	
protected:
	/** \brief Clean up context meta property. */
	~igdeMetaPropertyContextStorage() override;
	
public:
	/*@}*/
	/** \brief Storage. */
	virtual Storage &GetStorage(const ContextRef &context) const = 0;
	
	
	const ContextRef &GetPropertyValue(const ContextRef &context) const override;
	void SetPropertyValue(const ContextRef &context, const ContextRef &value) override;
};

#endif
