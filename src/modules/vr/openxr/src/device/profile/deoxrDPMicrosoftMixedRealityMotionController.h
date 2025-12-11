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

#ifndef _DEOXRDPMICROSOFTMIXEDREALITYMOTIONCONTROLLER_H_
#define _DEOXRDPMICROSOFTMIXEDREALITYMOTIONCONTROLLER_H_

#include "deoxrDPBaseTwoHandController.h"


/**
 * Microsoft mixed reality motion controller profile.
 */
class deoxrDPMicrosoftMixedRealityMotionController : public deoxrDPBaseTwoHandController{
public:
	typedef deTObjectReference<deoxrDPMicrosoftMixedRealityMotionController> Ref;
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device profile. */
	deoxrDPMicrosoftMixedRealityMotionController(deoxrInstance &instance);
	
protected:
	/** Create device profile. */
	deoxrDPMicrosoftMixedRealityMotionController(
		deoxrInstance &instance, const deoxrPath &path, const char *name);
	
	/** Clean up device profile. */
	~deoxrDPMicrosoftMixedRealityMotionController() override;
	/*@}*/
	
	
protected:
	virtual const char *pDeviceIdPrefix() const;
	void pSuggestBindings() override;
	void pAddDevice(bool left) override;
};

#endif

