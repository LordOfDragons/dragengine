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

#ifndef _DEBIDEVICEAXIS_H_
#define _DEBIDEVICEAXIS_H_

#include <sys/time.h>

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/input/deInputDeviceAxis.h>
#include <dragengine/resources/image/deImage.h>

class deBeOSInput;
class debiDevice;
class deInputDeviceAxis;



/**
 * \brief Input device axis.
 */
class debiDeviceAxis : public deObject{
private:
	deBeOSInput &pModule;
	
	int pIndex;
	decString pID;
	decString pName;
	deInputDeviceAxis::eAxisTypes pType;
	
	deImage::Ref pDisplayImage;
	decObjectOrderedSet pDisplayIcons;
	decString pDisplayText;
	
	int pMinimum;
	int pMaximum;
	int pFuzz;
	int pFlat;
	bool pAbsolute;
	bool pWheelOtherAxis;
	int pWheelChange;
	
	float pValue;
	float pChangedValue;
	int pLastModifiers;
	timeval pLastEventTime;
	
	int pBICode;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<debiDeviceAxis> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create device axis. */
	debiDeviceAxis(deBeOSInput &module);
	
protected:
	/** \brief Clean up device axis. */
	virtual ~debiDeviceAxis();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** \brief Input module. */
	inline deBeOSInput &GetModule() const{return pModule;}
	
	/** \brief Index. */
	inline int GetIndex() const{return pIndex;}
	
	/** \brief Set index. */
	void SetIndex(int index);
	
	/** \brief Identifier. */
	inline const decString &GetID() const{return pID;}
	
	/** \brief Set identifier. */
	void SetID(const char *id);
	
	/** \brief Name. */
	inline const decString &GetName() const{return pName;}
	
	/** \brief Set name. */
	void SetName(const char *name);
	
	/** \brief Type. */
	inline deInputDeviceAxis::eAxisTypes GetType() const{return pType;}
	
	/** \brief Set type. */
	void SetType(deInputDeviceAxis::eAxisTypes type);
	
	
	
	/** \brief Display image. */
	inline deImage *GetDisplayImage() const{return pDisplayImage;}
	
	/** \brief Display icons (deImage*). */
	inline const decObjectOrderedSet &GetDisplayIcons() const{return pDisplayIcons;}
	
	/** \brief Set display image and icons. */
	void SetDisplayImages(const char *name);
	
	/** \brief Display text. */
	inline const decString &GetDisplayText() const{return pDisplayText;}
	
	/** \brief Set display text. */
	void SetDisplayText(const char *text);
	
	
	
	/** \brief Minimum value. */
	inline int GetMinimum() const{return pMinimum;}
	
	/** \brief Set minimum value. */
	void SetMinimum(int minimum);
	
	/** \brief Maximum value. */
	inline int GetMaximum() const{return pMaximum;}
	
	/** \brief Set maximum value. */
	void SetMaximum(int maximum);
	
	/** \brief Fuzz. */
	inline int GetFuzz() const{return pFuzz;}
	
	/** \brief Set fuzz. */
	void SetFuzz(int fuzz);
	
	/** \brief Flat. */
	inline int GetFlat() const{return pFlat;}
	
	/** \brief Set flat. */
	void SetFlat(int flat);
	
	/** \brief Axis is absolute. */
	inline bool GetAbsolute() const{return pAbsolute;}
	
	/** \brief Set if axis is absolute. */
	void SetAbsolute(bool absolute);
	
	/** \brief Wheel around other axis. */
	inline bool GetWheelOtherAxis() const{return pWheelOtherAxis;}
	
	/** \brief Set to wheel around other axis. */
	void SetWheelOtherAxis(bool otherAxis);
	
	/** \brief Wheel changed value. */
	inline int GetWheelChange() const{return pWheelChange;}
	
	/** \brief Set wheel change value. */
	void SetWheelChange(int change, int modifiers, const timeval &eventTime);
	
	/** \brief Increment wheel change value. */
	void IncrementWheelChange(int amount, int modifiers, const timeval &eventTime);
	
	/** \brief Value. */
	inline float GetValue() const{return pValue;}
	
	/** \brief Set value. */
	void SetValue(float value);
	
	
	
	/** \brief BeOS input code. */
	inline int GetBICode() const{return pBICode;}
	
	/** \brief Set BeOS input code. */
	void SetBICode(int code);
	
	
	
	/** \brief Update engine input device information axis. */
	void GetInfo(deInputDeviceAxis &info) const;
	
	/** \brief Process new value dirty axis if changed enough. */
	void ProcessNewValue(debiDevice &device, float newValue, const timeval &eventTime);
	
	/** \brief Send events if required. */
	void SendEvents(debiDevice &device);
	/*@}*/
};

#endif
