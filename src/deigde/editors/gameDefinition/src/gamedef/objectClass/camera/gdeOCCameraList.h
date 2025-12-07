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

#ifndef _GDEOCCAMERALIST_H_
#define _GDEOCCAMERALIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeOCCamera;



/**
 * \brief Object class camera list.
 */
class gdeOCCameraList{
private:
	decObjectOrderedSet pCameras;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object class camera list. */
	gdeOCCameraList();
	
	/** \brief Create copy of object class camera list. */
	gdeOCCameraList(const gdeOCCameraList &list);
	
	/** \brief Clean up object class camera list. */
	~gdeOCCameraList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of object class cameras. */
	int GetCount() const;
	
	/** \brief Object class camera at position. */
	gdeOCCamera *GetAt(int index) const;
	
	/** \brief Index of named object class camera or -1 if absent. */
	int IndexOf(gdeOCCamera *camera) const;
	
	/** \brief Object class is present. */
	bool Has(gdeOCCamera *camera) const;
	
	/** \brief Add object class camera. */
	void Add(gdeOCCamera *camera);
	
	/** \brief Remove object class camera. */
	void Remove(gdeOCCamera *camera);
	
	/** \brief Remove all object class cameras. */
	void RemoveAll();
	
	/** \brief Set from another object class camera list. */
	gdeOCCameraList &operator=(const gdeOCCameraList &list);
	/*@}*/
};

#endif
