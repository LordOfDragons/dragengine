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

#ifndef _IGDEMETAPROPERTYGROUPWIDGET_H_
#define _IGDEMETAPROPERTYGROUPWIDGET_H_

#include "igdeMetaPropertyWidget.h"
#include "../igdeMetaPropertyGroup.h"
#include "../../../gui/igdeGroupBox.h"
#include "../../../gui/event/igdeAction.h"


/**
 * \brief Group meta property widget class able to add itself to a widget holder.
 */
class DE_DLL_EXPORT igdeMetaPropertyGroupWidget : public igdeMetaPropertyWidget{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyGroupWidget>;
	
	
private:
	igdeMetaPropertyGroup &pPropertyGroup;
	igdeGroupBox::Ref pGroupBox;
	igdeContainer::Ref pGroupBoxContainer;
	List pChildWidgets;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create meta property widget for property and context.
	 */
	igdeMetaPropertyGroupWidget(igdeMetaPropertyGroup &property, const deTObjectReference<igdeMetaContext> &context);
	
protected:
	/** \brief Clean up widget. */
	~igdeMetaPropertyGroupWidget() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Assigned property group. */
	inline igdeMetaPropertyGroup &GetPropertyGroup() const{ return pPropertyGroup; }
	
	
	/** \brief Create UI widgets adding them to container. */
	void Create(igdeContainer &container, igdeUIHelper &helper) override;
	
	/** \brief Filter widget. */
	void Filter(const igdeFilter &filter) override;
	
	/** \brief Drop UI widgets. */
	void Drop() override;
	
	/** \brief Update UI widgets with current property values. */
	void Update() override;
	
	
	/** \brief Group box or nullptr. */
	inline const igdeGroupBox::Ref &GetGroupBox() const{ return pGroupBox; }
	
	/** \brief Group box container or nullptr. */
	inline const igdeContainer::Ref &GetGroupBoxContainer() const{ return pGroupBoxContainer; }
	
	/** \brief Child widgets. */
	inline List &GetChildWidgets(){ return pChildWidgets; }
	inline const List &GetChildWidgets() const{ return pChildWidgets; }
	/*@}*/
	
	
protected:
	void UpdateFilteredOut() override;
};

#endif
