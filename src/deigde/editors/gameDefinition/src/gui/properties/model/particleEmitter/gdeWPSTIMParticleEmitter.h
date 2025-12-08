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

#ifndef _GDEWPSTIMPARTICLEEMITTER_H_
#define _GDEWPSTIMPARTICLEEMITTER_H_

#include "../gdeWPSTreeItemModel.h"

class gdeParticleEmitter;


/**
 * \brief Particle emitter.
 */
class gdeWPSTIMParticleEmitter : public gdeWPSTreeItemModel{
private:
	gdeParticleEmitter::Ref pParticleEmitter;
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	gdeWPSTIMParticleEmitter(gdeWPSTreeModel &tree, gdeParticleEmitter *particleEmitter);
	
protected:
	/** \brief Clean up tree item model. */
	~gdeWPSTIMParticleEmitter() override;
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Particle emitter. */
	inline gdeParticleEmitter *GetParticleEmitter() const{ return pParticleEmitter; }
	
	
	
	/** \brief Name changed. */
	void NameChanged();
	
	/**
	 * \brief Compare this item with another for sorting.
	 * 
	 * Default implementation returns 0 to keep same order.
	 */
	int Compare(const gdeWPSTreeItemModel &item) const override;
	
	/** \brief User selected item. */
	void OnSelected() override;
	
	/** \brief User requests context menu for selected item. */
	void OnContextMenu(igdeMenuCascade &contextMenu) override;
	
	/** \brief Select object mest matching name. */
	void SelectBestMatching(const char *string) override;
	/*@}*/
};

#endif
