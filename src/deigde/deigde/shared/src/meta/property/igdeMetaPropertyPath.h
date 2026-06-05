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

#ifndef _IGDEMETAPROPERTYPATH_H_
#define _IGDEMETAPROPERTYPATH_H_

#include "igdeMetaProperty.h"
#include "../../environment/igdeEnvironment.h"
#include "../../gui/filedialog/igdeFilePattern.h"


/**
 * \brief Path meta property.
 */
class DE_DLL_EXPORT igdeMetaPropertyPath : public igdeMetaProperty{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyPath>;
	
	
	/** \brief Listener. */
	class DE_DLL_EXPORT Listener : public TListener<igdeMetaPropertyPath>{
	};
	
	
private:
	igdeEnvironment::eFilePatternListTypes pResourceType;
	igdeFilePattern::List pCustomPatternList;
	igdeTListenerList<Listener> pListeners;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/

	/** \brief Create path meta property with label and description. */
	igdeMetaPropertyPath(const char *id, const char *name, const char *description,
		igdeEnvironment::eFilePatternListTypes resourceType);
	
	/** \brief Create path meta property with label, description and custom file pattern list. */
	igdeMetaPropertyPath(const char *id, const char *name, const char *description,
		const igdeFilePattern::List &customPatternList);
	
protected:
	/** \brief Clean up path meta property. */
	~igdeMetaPropertyPath() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Resource type. */
	inline igdeEnvironment::eFilePatternListTypes GetResourceType() const{ return pResourceType; }
	
	/** \brief Custom pattern list. */
	inline const igdeFilePattern::List &GetCustomPatternList() const{ return pCustomPatternList; }
	
	
	/** \brief Listeners. */
	inline igdeTListenerList<Listener> &GetListeners(){ return pListeners; }
	inline const igdeTListenerList<Listener> &GetListeners() const{ return pListeners; }
	
	/** \brief Notify listeners about value change. */
	void NotifyValueChanged(const igdeMetaContext::Ref &context);
	
	
	/**
	 * \brief Create UI widget.
	 * 
	 * This object is able to add itself to a widget holder in the appropriate way.
	 */
	deTObjectReference<igdeMetaPropertyWidget> CreateWidget(const igdeMetaContext::Ref &context) override;
	
	
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
	/*@}*/
};

#endif
