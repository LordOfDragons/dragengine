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

#ifndef _DEOXRDEVICECOMPONENT_H_
#define _DEOXRDEVICECOMPONENT_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/input/deInputDeviceComponent.h>
#include <dragengine/resources/image/deImage.h>

class deoxrDevice;


/**
 * VR input device component.
 */
class deoxrDeviceComponent : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoxrDeviceComponent> Ref;
	
	
private:
	deoxrDevice &pDevice;
	
	int pIndex;
	
	decString pID;
	decString pName;
	deInputDeviceComponent::eComponentTypes pType;
	
	deImage::Ref pDisplayImage;
	decObjectOrderedSet pDisplayIcons;
	decString pDisplayText;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device component. */
	deoxrDeviceComponent(deoxrDevice &device);
	
protected:
	/** Clean up device component. */
	virtual ~deoxrDeviceComponent();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** Device. */
	inline deoxrDevice &GetDevice() const{ return pDevice; }
	
	/** Index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** Set index. */
	void SetIndex(int index);
	
	
	
	/** Identifier. */
	inline const decString &GetID() const{ return pID; }
	
	/** Set identifier. */
	void SetID(const char *id);
	
	/** Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Set name. */
	void SetName(const char *name);
	
	/** Type. */
	inline deInputDeviceComponent::eComponentTypes GetType() const{ return pType; }
	
	/** Set type. */
	void SetType(deInputDeviceComponent::eComponentTypes type);
	
	
	
	/** Display image. */
	inline deImage *GetDisplayImage() const{ return pDisplayImage; }
	
	/** Display icons (deImage*). */
	inline const decObjectOrderedSet &GetDisplayIcons() const{ return pDisplayIcons; }
	
	/** Set display image and icons. */
	void SetDisplayImages(const char *name);
	
	/** Set display image and icons. */
	void SetDisplayImages(const deoxrDeviceComponent &component);
	
	/** Display text. */
	inline const decString &GetDisplayText() const{ return pDisplayText; }
	
	/** Set display text. */
	void SetDisplayText(const char *text);
	
	
	
	/** Update engine input device information component. */
	void GetInfo(deInputDeviceComponent &info) const;
	/*@}*/
};

#endif
