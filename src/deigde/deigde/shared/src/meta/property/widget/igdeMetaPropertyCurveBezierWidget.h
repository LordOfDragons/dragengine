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

#ifndef _IGDEMETAPROPERTYCURVEBEZIERWIDGET_H_
#define _IGDEMETAPROPERTYCURVEBEZIERWIDGET_H_

#include "igdeMetaPropertyWidget.h"
#include "../igdeMetaPropertyCurveBezier.h"
#include "../undo/igdeMetaPropertyCurveBezierUndo.h"
#include "../../../gui/igdeLabel.h"
#include "../../../gui/curveedit/igdeViewCurveBezier.h"
#include "../../../gui/curveedit/igdeViewCurveBezierListener.h"


/**
 * \brief Curve bezier meta property widget class able to add itself to a widget holder.
 */
class DE_DLL_EXPORT igdeMetaPropertyCurveBezierWidget : public igdeMetaPropertyWidget{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyCurveBezierWidget>;
	
	
private:
	class PropertyListener : public igdeMetaPropertyCurveBezier::Listener{
		igdeMetaPropertyCurveBezierWidget &pWidget;
		
	public:
		using Ref = deTObjectReference<PropertyListener>;
		PropertyListener(igdeMetaPropertyCurveBezierWidget &widget);
		
	protected:
		~PropertyListener() override;
		
	public:
		void OnValueChanged(igdeMetaPropertyCurveBezier *property, const igdeMetaContext::Ref &context) override;
	};
	
	
	igdeMetaPropertyCurveBezier &pPropertyCurveBezier;
	PropertyListener::Ref pPropertyListener;
	igdeViewCurveBezier::Ref pViewCurveBezier;
	igdeViewCurveBezierListener::Ref pListener;
	igdeMetaPropertyCurveBezierUndo::Ref pUndoDragging;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create meta property widget for property.
	 */
	explicit igdeMetaPropertyCurveBezierWidget(igdeMetaPropertyCurveBezier &property);
	
protected:
	/** \brief Clean up widget. */
	~igdeMetaPropertyCurveBezierWidget() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Assigned property curve bezier. */
	inline igdeMetaPropertyCurveBezier &GetPropertyCurveBezier() const{ return pPropertyCurveBezier; }
	
	
	/** \brief Create UI widgets adding them to container. */
	void Create(igdeContainer &container, igdeUIHelper &helper, bool noLabel) override;
	
	/** \brief Drop UI widgets. */
	void Drop() override;
	
	/** \brief Update UI widgets with current property values. */
	void Update() override;
	
	
	/** \brief View curve bezier widget or nullptr. */
	inline const igdeViewCurveBezier::Ref &GetViewCurveBezier() const{ return pViewCurveBezier; }
	
	/** \brief Dragging undo or nullptr. */
	inline const igdeMetaPropertyCurveBezierUndo::Ref &GetUndoDragging() const{ return pUndoDragging; }
	
	/** \brief Set dragging undo or nullptr. */
	void SetUndoDragging(const igdeMetaPropertyCurveBezierUndo::Ref &undo);
	
	/** \brief Add curve edit context menu entries. */
	void AddCurveEditContextMenuEntries(igdeMenuCascade &menu);
	
	void AddContextMenuEntries(igdeMenuCascade &contextMenu) override;
	void AddContextMenuEntriesEditInDialog(igdeMenuCascade &contextMenu);
	
	/** \brief Open dialog to edit curve in large view. */
	void EditInDialog();
	/*@}*/
	
	
protected:
	void OnContextChanged() override;
};

#endif
