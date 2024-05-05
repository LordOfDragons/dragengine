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

#ifndef _DEOGLENVMAPSLOTMANAGER_H_
#define _DEOGLENVMAPSLOTMANAGER_H_

class deoglRenderThread;
class deoglArrayTexture;
class deoglEnvMapSlot;
class deoglEnvironmentMap;



/**
 * Environment Map Slot Manager.
 * Manages a list of environment map slots. Slots can be empty or referencing an environment map using
 * a last used counter. A fixed number of slots is managed with the possibility to enlarge if required.
 * New environment maps are assigned a slot if not in the list already and their last use is reset to 0.
 * After use the last used counter is increased of all slots. If all slots are full the empty slot not
 * used the since the most number of frames is assigned the environment map. If an environment map is
 * assigned to a slot the environment map texture is copied to the array texture of the matching layer.
 * As a result the array texture contains the active environment maps with the least amount of texture
 * copying required. In addition a list of indices to all the non-empty slots is stored easy to be used
 * for rendering later on.
 */
class deoglEnvMapSlotManager{
private:
	deoglRenderThread &pRenderThread;
	deoglArrayTexture *pArrayTexture;
	
	int pWidth;
	int pHeight;
	int pLayerCount;
	
	deoglEnvMapSlot *pSlots;
	int *pUsedSlots;
	int pUsedSlotsSize;
	int pUsedSlotsCount;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new environment map slot manager. */
	deoglEnvMapSlotManager( deoglRenderThread &renderThread );
	/** Cleans up the environment map slot manager. */
	~deoglEnvMapSlotManager();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	/** Retrieves the array texture. */
	inline deoglArrayTexture *GetArrayTexture() const{ return pArrayTexture; }
	
	/** Retrieves the width of the array texture. */
	inline int GetWidth() const{ return pWidth; }
	/** Retrieves the height of the array texture. */
	inline int GetHeight() const{ return pHeight; }
	/** Retrieves the number of layers in the array texture. This is also the number of available slots. */
	inline int GetLayerCount() const{ return pLayerCount; }
	
	/** Retrieves the slot at the given index. */
	deoglEnvMapSlot &GetSlotAt( int index ) const;
	/** Retrieves the index of the slot with the given environment map or -1 if not found. */
	int IndexOfSlotWith( deoglEnvironmentMap *envmap ) const;
	/** Retrieves the index of the oldest empty slot or -1 if all are used. */
	int IndexOfOldestUnusedSlot() const;
	
	/** Mark all slots as unused. This has to be called before any environment maps are added for a render run. */
	void MarkSlotsUnused();
	/**
	 * Adds an environment map. If no slot holds the environment map the oldest empty slot is assigned the
	 * environment map and the texture data copied to the appropriate layer in the array texture. In all
	 * cases the last used is reset to 0.
	 */
	void AddEnvironmentMap( deoglEnvironmentMap *envmap );
	/** Increase the last used counters of all environment slots by one. */
	void IncreaseSlotLastUsedCounters();
	
	/** Notification by an environment map that the texture changed and needs to be copied to the corrensponding array texture level. */
	void NotifyEnvMapChanged( int slotIndex );
	
	/** Retrieves the number of used slots. */
	inline int GetUsedSlotCount() const{ return pUsedSlotsCount; }
	/** Retrieves the index of the used slot by index. */
	int GetUsedSlotIndexAt( int index ) const;
	/** Update the used slots list from the current slot assignment. */
	void UpdateUsedSlots();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
