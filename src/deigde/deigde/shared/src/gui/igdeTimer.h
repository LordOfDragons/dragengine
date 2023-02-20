/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _IGDETIMER_H_
#define _IGDETIMER_H_

#include <dragengine/deObject.h>

class igdeEnvironment;



/**
 * \brief IGDE UI Timer.
 */
class DE_DLL_EXPORT igdeTimer : public deObject{
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
	igdeTimer( igdeEnvironment &environment );
	
	
	
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
	inline double GetTimeoutSeconds() const{ return ( double )pTimeout * 1000.0; }
	
	/** \brief Repeating. */
	inline bool GetRepeating() const{ return pRepeating; }
	
	/** \brief Timer is running. */
	inline bool GetRunning() const{ return pRunning; }
	
	
	
	/**
	 * \brief Start timer in milliseconds.
	 * 
	 * If timer is running it is restarted.
	 */
	void Start( int timeoutMS, bool repeating );
	
	/**
	 * \brief Start timer in seconds.
	 * 
	 * If timer is running it is restarted.
	 */
	void Start( double timeout, bool repeating );
	
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
	void SetNativeTimer( void *nativeTimer );
	/*@}*/
};

#endif
