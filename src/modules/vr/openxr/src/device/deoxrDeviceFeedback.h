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

#ifndef _DEOXRDEVICEFEEDBACK_H_
#define _DEOXRDEVICEFEEDBACK_H_

#include "deoxrDeviceComponent.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/input/deInputDeviceFeedback.h>
#include <dragengine/resources/image/deImage.h>

class deVROpenXR;



/**
 * VR input device feedback.
 */
class deoxrDeviceFeedback : public deObject{
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoxrDeviceFeedback> Ref;


private:
	deVROpenXR &pOxr;
	
	int pIndex;
	decString pID;
	decString pName;
	deInputDeviceFeedback::eFeedbackTypes pType;
	deoxrDeviceComponent::Ref pInputDeviceComponent;
	
	deImage::Ref pDisplayImage;
	decObjectOrderedSet pDisplayIcons;
	decString pDisplayText;
	
	int pMaximum;
	
	float pValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device feedback. */
	deoxrDeviceFeedback( deVROpenXR &oxr );
	
protected:
	/** Clean up device feedback. */
	virtual ~deoxrDeviceFeedback();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** Oxr. */
	inline deVROpenXR &GetOxr() const{ return pOxr; }
	
	/** Index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** Set index. */
	void SetIndex( int index );
	
	/** Identifier. */
	inline const decString &GetID() const{ return pID; }
	
	/** Set identifier. */
	void SetID( const char *id );
	
	/** Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Set name. */
	void SetName( const char *name );
	
	/** Type. */
	inline deInputDeviceFeedback::eFeedbackTypes GetType() const{ return pType; }
	
	/** Set type. */
	void SetType( deInputDeviceFeedback::eFeedbackTypes type );
	
	/** Input device component. */
	inline deoxrDeviceComponent *GetInputDeviceComponent() const{ return pInputDeviceComponent; }
	
	/** Set input device component. */
	void SetInputDeviceComponent( deoxrDeviceComponent *component );
	
	
	
	/** Display image. */
	inline deImage *GetDisplayImage() const{ return pDisplayImage; }
	
	/** Display icons (deImage*). */
	inline const decObjectOrderedSet &GetDisplayIcons() const{ return pDisplayIcons; }
	
	/** Set display image and icons. */
	void SetDisplayImages( const char *name );
	
	/** Display text. */
	inline const decString &GetDisplayText() const{ return pDisplayText; }
	
	/** Set display text. */
	void SetDisplayText( const char *text );
	
	
	
	/** Maximum value. */
	inline int GetMaximum() const{ return pMaximum; }
	
	/** Set maximum value. */
	void SetMaximum( int maximum );
	
	
	
	/** Current value. */
	inline float GetValue() const{ return pValue; }
	
	/** Set current value. */
	void SetValue( float value );
	
	
	
	/** Update engine input device information feedback. */
	void GetInfo( deInputDeviceFeedback &info ) const;
	/*@}*/
};

#endif
