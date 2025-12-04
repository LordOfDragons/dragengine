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

#ifndef _MEWPSENSORS_H_
#define _MEWPSENSORS_H_

#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeColorBox.h>
#include <deigde/gui/composed/igdeEditDVector.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/layout/igdeContainerScroll.h>


class meWindowProperties;



/**
 * \brief Sensors property panel.
 */
class meWPSensors : public igdeContainerScroll{
private:
	meWindowProperties &pWindowProperties;
	
	igdeCheckBox::Ref pChkLMTrackCam;
	igdeEditDVector::Ref pEditLMPos;
	igdeEditVector::Ref pEditLMDir;
	igdeTextField::Ref pEditLMConeIA;
	igdeTextField::Ref pEditLMConeOA;
	igdeTextField::Ref pEditLMConeExp;
	igdeTextField::Ref pEditLMLumi;
	igdeColorBox::Ref pEditLMColor;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	meWPSensors( meWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up panel. */
	virtual ~meWPSensors();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Properties window. */
	inline meWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** \brief Update sensors. */
	void UpdateSensors();
	
	/** \brief Update lumimeters. */
	void UpdateLumimeter();
	/*@}*/
};

#endif
