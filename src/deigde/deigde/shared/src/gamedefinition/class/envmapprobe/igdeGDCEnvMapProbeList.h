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

#ifndef _IGDEGDCENVMAPPROBELIST_H_
#define _IGDEGDCENVMAPPROBELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeGDCEnvMapProbe;



/**
 * \brief Game Definition Class Environment Map Probe List.
 */
class DE_DLL_EXPORT igdeGDCEnvMapProbeList{
private:
	decObjectOrderedSet pProbes;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	igdeGDCEnvMapProbeList();
	
	/** \brief Create copy of a game definition environment map probe list. */
	igdeGDCEnvMapProbeList(const igdeGDCEnvMapProbeList &list);
	
	/** \brief Clean up list. */
	~igdeGDCEnvMapProbeList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of environment map probes. */
	int GetCount() const;
	
	/** \brief Environment map probe by index. */
	igdeGDCEnvMapProbe *GetAt(int index) const;
	
	/** \brief Index of an environment map probe or -1 if not found. */
	int IndexOf(igdeGDCEnvMapProbe::Ref probe) const;
	
	/** \brief Determines if an environment map probe exists. */
	bool Has(igdeGDCEnvMapProbe::Ref probe) const;
	
	/** \brief Adds an environment map probe. */
	void Add(igdeGDCEnvMapProbe::Ref probe);
	
	/** \brief Inserts a new environment map probe. */
	void InsertAt(igdeGDCEnvMapProbe::Ref probe, int index);
	
	/** \brief Moves an environment map probe to a new position. */
	void MoveTo(igdeGDCEnvMapProbe::Ref probe, int index);
	
	/** \brief Removes an environment map probe. */
	void Remove(igdeGDCEnvMapProbe::Ref probe);
	
	/** \brief Removes all environment map probes. */
	void RemoveAll();
	
	/** \brief Set this list to a deep copy of another list. */
	void SetToDeepCopyFrom(const igdeGDCEnvMapProbeList &list);
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set list from another list. */
	igdeGDCEnvMapProbeList &operator=(const igdeGDCEnvMapProbeList &list);
	/*@}*/
};

#endif
