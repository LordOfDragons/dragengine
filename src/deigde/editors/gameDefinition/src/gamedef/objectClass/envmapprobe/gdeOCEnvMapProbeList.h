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

#ifndef _GDEOCENVMAPPROBELIST_H_
#define _GDEOCENVMAPPROBELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeOCEnvMapProbe;



/**
 * \brief Object class probe list.
 */
class gdeOCEnvMapProbeList{
private:
	decObjectOrderedSet pProbes;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object class environment map probe list. */
	gdeOCEnvMapProbeList();
	
	/** \brief Create copy of object class environment map probe list. */
	gdeOCEnvMapProbeList(const gdeOCEnvMapProbeList &list);
	
	/** \brief Clean up object class probe list. */
	~gdeOCEnvMapProbeList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of object class environment map probes. */
	int GetCount() const;
	
	/** \brief Object class environment map probe at position. */
	gdeOCEnvMapProbe *GetAt(int index) const;
	
	/** \brief Index of named object class environment map probe or -1 if absent. */
	int IndexOf(gdeOCEnvMapProbe *probe) const;
	
	/** \brief Object class environment map is present. */
	bool Has(gdeOCEnvMapProbe *probe) const;
	
	/** \brief Add object class environment map probe. */
	void Add(gdeOCEnvMapProbe *probe);
	
	/** \brief Remove object class environment map probe. */
	void Remove(gdeOCEnvMapProbe *probe);
	
	/** \brief Remove all object class environment map probes. */
	void RemoveAll();
	
	/** \brief Set from another object class environment map probe list. */
	gdeOCEnvMapProbeList &operator=(const gdeOCEnvMapProbeList &list);
	/*@}*/
};

#endif
