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

#ifndef _SEULAYERREMOVE_H_
#define _SEULAYERREMOVE_H_

#include <deigde/undo/igdeUndo.h>

class seLayer;
class seSky;


/**
 * \brief Undo action remove layer.
 */
class seULayerRemove : public igdeUndo{
private:
	seSky *pSky;
	seLayer *pLayer;
	int pIndex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo action. */
	seULayerRemove( seLayer *layer );
	
protected:
	/** \brief Clean up undo action. */
	virtual ~seULayerRemove();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo. */
	virtual void Undo();
	
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
};

#endif
