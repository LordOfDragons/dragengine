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

#ifndef _IGDEWPCAMERA_H_
#define _IGDEWPCAMERA_H_

#include <dragengine/common/math/decMath.h>

#include "../igdeTextFieldReference.h"
#include "../igdeCheckBoxReference.h"
#include "../igdeButtonReference.h"
#include "../composed/igdeEditVectorReference.h"
#include "../composed/igdeEditSliderTextReference.h"
#include "../event/igdeActionListener.h"
#include "../event/igdeAction::Ref.h"
#include "../filedialog/igdeFilePatternList.h"
#include "../layout/igdeContainerFlow.h"
#include "../curveedit/igdeViewCurveBezierReference.h"


class igdeCamera;


/**
 * Window Properties Camera.
 * 
 * Provides support for configurating a wrapper camera. This panel is supposed to be used
 * on a properties window typically inside a group box. An camera wrapper is useful for
 * editors to provide a static environments or adding cameras to an environment to test
 * features of whatever they are editing. The camera wrapper makes it easy to setup an
 * camera using a game definition camera class. In addition properties of the wrapped camera
 * can be set to configured certain camera features. To use this class assign an wrapper
 * camera. Whenever a change is made the set action listener is called. The camera wrapper
 * camera is not held in any way. The application is responsible to ensure the wrapper
 * camera exists as long as it is assigned to this widget.
 */
class DE_DLL_EXPORT igdeWPCamera : public igdeContainerFlow, igdeActionListener{
private:
	igdeCamera *pCamera;
	
	igdeEditVectorReference pEditPosition;
	igdeEditVectorReference pEditRotation;
	igdeEditVectorReference pEditViewDir;
	igdeButtonReference pBtnCamera;
	igdeTextFieldReference pEditOrbitDistance;
	igdeTextFieldReference pEditFov;
	igdeTextFieldReference pEditFovRatio;
	igdeTextFieldReference pEditImageDist;
	igdeTextFieldReference pEditViewDist;
	igdeTextFieldReference pEditExposure;
	igdeTextFieldReference pEditLowInt;
	igdeTextFieldReference pEditHiInt;
	igdeTextFieldReference pEditAdaptTime;
	igdeCheckBoxReference pChkEnableHDRR;
	igdeCheckBoxReference pChkEnableGI;
	igdeTextFieldReference pEditWhiteIntensity;
	igdeTextFieldReference pEditBloomIntensity;
	igdeTextFieldReference pEditBloomStrength;
	igdeEditSliderTextReference pSldBloomSize;
	igdeEditSliderTextReference pSldBloomBlend;
	igdeViewCurveBezierReference pEditToneMapCurve;
	
	igdeAction::Ref pAction;
	
	
	
public:
	static decString lastCameraFile;
	static const igdeFilePatternList patternCamera;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create panel. */
	igdeWPCamera( igdeEnvironment &environment );
	
	/** Create panel. */
	igdeWPCamera( igdeEnvironment &environment, igdeAction *action );
	
	
	
protected:
	/** Clean up panel. */
	virtual ~igdeWPCamera();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Camera or nullptr if not set. */
	inline igdeCamera *GetCamera() const{ return pCamera; }
	
	/** Set camera or nullptr if not set. */
	void SetCamera( igdeCamera *camera );
	
	/** Update widget after camera changed outside. */
	void UpdateCamera();
	
	/** Update camera view direction only. */
	void UpdateViewDirection();
	
	/** Set tone mapping curve range from white intensity. */
	void SetToneMapCurveRangeFromWhiteIntensity();
	
	
	
	/** Action or nullptr. */
	inline igdeAction *GetAction() const{ return pAction; }
	
	/** Set action or nullptr. */
	void SetAction( igdeAction *action );
	
	/**
	 * Sky parameters changed.
	 * 
	 * Called if user changes any of the sky parameters. Default implementation calls
	 * OnAction on set action camera if present.
	 */
	virtual void OnAction();
	
	/** Action parameters changed. */
	virtual void OnParameterChanged( igdeAction *action );
	
	/** Action has been destroyed. */
	virtual void OnDestroyed( igdeAction *action );
	/*@}*/
	
	
	
private:
	void pCreateContent();
};

#endif
