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

#ifndef _DEOGLPARTICLEEMITTERINSTANCELIST_H_
#define _DEOGLPARTICLEEMITTERINSTANCELIST_H_

class deoglRParticleEmitterInstance;



/**
 * Particle Emitter Instance List.
 * 
 * List of pointers to particle emitter instances. The ordering of the instances in the list is
 * not defined and can change when emitters are removed. This allows to remove elements with
 * constant time requirements.
 */
class deoglParticleEmitterInstanceList{
private:
	deoglRParticleEmitterInstance **pInstances;
	int pInstanceCount;
	int pInstanceSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create particle emitter instance list. */
	deoglParticleEmitterInstanceList();
	
	/** Clean up particle emitter instance list. */
	~deoglParticleEmitterInstanceList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Number of instances. */
	inline int GetCount() const{ return pInstanceCount; }
	
	/** Index of the instance or -1 if not in the list. */
	int IndexOf(deoglRParticleEmitterInstance *instance) const;
	
	/** Instance exists. */
	bool Has(deoglRParticleEmitterInstance *instance) const;
	
	/** Instance at the given index. */
	deoglRParticleEmitterInstance *GetAt(int index) const;
	
	/** Add instance. */
	void Add(deoglRParticleEmitterInstance *instance);
	
	/** Add instance if missing in the list. Returns true if added. */
	bool AddIfMissing(deoglRParticleEmitterInstance *instance);
	
	/** Remove instance. */
	void Remove(deoglRParticleEmitterInstance *instance);
	
	/** Remove instance if existing in the list. Returns true if removed. */
	bool RemoveIfExisting(deoglRParticleEmitterInstance *instance);
	
	/** Remove instance from the given index. */
	void RemoveFrom(int index);
	
	/** Remove all instances. */
	void RemoveAll();
	/*@}*/
	
private:
	void pAddInstance(deoglRParticleEmitterInstance *instance);
};

#endif
