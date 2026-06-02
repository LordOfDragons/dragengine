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

#ifndef _IGDEMETAPROPERTYSELECTION_H_
#define _IGDEMETAPROPERTYSELECTION_H_

#include "igdeMetaProperty.h"
#include "../../gui/model/igdeListItem.h"

#include <type_traits>


/**
 * \brief Selection meta property.
 */
class DE_DLL_EXPORT igdeMetaPropertySelection : public igdeMetaProperty{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertySelection>;
	
	
private:
	igdeListItem::List pChoices;
	void *pDefaultValue;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	
	/** \brief Create selection meta property with label and description. */
	igdeMetaPropertySelection(const char *name, const char *description, const igdeListItem::List &choices);
	
protected:
	/** \brief Clean up selection meta property. */
	~igdeMetaPropertySelection() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Choices. */
	inline const igdeListItem::List &GetChoices() const{ return pChoices; }
	
	/** \brief Default value. */
	inline void *GetDefaultValue() const{ return pDefaultValue; }
	
	/** \brief Set default value. */
	void SetDefaultValue(void *value);
	
	
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
	virtual void *GetPropertyValue(const igdeMetaContext::Ref &context) const = 0;
	
	/**
	 * \brief Set property value matching context.
	 * 
	 * Implemented by subclass.
	 */
	virtual void SetPropertyValue(const igdeMetaContext::Ref &context, void *value) = 0;
	
	
	/**
	 * \brief Create UI widget.
	 * 
	 * This object is able to add itself to a widget holder in the appropriate way.
	 */
	deTObjectReference<igdeMetaPropertyWidget> CreateWidget(const igdeMetaContext::Ref &context) override;
	/*@}*/
};


/**
 * \brief Selection meta property with enumeration type.
 */
template<typename T, typename = typename std::enable_if<std::is_enum<T>::value>::type>
class DE_DLL_EXPORT igdeMetaPropertySelectionEnum : public igdeMetaPropertySelection{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertySelectionEnum<T>>;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	
	/** \brief Create selection meta property with label and description. */
	igdeMetaPropertySelectionEnum(const char *name, const char *description,
		const igdeListItem::List &choices) :
	igdeMetaPropertySelection(name, description, choices){
	}
	
protected:
	/** \brief Clean up selection meta property. */
	~igdeMetaPropertySelectionEnum() override = default;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Default value. */
	inline T GetDefaultValueEnum() const{
		return static_cast<T>(reinterpret_cast<intptr_t>(igdeMetaPropertySelection::GetDefaultValue()));
	}
	
	/** \brief Set default value. */
	inline void SetDefaultValueEnum(T value){
		igdeMetaPropertySelection::SetDefaultValue(reinterpret_cast<void*>(static_cast<intptr_t>(value)));
	}
	
	
	void *GetPropertyValue(const igdeMetaContext::Ref &context) const override{
		return reinterpret_cast<void*>(static_cast<intptr_t>(GetPropertyValueEnum(context)));
	}
	
	void SetPropertyValue(const igdeMetaContext::Ref &context, void *value) override{
		SetPropertyValueEnum(context, static_cast<T>(reinterpret_cast<intptr_t>(value)));
	}
	
	/**
	 * \brief Get property value matching context.
	 * 
	 * Implemented by subclass.
	 */
	virtual T GetPropertyValueEnum(const igdeMetaContext::Ref &context) const = 0;
	
	/**
	 * \brief Set property value matching context.
	 * 
	 * Implemented by subclass.
	 */
	virtual void SetPropertyValueEnum(const igdeMetaContext::Ref &context, T value) = 0;
	/*@}*/
	
	
protected:
	/** \brief Helper function to add choices to the list. */
	static void AddChoice(igdeListItem::List &choices, T value, const char *name, igdeIcon *icon = nullptr){
		choices.Add(igdeListItem::Ref::New(name, icon, "", reinterpret_cast<void*>(static_cast<intptr_t>(value))));
	}
};

#endif
