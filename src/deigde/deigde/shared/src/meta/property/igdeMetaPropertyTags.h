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

#ifndef _IGDEMETAPROPERTYTAGS_H_
#define _IGDEMETAPROPERTYTAGS_H_

#include "igdeMetaProperty.h"
#include "storage/igdeMetaPropertyStorageComplex.h"

#include <dragengine/common/string/decStringSet.h>

class igdeMetaPropertyTagsUndo;


/**
 * \brief Tags meta property.
 */
class DE_DLL_EXPORT igdeMetaPropertyTags : public igdeMetaProperty{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyTags>;
	
	
	/** \brief Listener. */
	class DE_DLL_EXPORT Listener : public TListener<igdeMetaPropertyTags>{
	};
	
	
private:
	int pRows;
	igdeTListenerList<Listener> pListeners;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	
	/** \brief Create tags meta property with label and description. */
	igdeMetaPropertyTags(const char *id, const char *name, const char *description, int rows);
	
	/** \brief Create tags meta property with label, description, filter and undo info set from translation tag. */
	igdeMetaPropertyTags(const char *id, const char *translationTag, int rows);
	
protected:
	/** \brief Clean up tags meta property. */
	~igdeMetaPropertyTags() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Rows. */
	inline int GetRows() const{ return pRows; }
	
	/** \brief Set rows. */
	void SetRows(int rows);
	
	
	/** \brief Listeners. */
	inline igdeTListenerList<Listener> &GetListeners(){ return pListeners; }
	inline const igdeTListenerList<Listener> &GetListeners() const{ return pListeners; }
	
	/** \brief Notify listeners about value change. */
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
	virtual const decStringSet &GetPropertyValue(const ContextRef &context) const = 0;
	
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
	virtual deTObjectReference<igdeMetaPropertyTagsUndo> ChangePropertyValue(
		const ContextRef &context, const decStringSet &newValue,
		const char *undoInfo = nullptr, const char *undoInfoLong = nullptr);
	
	
	/**
	 * \brief Create UI widget.
	 *
	 * This object is able to add itself to a widget holder in the appropriate way.
	 */
	deTObjectReference<igdeMetaPropertyWidget> CreateWidget() override;
	/*@}*/
};


/**
 * \brief Tags meta property using storage.
 */
class DE_DLL_EXPORT igdeMetaPropertyTagsStorage : public igdeMetaPropertyTags{
public:
	/** \brief Storage type. */
	using Storage = igdeMetaPropertyStorageComplex<decStringSet, igdeMetaPropertyTagsStorage>;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create tags meta property with label and description. */
	igdeMetaPropertyTagsStorage(const char *id, const char *name, const char *description, int rows);
	
	/** \brief Create meta property with label, description, filter and undo info set from translation tag. */
	igdeMetaPropertyTagsStorage(const char *id, const char *translationTag, int rows);
	
protected:
	/** \brief Clean up tags meta property. */
	~igdeMetaPropertyTagsStorage() override;
	
public:
	/*@}*/
	/** \brief Storage. */
	virtual Storage &GetStorage(const ContextRef &context) const = 0;
	
	
	const decStringSet &GetPropertyValue(const ContextRef &context) const override;
	void SetPropertyValue(const ContextRef &context, const decStringSet &value) override;
};


#include "undo/igdeMetaPropertyTagsUndo.h"

#endif
