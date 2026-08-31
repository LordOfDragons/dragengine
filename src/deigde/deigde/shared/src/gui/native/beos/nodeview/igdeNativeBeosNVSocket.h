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

#ifndef _IGDENATIBESOSNVSOCKET_H_
#define _IGDENATIBESOSNVSOCKET_H_

#include <interface/View.h>
#include <dragengine/common/math/decMath.h>

class igdeGuiTheme;


/**
 * \brief BeOS native NodeView socket widget.
 * 
 * Circular socket widget used as connector on NVSlot.
 */
class igdeNativeBeosNVSocket : public BView{
private:
	bool pChecked;
	int pRadius;
	rgb_color pColorFrame;
	rgb_color pColorFillChecked;
	rgb_color pColorFillUnchecked;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeBeosNVSocket(BView *parent, const igdeGuiTheme &guitheme);
	
	/** \brief Clean up native widget. */
	~igdeNativeBeosNVSocket() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Checked state (socket is linked). */
	inline bool GetChecked() const{ return pChecked; }
	
	/** \brief Set checked state. */
	void SetChecked(bool checked);
	
	/** \brief Frame color. */
	inline rgb_color GetColorFrame() const{ return pColorFrame; }
	
	/** \brief Set frame color. */
	void SetColorFrame(rgb_color color);
	
	/** \brief Checked fill color. */
	inline rgb_color GetColorFillChecked() const{ return pColorFillChecked; }
	
	/** \brief Set checked fill color. */
	void SetColorFillChecked(rgb_color color);
	
	/** \brief Unchecked fill color. */
	inline rgb_color GetColorFillUnchecked() const{ return pColorFillUnchecked; }
	
	/** \brief Set unchecked fill color. */
	void SetColorFillUnchecked(rgb_color color);
	
	/** \brief Socket radius. */
	inline int GetRadius() const{ return pRadius; }
	
	
	
	/** \brief Center of socket in local coordinates. */
	decPoint GetCenter() const;
	
	/** \brief Left border of socket in local coordinates. */
	decPoint GetBorderLeft() const;
	
	/** \brief Right border of socket in local coordinates. */
	decPoint GetBorderRight() const;
	
	/** \brief Point is inside socket circle. */
	bool IsInsideSocket(const decPoint &point) const;
	/*@}*/
	
	
	
	/** \name BView overrides */
	/*@{*/
	void Draw(BRect updateRect) override;
	BSize PreferredSize() override;
	/*@}*/
};

#endif
