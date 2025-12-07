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

#ifndef _DEBNCONFIGURATION_H_
#define _DEBNCONFIGURATION_H_

#include <dragengine/common/string/decString.h>


/**
 * Configuration.
 */
class debnConfiguration{
public:
	enum eLogLevels{
		ellError,
		ellWarning,
		ellInfo,
		ellDebug
	};
	
	
	
private:
	eLogLevels pLogLevel;
	
	float pConnectResendInterval;
	float pConnectTimeout;
	float pReliableResendInterval;
	float pReliableTimeout;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create configuration. */
	debnConfiguration();
	
	/** Clean up configuration. */
	~debnConfiguration();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Log level. */
	inline eLogLevels GetLogLevel() const{ return pLogLevel; }
	void SetLogLevel(eLogLevels level);
	
	/** Connect resent interval in seconds. */
	inline float GetConnectResendInterval() const{ return pConnectResendInterval; }
	void SetConnectResendInterval(float interval);
	
	/** Connect timeout in seconds. */
	inline float GetConnectTimeout() const{ return pConnectTimeout; }
	void SetConnectTimeout(float timeout);
	
	/** Reliable message resend interval in seconds. */
	inline float GetReliableResendInterval() const{ return pReliableResendInterval; }
	void SetReliableResendInterval(float interval);
	
	/** Reliable message timeout in seconds. */
	inline float GetReliableTimeout() const{ return pReliableTimeout; }
	void SetReliableTimeout(float timeout);
	/*@}*/
};

#endif
