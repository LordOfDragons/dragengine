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

#ifndef _IGDEMETAPROPERTYTOGGLETAGSWIDGET_H_
#define _IGDEMETAPROPERTYTOGGLETAGSWIDGET_H_

#include "igdeMetaPropertyWidget.h"
#include "../igdeMetaPropertyToggleTags.h"
#include "../../../gui/igdeLabel.h"
#include "../../../gui/composed/igdeToggleTags.h"
#include "../../../gui/event/igdeAction.h"


/**
 * \brief Toggle tags meta property widget class able to add itself to a widget holder.
 */
class DE_DLL_EXPORT igdeMetaPropertyToggleTagsWidget : public igdeMetaPropertyWidget{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyToggleTagsWidget>;
	
	
private:
	class PropertyListener : public igdeMetaPropertyToggleTags::Listener{
		igdeMetaPropertyToggleTagsWidget &pWidget;
		
	public:
		using Ref = deTObjectReference<PropertyListener>;
		PropertyListener(igdeMetaPropertyToggleTagsWidget &widget);
		
	protected:
		~PropertyListener() override;
		
	public:
		void OnValueChanged(igdeMetaPropertyToggleTags *property, const igdeMetaContext::Ref &context) override;
	};
	
	
	igdeMetaPropertyToggleTags &pPropertyToggleTags;
	PropertyListener::Ref pPropertyListener;
	igdeToggleTags::Ref pToggleTags;
	igdeAction::Ref pAction;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create meta property widget for property.
	 */
	explicit igdeMetaPropertyToggleTagsWidget(igdeMetaPropertyToggleTags &property);
	
protected:
	/** \brief Clean up widget. */
	~igdeMetaPropertyToggleTagsWidget() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Assigned property toggle tags. */
	inline igdeMetaPropertyToggleTags &GetPropertyToggleTags() const{ return pPropertyToggleTags; }
	
	
	/** \brief Create UI widgets adding them to container. */
	void Create(igdeContainer &container, igdeUIHelper &helper, bool noLabel) override;
	
	/** \brief Drop UI widgets. */
	void Drop() override;
	
	/** \brief Update UI widgets with current property values. */
	void Update() override;
	
	
	/** \brief Toggle tags widget or nullptr. */
	inline const igdeToggleTags::Ref &GetToggleTags() const{ return pToggleTags; }
	
	void AddContextMenuEntries(igdeMenuCascade &contextMenu) override;
	/*@}*/
	
	
protected:
	void OnContextChanged() override;
};

#endif
