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

#ifndef _IGDEMETAPROPERTYVectorWIDGET_H_
#define _IGDEMETAPROPERTYVectorWIDGET_H_

#include "igdeMetaPropertyWidget.h"
#include "../igdeMetaPropertyVector.h"
#include "../../../gui/igdeLabel.h"
#include "../../../gui/composed/igdeEditVector.h"
#include "../../../gui/composed/igdeEditVectorListener.h"


/**
 * \brief Vector meta property widget class able to add itself to a widget holder.
 */
class DE_DLL_EXPORT igdeMetaPropertyVectorWidget : public igdeMetaPropertyWidget{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyVectorWidget>;
	
	
private:
	class PropertyListener : public igdeMetaPropertyVector::Listener{
		igdeMetaPropertyVectorWidget &pWidget;
		
	public:
		using Ref = deTObjectReference<PropertyListener>;
		PropertyListener(igdeMetaPropertyVectorWidget &widget);
		
	protected:
		~PropertyListener() override;
		
	public:
		void OnValueChanged(igdeMetaPropertyVector *property, const igdeMetaContext::Ref &context) override;
	};
	
	
	igdeMetaPropertyVector &pPropertyVector;
	PropertyListener::Ref pPropertyListener;
	igdeEditVector::Ref pEditVector;
	igdeEditVectorListener::Ref pListener;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create meta property widget for property.
	 */
	explicit igdeMetaPropertyVectorWidget(igdeMetaPropertyVector &property);
	
protected:
	/** \brief Clean up widget. */
	~igdeMetaPropertyVectorWidget() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Assigned property Vector. */
	inline igdeMetaPropertyVector &GetPropertyVector() const{ return pPropertyVector; }
	
	
	/** \brief Create UI widgets adding them to container. */
	void Create(Builder &builder, bool noLabel) override;
	
	/** \brief Drop UI widgets. */
	void Drop() override;
	
	/** \brief Update UI widgets with current property values. */
	void Update() override;
	
	
	/** \brief Edit vector widget or nullptr. */
	inline const igdeEditVector::Ref &GetEditVector() const{ return pEditVector; }
	
	void AddContextMenuEntries(igdeMenuCascade &contextMenu) override;
	bool IsPropertyValid() const override;
	/*@}*/
	
	
protected:
	void OnContextChanged() override;
};

#endif
