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

#ifndef _IGDETRIGGERTARGET_H_
#define _IGDETRIGGERTARGET_H_

#include <dragengine/common/collection/decObjectSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/deObject.h>

class igdeTriggerListener;



/**
 * \brief Trigger Target.
 */
class DE_DLL_EXPORT igdeTriggerTarget : public deObject{
private:
	decString pName;
	bool pFired;
	bool pHasFired;
	decObjectSet pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create trigger target. */
	igdeTriggerTarget( const char *name );
	
	
	
protected:
	/** \brief Clean up trigger target. */
	virtual ~igdeTriggerTarget();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Determines if the target is currently fired or not. */
	inline bool GetFired() const{ return pFired; }
	
	/** \brief Sets if the target is currently fired or not. */
	void SetFired( bool fired );
	
	/** \brief Determines if the target has ever fired. */
	inline bool GetHasFired() const{ return pHasFired; }
	
	/** \brief Sets if the target has ever fired. */
	void SetHasFired( bool hasFired );
	
	/** \brief Fire the trigger action. */
	void Fire();
	
	/** \brief Reset the trigger. */
	void Reset();
	
	/** \brief Fully resets the trigger. */
	void FullReset();
	
	/** \brief Adds a listener. */
	void AddListener( igdeTriggerListener *listener );
	
	/** \brief Removes a listener if existing. */
	void RemoveListener( igdeTriggerListener *listener );
	
	/** \brief Notify all listeners. */
	void NotifyListeners();
	/*@}*/
};

#endif
