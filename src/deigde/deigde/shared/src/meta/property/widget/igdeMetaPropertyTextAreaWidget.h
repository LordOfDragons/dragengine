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

#ifndef _IGDEMETAPROPERTYTEXTAREAWIDGET_H_
#define _IGDEMETAPROPERTYTEXTAREAWIDGET_H_

#include "igdeMetaPropertyWidget.h"
#include "../igdeMetaPropertyTextArea.h"
#include "../../../gui/igdeLabel.h"
#include "../../../gui/igdeTextArea.h"
#include "../../../gui/event/igdeTextAreaListener.h"


/**
 * \brief Text area meta property widget class able to add itself to a widget holder.
 */
class DE_DLL_EXPORT igdeMetaPropertyTextAreaWidget : public igdeMetaPropertyWidget{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyTextAreaWidget>;
	
	
private:
	class PropertyListener : public igdeMetaPropertyTextArea::Listener{
		igdeMetaPropertyTextAreaWidget &pWidget;
		
	public:
		using Ref = deTObjectReference<PropertyListener>;
		PropertyListener(igdeMetaPropertyTextAreaWidget &widget);
		
	protected:
		~PropertyListener() override;
		
	public:
		void OnValueChanged(igdeMetaPropertyTextArea *property, const igdeMetaContext::Ref &context) override;
	};
	
	
	igdeMetaPropertyTextArea &pPropertyTextArea;
	PropertyListener::Ref pPropertyListener;
	igdeTextArea::Ref pTextArea;
	igdeTextAreaListener::Ref pListener;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create meta property widget for property.
	 */
	explicit igdeMetaPropertyTextAreaWidget(igdeMetaPropertyTextArea &property);
	
protected:
	/** \brief Clean up widget. */
	~igdeMetaPropertyTextAreaWidget() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Assigned property text area. */
	inline igdeMetaPropertyTextArea &GetPropertyTextArea() const{ return pPropertyTextArea; }
	
	
	/** \brief Create UI widgets adding them to container. */
	void Create(igdeContainer &container, igdeUIHelper &helper, bool noLabel) override;
	
	/** \brief Drop UI widgets. */
	void Drop() override;
	
	/** \brief Update UI widgets with current property values. */
	void Update() override;
	
	
	/** \brief Text area widget or nullptr. */
	inline const igdeTextArea::Ref &GetTextArea() const{ return pTextArea; }
	
	void AddContextMenuEntries(igdeMenuCascade &contextMenu) override;
	/*@}*/
	
	
protected:
	void OnContextChanged() override;
};

#endif
