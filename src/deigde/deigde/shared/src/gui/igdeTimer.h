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

#ifndef _IGDETIMER_H_
#define _IGDETIMER_H_

#include <dragengine/deObject.h>

class igdeEnvironment;



/**
 * \brief IGDE UI Timer.
 */
class DE_DLL_EXPORT igdeTimer : public deObject{

public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeTimer> Ref;
	
	
private:
	igdeEnvironment &pEnvironment;
	void *pNativeTimer;
	int pTimeout;
	bool pRepeating;
	bool pRunning;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	igdeTimer(igdeEnvironment &environment);
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeTimer();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Environment. */
	inline igdeEnvironment &GetEnvironment() const{ return pEnvironment; }
	
	/** \brief Timeout in milliseconds. */
	inline int GetTimeout() const{ return pTimeout; }
	
	/** \brief Timeout in seconds. */
	inline double GetTimeoutSeconds() const{ return (double)pTimeout * 1000.0; }
	
	/** \brief Repeating. */
	inline bool GetRepeating() const{ return pRepeating; }
	
	/** \brief Timer is running. */
	inline bool GetRunning() const{ return pRunning; }
	
	
	
	/**
	 * \brief Start timer in milliseconds.
	 * 
	 * If timer is running it is restarted.
	 */
	void Start(int timeoutMS, bool repeating);
	
	/**
	 * \brief Start timer in seconds.
	 * 
	 * If timer is running it is restarted.
	 */
	void Start(double timeout, bool repeating);
	
	/** \brief Stop timer. */
	void Stop();
	
	
	
	/** \brief Timer elapsed. */
	virtual void OnTimeout();
	/*@}*/
	
	
	
	/**
	 * \name IGDE Internal Use Only
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	/*@{*/
	/**
	 * \brief Get native widget pointer.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	void *GetNativeTimer() const{ return pNativeTimer; }
	
protected:
	/**
	 * \brief Set native widget pointer.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	void SetNativeTimer(void *nativeTimer);
	/*@}*/
};

#endif
