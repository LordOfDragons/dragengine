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

#ifndef _DEOXRDEBUG_H_
#define _DEOXRDEBUG_H_

#include "deoxrBasics.h"

class deoxrInstance;


/**
 * OpenXR debug support.
 */
class deoxrDebug{
private:
	deoxrInstance &pInstance;
	
	bool pEnabled;
	
	XrDebugUtilsMessengerEXT pMessenger;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create debug. */
	deoxrDebug(deoxrInstance &instance);
	
	/** Clean up debug. */
	~deoxrDebug();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Instance. */
	inline deoxrInstance &GetInstance() const{ return pInstance; }
	
	/** Debug mode is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** Enable or disable debug mode. */
	void SetEnabled(bool enabled);
	
	
	
private:
	void pRegisterReportCallback();
	void pUnregisterReportCallback();
	
	void DebugMessage(const char *function, const char *messageId, const char *message);
	
	static XrBool32 DebugMessageCallback(XrDebugUtilsMessageSeverityFlagsEXT messageSeverity,
		XrDebugUtilsMessageTypeFlagsEXT messageTypes,
		const XrDebugUtilsMessengerCallbackDataEXT *callbackData, void *userData);
};

#endif

