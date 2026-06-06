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

#ifndef _IGDEMETAPROPERTYBOOLEANWIDGET_H_
#define _IGDEMETAPROPERTYBOOLEANWIDGET_H_

#include "igdeMetaPropertyWidget.h"
#include "../igdeMetaPropertyBoolean.h"
#include "../../../gui/igdeCheckBox.h"
#include "../../../gui/event/igdeAction.h"


/**
 * \brief Boolean meta property widget class able to add itself to a widget holder.
 */
class DE_DLL_EXPORT igdeMetaPropertyBooleanWidget : public igdeMetaPropertyWidget{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyBooleanWidget>;
	
	
private:
	class PropertyListener : public igdeMetaPropertyBoolean::Listener{
		igdeMetaPropertyBooleanWidget &pWidget;
		
	public:
		using Ref = deTObjectReference<PropertyListener>;
		PropertyListener(igdeMetaPropertyBooleanWidget &widget);
		
	protected:
		virtual ~PropertyListener() override;
		
	public:
		void OnValueChanged(igdeMetaPropertyBoolean *property, const igdeMetaContext::Ref &context) override;
		void OnPropertyContextChanged(igdeMetaPropertyBoolean *property, const igdeMetaContext::Ref &context) override;
	};
	
	
	igdeMetaPropertyBoolean &pPropertyBoolean;
	PropertyListener::Ref pPropertyListener;
	igdeCheckBox::Ref pCheckBox;
	igdeAction::Ref pAction;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create meta property widget for property and context.
	 */
	igdeMetaPropertyBooleanWidget(igdeMetaPropertyBoolean &property);
	
protected:
	/** \brief Clean up widget. */
	~igdeMetaPropertyBooleanWidget() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Assigned property boolean. */
	inline igdeMetaPropertyBoolean &GetPropertyBoolean() const{ return pPropertyBoolean; }
	
	
	/** \brief Create UI widgets adding them to container. */
	void Create(igdeContainer &container, igdeUIHelper &helper) override;
	
	/** \brief Drop UI widgets. */
	void Drop() override;
	
	/** \brief Update UI widgets with current property values. */
	void Update() override;
	
	
	/** \brief Boolean box widget or nullptr. */
	inline const igdeCheckBox::Ref &GetBooleanBox() const{ return pCheckBox; }
	/*@}*/
	
	
protected:
	void AddContextMenuEntries(igdeMenuCascade &contextMenu) override;
	void UpdateFilteredOut() override;
};

#endif
