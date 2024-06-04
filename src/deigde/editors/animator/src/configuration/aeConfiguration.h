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

#ifndef _AECONFIGURATION_H_
#define _AECONFIGURATION_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/input/deInputEvent.h>

class aeWindowMain;



/**
 * Editor Configuration.
 */
class aeConfiguration{
private:
	aeWindowMain &pWindowMain;
	
	deInputEvent::eKeyCodes pLocoKeyForward;
	deInputEvent::eKeyCodes pLocoKeyBackwards;
	deInputEvent::eKeyCodes pLocoKeyLeft;
	deInputEvent::eKeyCodes pLocoKeyRight;
	deInputEvent::eKeyCodes pLocoKeyCrouch;
	deInputEvent::eKeyCodes pLocoKeyRun;
	
	bool pPreventSaving;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create configuration. */
	aeConfiguration( aeWindowMain &windowMain );
	
	/** Clean up configuration. */
	~aeConfiguration();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Main window. */
	inline aeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	
	
	/** Locomotion testing forward key. */
	inline deInputEvent::eKeyCodes GetLocoKeyForward() const{ return pLocoKeyForward; }
	
	/** Set locomotion testing forward key. */
	void SetLocoKeyForward( deInputEvent::eKeyCodes key );
	
	/** Locomotion testing backwards key. */
	inline deInputEvent::eKeyCodes GetLocoKeyBackwards() const{ return pLocoKeyBackwards; }
	
	/** Set locomotion testing backwards key. */
	void SetLocoKeyBackwards( deInputEvent::eKeyCodes key );
	
	/** Locomotion testing left key. */
	inline deInputEvent::eKeyCodes GetLocoKeyLeft() const{ return pLocoKeyLeft; }
	
	/** Set locomotion testing left key. */
	void SetLocoKeyLeft( deInputEvent::eKeyCodes key );
	
	/** Locomotion testing right key. */
	inline deInputEvent::eKeyCodes GetLocoKeyRight() const{ return pLocoKeyRight; }
	
	/** Set locomotion testing right key. */
	void SetLocoKeyRight( deInputEvent::eKeyCodes key );
	
	/** Locomotion testing crouch key. */
	inline deInputEvent::eKeyCodes GetLocoKeyCrouch() const{ return pLocoKeyCrouch; }
	
	/** Set locomotion testing crouch key. */
	void SetLocoKeyCrouch( deInputEvent::eKeyCodes key );
	
	/** Locomotion testing run key. */
	inline deInputEvent::eKeyCodes GetLocoKeyRun() const{ return pLocoKeyRun; }
	
	/** Set locomotion testing run key. */
	void SetLocoKeyRun( deInputEvent::eKeyCodes key );
	
	
	
	/** Prevent saving. */
	inline bool GetPreventSaving() const{ return pPreventSaving; }
	
	/** Set prevent saving. */
	void SetPreventSaving( bool preventSaving );
	
	
	
	/** Load configuration. */
	void LoadConfiguration();
	
	/** Save configuration. */
	void SaveConfiguration();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pReset();
};

#endif
