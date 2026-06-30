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

#ifndef _IGDEMETAPROPERTYSLIDERBOARDWIDGET_H_
#define _IGDEMETAPROPERTYSLIDERBOARDWIDGET_H_

#include "igdeMetaPropertyWidget.h"
#include "../igdeMetaPropertySliderBoard.h"
#include "../../../gui/igdeLabel.h"
#include "../../../gui/composed/igdeEditSliderText.h"
#include "../../../gui/composed/igdeEditSliderTextListener.h"
#include "../../../gui/layout/igdeContainerForm.h"

#include <dragengine/common/collection/decTList.h>


/**
 * \brief Set meta property widget class able to add itself to a widget holder.
 */
class DE_DLL_EXPORT igdeMetaPropertySliderBoardWidget : public igdeMetaPropertyWidget{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertySliderBoardWidget>;
	
	
	/** \brief Slider. */
	class Slider : public deObject{
	public:
		using Ref = deTObjectReference<Slider>;
		using List = decTObjectList<Slider>;
		
		const igdeMetaContext::Ref context;
		const igdeLabel::Ref label;
		const igdeEditSliderText::Ref slider;
		const igdeEditSliderTextListener::Ref listener;
		
		Slider(igdeMetaPropertySliderBoardWidget &widget, const igdeMetaContext::Ref &context,
			const igdeLabel::Ref &label, const igdeEditSliderText::Ref &slider);
		~Slider() override;
	};
	
	
private:
	class PropertyListener : public igdeMetaPropertySliderBoard::Listener{
		igdeMetaPropertySliderBoardWidget &pWidget;
		
	public:
		using Ref = deTObjectReference<PropertyListener>;
		PropertyListener(igdeMetaPropertySliderBoardWidget &widget);
		
	protected:
		~PropertyListener() override;
		
	public:
		void OnValueChanged(igdeMetaPropertySliderBoard *property, const igdeMetaContext::Ref &context) override;
	};
	
	
	class NamePropertyListener : public igdeMetaPropertyString::Listener{
		igdeMetaPropertySliderBoardWidget &pWidget;
		
	public:
		using Ref = deTObjectReference<NamePropertyListener>;
		NamePropertyListener(igdeMetaPropertySliderBoardWidget &widget);
		
	protected:
		~NamePropertyListener() override;
		
	public:
		void OnValueChanged(igdeMetaPropertyString *property, const igdeMetaContext::Ref &context) override;
	};
	
	
	class ValuePropertyListener : public igdeMetaPropertyFloat::Listener{
		igdeMetaPropertySliderBoardWidget &pWidget;
		
	public:
		using Ref = deTObjectReference<ValuePropertyListener>;
		ValuePropertyListener(igdeMetaPropertySliderBoardWidget &widget);
		
	protected:
		~ValuePropertyListener() override;
		
	public:
		void OnValueChanged(igdeMetaPropertyFloat *property, const igdeMetaContext::Ref &context) override;
		void OnLimitsChanged(igdeMetaPropertyFloat *property, const igdeMetaContext::Ref &context) override;
	};
	
	
	igdeMetaPropertySliderBoard &pPropertySliderBoard;
	PropertyListener::Ref pPropertyListener;
	NamePropertyListener::Ref pNamePropertyListener;
	ValuePropertyListener::Ref pValuePropertyListener;
	igdeContainerForm::Ref pContainer;
	Slider::List pSliders;
	igdeMetaPropertyFloatUndo::Ref pUndoSliding;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create meta property widget for property.
	 */
	explicit igdeMetaPropertySliderBoardWidget(igdeMetaPropertySliderBoard &property);
	
protected:
	/** \brief Clean up widget. */
	~igdeMetaPropertySliderBoardWidget() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Assigned property slider board. */
	inline igdeMetaPropertySliderBoard &GetPropertySliderBoard() const{ return pPropertySliderBoard; }
	
	
	/** \brief Create UI widgets adding them to container. */
	void Create(Builder &builder, bool noLabel) override;
	
	/** \brief Drop UI widgets. */
	void Drop() override;
	
	/** \brief Update UI widgets with current property values. */
	void Update() override;
	
	/** \brief Update slider values. */
	void UpdateSliderValues();
	
	/** \brief Update slider names. */
	void UpdateSliderNames();
	
	/** \brief Update slider limits. */
	void UpdateSliderLimits();
	
	/** \brief Sliders. */
	inline const Slider::List &GetSliders() const{ return pSliders; }
	
	/** \brief Context matches sliders. */
	bool HasSliderContext(const igdeMetaContext::Ref &context) const;
	
	/** \brief Sliding undo or nullptr. */
	inline const igdeMetaPropertyFloatUndo::Ref &GetUndoSliding() const{ return pUndoSliding; }
	
	/** \brief Set sliding undo or nullptr. */
	void SetUndoSliding(const igdeMetaPropertyFloatUndo::Ref &undo);
	/*@}*/
	
	
protected:
	void OnContextChanged() override;
	
	
private:
	void pRebuildBoard();
};

#endif
