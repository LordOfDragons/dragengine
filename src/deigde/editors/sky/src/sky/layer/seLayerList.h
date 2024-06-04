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

#ifndef _SELAYERLIST_H_
#define _SELAYERLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class seLayer;



/**
 * \brief Sky layer list.
 */
class seLayerList{
private:
	decObjectOrderedSet pLayers;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create layer list. */
	seLayerList();
	
	/** \brief Clean up layer list. */
	~seLayerList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of layers. */
	int GetCount() const;
	
	/** \brief Layer at index. */
	seLayer *GetAt( int index ) const;
	
	/** \brief Index of layer or -1 if absent. */
	int IndexOf( seLayer *layer ) const;
	
	/** \brief Layer is present. */
	bool Has( seLayer *layer ) const;
	
	/** \brief Add layer. */
	void Add( seLayer *layer );
	
	/** \brief Insert layer. */
	void InsertAt( seLayer *layer, int index );
	
	/** \brief Move layer. */
	void MoveTo( seLayer *layer, int index );
	
	/** \brief Remove layer. */
	void Remove( seLayer *layer );
	
	/** \brief Remove all layers. */
	void RemoveAll();
	
	/** \brief Set list from another list. */
	seLayerList &operator=( const seLayerList &list );
	/*@}*/
};

#endif
