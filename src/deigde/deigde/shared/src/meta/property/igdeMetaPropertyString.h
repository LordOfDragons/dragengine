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

#ifndef _IGDEMETAPROPERTYSTRING_H_
#define _IGDEMETAPROPERTYSTRING_H_

#include "igdeMetaProperty.h"
#include "igdeMetaPropertyShared.h"
#include "../../gui/model/igdeListItem.h"


/**
 * \brief String meta property.
 */
class DE_DLL_EXPORT igdeMetaPropertyString : public igdeMetaProperty{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyString>;
	
	
private:
	decString pDefaultValue;
	bool pEnableStringList;
	igdeTMetaPropertyData<igdeListItem::List>::Ref pStringList;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	
	/** \brief Create string meta property with label and description. */
	igdeMetaPropertyString(const char *name, const char *description);
	
protected:
	/** \brief Clean up string meta property. */
	~igdeMetaPropertyString() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Default value. */
	inline const decString &GetDefaultValue() const{ return pDefaultValue; }
	
	/** \brief Set default value. */
	void SetDefaultValue(const decString &value);
	
	/** \brief Enable string list restriction. */
	inline bool GetEnableStringList() const{ return pEnableStringList; }
	
	/** \brief Set enable string list restriction. */
	void SetEnableStringList(bool enable);
	
	
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
	virtual const decString &GetPropertyValue(const igdeMetaContext::Ref &context) const = 0;
	
	/**
	 * \brief Set property value matching context.
	 * 
	 * Implemented by subclass.
	 */
	virtual void SetPropertyValue(const igdeMetaContext::Ref &context, const decString &value) = 0;
	
	/**
	 * \brief String list to choose from.
	 *
	 * Subclass has to return the same immutable list reference as long as the list stays unchanged.
	 * As soon as the list reference changes the widget content is updated to reflect the changes.
	 */
	virtual igdeTMetaPropertyData<igdeListItem::List>::Ref GetStringList() const = 0;
	
	
	/**
	 * \brief Create UI widget.
	 * 
	 * This object is able to add itself to a widget holder in the appropriate way.
	 */
	deTObjectReference<igdeMetaPropertyWidget> CreateWidget(const igdeMetaContext::Ref &context) override;
	/*@}*/
};

#endif
