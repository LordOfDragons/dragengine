/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _IGDENATIVENULLNVSLOT_H_
#define _IGDENATIVENULLNVSLOT_H_

#include <dragengine/common/math/decMath.h>
#include "../../../nodeview/igdeNVSlot.h"
class igdeNativeNullNVSocket;


/**
 * Null NodeView Slot.
 */
class igdeNativeNullNVSlot : public igdeNVSlot::cNativeNVSlot{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeNullNVSlot();
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeNullNVSlot();
	
	/** \brief Create native widget. */
	static igdeNativeNullNVSlot* CreateNativeWidget(igdeNVSlot &owner);
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Update text from widget. */
	void UpdateText() override;
	
	/** \brief Update description from widget. */
	void UpdateDescription() override;
	
	/** \brief Update enabled from widget. */
	void UpdateEnabled() override;
	
	/** \brief Update color from widget. */
	void UpdateColor() override;
	
	/** \brief Update linked state from widget. */
	void UpdateLinkedState() override;
	
	
	
	/** \brief Center of slot in local window coordinates. */
	decPoint GetCenter() const override;
	
	/** \brief Center of slot in owner node coordinates.. */
	decPoint GetCenterNode() const override;
	
	/** \brief Center of slot in owner board coordinates.. */
	decPoint GetCenterBoard() const override;
	
	/** \brief Connector position of slot in local window coordinates. */
	decPoint GetConnector() const override;
	
	/** \brief Connector position of slot in owner node coordinates.. */
	decPoint GetConnectorNode() const override;
	
	/** \brief Connector position of slot in owner board coordinates.. */
	decPoint GetConnectorBoard() const override;
	/*@}*/
};

typedef igdeNativeNullNVSlot igdeNativeNVSlot;

#endif
