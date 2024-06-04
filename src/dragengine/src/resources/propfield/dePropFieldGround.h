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

#ifndef _DEPROPFIELDGROUND_H_
#define _DEPROPFIELDGROUND_H_

#include "../../dragengine_export.h"


class deComponent;
class deHeightTerrain;


/**
 * \brief Prop Field Ground.
 *
 * Defines the ground for instances of a prop field to be projected
 * upon. This list contains components as well as the
 * height terrain making up the ground. This list is used by the
 * physics module to project prop field instances onto the ground.
 * This list is designed to be used for a short time to align
 * instances added to a prop field and is afterwards released.
 * components are held by the prop field ground. Since
 * this object is designed to live only for a short time no
 * reference counting is used.
 */
class DE_DLL_EXPORT dePropFieldGround{
private:
	deHeightTerrain *pHeightTerrain;
	
	deComponent **pComponents;
	int pComponentCount;
	int pComponentSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new prop field ground. */
	dePropFieldGround();
	
	/** \brief Clean up prop field ground. */
	~dePropFieldGround();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Clears the ground of the height terrain and all terrains and components. */
	void Clear();
	
	/** \brief Height terrain or NULL if not set. */
	inline deHeightTerrain *GetHeightTerrain() const{ return pHeightTerrain; }
	
	/** \brief Set height terrain or NULL if not set. */
	void SetHeightTerrain( deHeightTerrain *heightTerrain );
	/*@}*/
	
	
	
	/** \name Components */
	/*@{*/
	/** \brief Number of components. */
	inline int GetComponentCount() const{ return pComponentCount; }
	
	/** \brief Component at the given index. */
	deComponent *GetComponentAt( int index ) const;
	
	/** \brief Index of the given component or -1 if not found. */
	int IndexOfComponent( deComponent *component ) const;
	
	/** \brief Given component exists. */
	bool HasComponent( deComponent *component ) const;
	
	/** \brief Adds a component. */
	void AddComponent( deComponent *component );
	
	/** \brief Removes a component. */
	void RemoveComponent( deComponent *component );
	
	/** \brief Removes all components. */
	void RemoveAllComponents();
	/*@}*/
};

#endif
