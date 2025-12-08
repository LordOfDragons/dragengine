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

#ifndef _IGDEGDCLIGHTLIST_H_
#define _IGDEGDCLIGHTLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeGDCLight;



/**
 * \brief Game Definition Class Light List.
 */
class DE_DLL_EXPORT igdeGDCLightList{
private:
	decObjectOrderedSet pLights;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	igdeGDCLightList();
	
	/** \brief Create copy of a game definition class light list. */
	igdeGDCLightList(const igdeGDCLightList &list);
	
	/** \brief Clean up list. */
	~igdeGDCLightList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of lights. */
	int GetCount() const;
	
	/** \brief Light by index. */
	igdeGDCLight *GetAt(int index) const;
	
	/** \brief Index of an light or -1 if not found. */
	int IndexOf(igdeGDCLight::Ref light) const;
	
	/** \brief Determines if an light exists. */
	bool Has(igdeGDCLight::Ref light) const;
	
	/** \brief Adds an light. */
	void Add(igdeGDCLight::Ref light);
	
	/** \brief Inserts a new light. */
	void InsertAt(igdeGDCLight::Ref light, int index);
	
	/** \brief Moves an light to a new position. */
	void MoveTo(igdeGDCLight::Ref light, int index);
	
	/** \brief Removes an light. */
	void Remove(igdeGDCLight::Ref light);
	
	/** \brief Removes all lights. */
	void RemoveAll();
	
	/** \brief Set this list to a deep copy of another list. */
	void SetToDeepCopyFrom(const igdeGDCLightList &list);
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set list from another list. */
	igdeGDCLightList &operator=(const igdeGDCLightList &list);
	/*@}*/
};

#endif
