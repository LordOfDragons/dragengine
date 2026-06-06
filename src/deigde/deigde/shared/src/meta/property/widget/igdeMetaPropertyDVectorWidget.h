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

#ifndef _IGDEMETAPROPERTYDVECTORWIDGET_H_
#define _IGDEMETAPROPERTYDVECTORWIDGET_H_

#include "igdeMetaPropertyWidget.h"
#include "../igdeMetaPropertyDVector.h"
#include "../../../gui/igdeLabel.h"
#include "../../../gui/composed/igdeEditDVector.h"
#include "../../../gui/composed/igdeEditDVectorListener.h"


/**
 * \brief DVector meta property widget class able to add itself to a widget holder.
 */
class DE_DLL_EXPORT igdeMetaPropertyDVectorWidget : public igdeMetaPropertyWidget{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyDVectorWidget>;
	
	
private:
	class PropertyListener : public igdeMetaPropertyDVector::Listener{
		igdeMetaPropertyDVectorWidget &pWidget;
		
	public:
		using Ref = deTObjectReference<PropertyListener>;
		PropertyListener(igdeMetaPropertyDVectorWidget &widget);
		
	protected:
		virtual ~PropertyListener() override;
		
	public:
		void OnValueChanged(igdeMetaPropertyDVector *property, const igdeMetaContext::Ref &context) override;
		void OnPropertyContextChanged(igdeMetaPropertyDVector *property, const igdeMetaContext::Ref &context) override;
	};
	
	
	igdeMetaPropertyDVector &pPropertyDVector;
	PropertyListener::Ref pPropertyListener;
	igdeEditDVector::Ref pEditDVector;
	igdeEditDVectorListener::Ref pListener;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create meta property widget for property and context.
	 */
	igdeMetaPropertyDVectorWidget(igdeMetaPropertyDVector &property);
	
protected:
	/** \brief Clean up widget. */
	~igdeMetaPropertyDVectorWidget() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Assigned property dvector. */
	inline igdeMetaPropertyDVector &GetPropertyDVector() const{ return pPropertyDVector; }
	
	
	/** \brief Create UI widgets adding them to container. */
	void Create(igdeContainer &container, igdeUIHelper &helper) override;
	
	/** \brief Drop UI widgets. */
	void Drop() override;
	
	/** \brief Update UI widgets with current property values. */
	void Update() override;
	
	
	/** \brief Edit dvector widget or nullptr. */
	inline const igdeEditDVector::Ref &GetEditDVector() const{ return pEditDVector; }
	/*@}*/
	
	
protected:
	void AddContextMenuEntries(igdeMenuCascade &contextMenu) override;
	void UpdateFilteredOut() override;
};

#endif
