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

#ifndef _DEAINPDEVICEAXIS_H_
#define _DEAINPDEVICEAXIS_H_

#include <unistd.h>

#include <dragengine/common/string/decString.h>
#include <dragengine/input/deInputDeviceAxis.h>

class deainpDevice;
class deInputDeviceAxis;



/**
 * \brief Android input device axis.
 */
class deainpDeviceAxis{
private:
	int pIndex;
	decString pID;
	decString pName;
	deInputDeviceAxis::eAxisTypes pType;
	
	int pMinimum;
	int pMaximum;
	int pFuzz;
	int pFlat;
	bool pAbsolute;
	bool pWheelOtherAxis;
	
	float pValue;
	float pChangedValue;
	timeval pLastEventTime;
	
	int pAICode;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create device axis. */
	deainpDeviceAxis();
	
	/** \brief Clean up device axis. */
	~deainpDeviceAxis();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/** \brief Index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** \brief Set index. */
	void SetIndex(int index);
	
	/** \brief Identifier. */
	inline const decString &GetID() const{ return pID; }
	
	/** \brief Set identifier. */
	void SetID(const char *id);
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName(const char *name);
	
	/** \brief Type. */
	inline deInputDeviceAxis::eAxisTypes GetType() const{ return pType; }
	
	/** \brief Set type. */
	void SetType(deInputDeviceAxis::eAxisTypes type);
	
	
	
	/** \brief Minimum value. */
	inline int GetMinimum() const{ return pMinimum; }
	
	/** \brief Set minimum value. */
	void SetMinimum(int minimum);
	
	/** \brief Maximum value. */
	inline int GetMaximum() const{ return pMaximum; }
	
	/** \brief Set maximum value. */
	void SetMaximum(int maximum);
	
	/** \brief Fuzz. */
	inline int GetFuzz() const{ return pFuzz; }
	
	/** \brief Set fuzz. */
	void SetFuzz(int fuzz);
	
	/** \brief Flat. */
	inline int GetFlat() const{ return pFlat; }
	
	/** \brief Set flat. */
	void SetFlat(int flat);
	
	/** \brief Axis is absolute. */
	inline bool GetAbsolute() const{ return pAbsolute; }
	
	/** \brief Set if axis is absolute. */
	void SetAbsolute(bool absolute);
	
	/** \brief Wheel around other axis. */
	inline bool GetWheelOtherAxis() const{ return pWheelOtherAxis; }
	
	/** \brief Set to wheel around other axis. */
	void SetWheelOtherAxis(bool otherAxis);
	
	
	/** \brief Value. */
	inline float GetValue() const{ return pValue; }
	
	/** \brief Set value. */
	void SetValue(float value);
	
	
	
	/** \brief Android specific code. */
	inline int GetAICode() const{ return pAICode; }
	
	/** \brief Set Android specific code. */
	void SetAICode(int code);
	
	
	
	/** \brief Update engine input device information axis. */
	void GetInfo(deInputDeviceAxis &info) const;
	
	/** \brief Send events if present. */
	void SendEvents(deainpDevice &device);
	/*@}*/
};

#endif
