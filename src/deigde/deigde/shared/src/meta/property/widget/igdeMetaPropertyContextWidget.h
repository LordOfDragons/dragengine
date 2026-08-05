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

#ifndef _IGDEMETAPROPERTYCONTEXTWIDGET_H_
#define _IGDEMETAPROPERTYCONTEXTWIDGET_H_

#include "igdeMetaPropertyWidget.h"
#include "../igdeMetaPropertyContext.h"
#include "../igdeMetaPropertyGroup.h"
#include "../../../gui/properties/igdeWPMetaContext.h"


/**
 * \brief Context meta property widget class able to add itself to a widget holder.
 */
class DE_DLL_EXPORT igdeMetaPropertyContextWidget : public igdeMetaPropertyWidget{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyContextWidget>;
	
	
private:
	class PropertyListener : public igdeMetaPropertyContext::Listener{
		igdeMetaPropertyContextWidget &pWidget;
		
	public:
		using Ref = deTObjectReference<PropertyListener>;
		PropertyListener(igdeMetaPropertyContextWidget &widget);
		
	protected:
		~PropertyListener() override;
		
	public:
		void OnValueChanged(igdeMetaPropertyContext *property, const igdeMetaContext::Ref &context) override;
	};
	
	
	igdeMetaPropertyContext &pPropertyContext;
	PropertyListener::Ref pPropertyListener;
	
	igdeFilter pFilter;
	
	igdeMetaContext::Ref pValueContext;
	igdeMetaContext::PropertyList::Ref pProperties, pPropertyProperties;
	igdeMetaPropertyWidget::List pPropertyWidgets;
	igdeWPMetaContext::PropertyWidgetCache pPropertyWidgetCache;
	
	igdeContainer::Ref pContainer;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create meta property widget for property.
	 */
	explicit igdeMetaPropertyContextWidget(igdeMetaPropertyContext &property);
	
protected:
	/** \brief Clean up widget. */
	~igdeMetaPropertyContextWidget() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Assigned property context. */
	inline igdeMetaPropertyContext &GetPropertyContext() const{ return pPropertyContext; }
	
	
	/** \brief Create UI widgets adding them to container. */
	void Create(Builder &builder, bool noLabel) override;
	
	/** \brief Drop UI widgets. */
	void Drop() override;
	
	/** \brief Update UI widgets with current property values. */
	void Update() override;
	
	/** \brief Filter widget. */
	void Filter(const igdeFilter &filter) override;
	
	igdeEnvironment &GetEnvironment() const override;
	bool IsPropertyValid() const override;
	/*@}*/
	
	
protected:
	void UpdateFilteredOut() override;
	void OnContextChanged() override;
	void pUpdatePropertyWidgets(Builder &builder);
	void pClearPropertyWidgets();
	void pFilterPropertyWidgets();
};

#endif
