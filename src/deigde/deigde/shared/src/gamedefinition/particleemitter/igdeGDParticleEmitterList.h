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

#ifndef _IGDEGDPARTICLEEMITTERLIST_H_
#define _IGDEGDPARTICLEEMITTERLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeGDParticleEmitter;



/**
 * \brief Game Definition Particle Emitter List.
 */
class DE_DLL_EXPORT igdeGDParticleEmitterList{
private:
	decObjectOrderedSet pEmitters;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	igdeGDParticleEmitterList();
	
	/** \brief Clean up list. */
	~igdeGDParticleEmitterList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of emitters. */
	int GetCount() const;
	
	/** \brief Emitter by index. */
	igdeGDParticleEmitter *GetAt(int index) const;
	
	/** \brief Emitter with the given path or nullptr if not found. */
	igdeGDParticleEmitter *GetWithPath(const char *path) const;
	
	/** \brief Emitter with the given name or nullptr if not found. */
	igdeGDParticleEmitter *GetWithName(const char *name) const;
	
	/** \brief Index of an emitter or -1 if not found. */
	int IndexOf(igdeGDParticleEmitter *emitter) const;
	
	/** \brief Index of the enitter with the given path or -1 if not found. */
	int IndexOfWithPath(const char *path) const;
	
	/** \brief Index of the emitter with the given name or -1 if not found. */
	int IndexOfWithName(const char *name) const;
	
	/** \brief Index of the emitter with the given name or -1 if not found. */
	int IndexOfWithPathOrName(const char *path, const char *name) const;
	
	/** \brief Determines if an emitter exists. */
	bool Has(igdeGDParticleEmitter *emitter) const;
	
	/** \brief Determines if an emitter with the given path exists. */
	bool HasWithPath(const char *path) const;
	
	/** \brief Determines if an emitter with the given name exists. */
	bool HasWithName(const char *name) const;
	
	/** \brief Determines if an emitter with the given name or path exists. */
	bool HasWithPathOrName(const char *path, const char *name) const;
	
	/** \brief Adds an emitter. */
	void Add(igdeGDParticleEmitter *emitter);
	
	/** \brief Inserts a new emitter. */
	void InsertAt(igdeGDParticleEmitter *emitter, int index);
	
	/** \brief Moves an emitter to a new position. */
	void MoveTo(igdeGDParticleEmitter *emitter, int index);
	
	/** \brief Removes an emitter. */
	void Remove(igdeGDParticleEmitter *emitter);
	
	/** \brief Removes all emitters. */
	void RemoveAll();
	
	/** \brief Set list from another list. */
	igdeGDParticleEmitterList &operator=(const igdeGDParticleEmitterList &list);
	/*@}*/
};

#endif
