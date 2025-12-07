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

#ifndef _IGDEDIALOGBROWSERPARTICLEEMITTER_H_
#define _IGDEDIALOGBROWSERPARTICLEEMITTER_H_

#include "igdeDialogBrowser.h"


class igdeGDParticleEmitter;


/**
 * \brief Dialog for user to browse particle emitter.
 */
class DE_DLL_EXPORT igdeDialogBrowserParticleEmitter : public igdeDialogBrowser{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeDialogBrowserParticleEmitter> Ref;
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	igdeDialogBrowserParticleEmitter( igdeEnvironment &environment, const char *title = "Select ParticleEmitter" );
	
protected:
	/** \brief Clean up selection dialog. */
	virtual ~igdeDialogBrowserParticleEmitter();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Selected particle emitter. */
	igdeGDParticleEmitter *GetSelectedParticleEmitter() const;
	
	/** \brief Set selected particle emitter. */
	void SetSelectedParticleEmitter( igdeGDParticleEmitter *gdParticleEmitter );
	
	
	
	/** \brief Convenience method to select particle emitter. */
	static bool SelectParticleEmitter( igdeWidget *owner, igdeGDParticleEmitter* &particleEmitter,
		const char *title = "Select ParticleEmitter" );
	
	/** \brief Convenience method to select particle emitter. */
	static bool SelectParticleEmitter( igdeWidget *owner, decString &particleEmitter,
		const char *title = "Select ParticleEmitter" );
	/*@}*/
	
	
	
protected:
	virtual igdeGDCategory *GetRootCategory() const;
	virtual void AddItemsToList( igdeGDAddToListVisitor &visitor );
	virtual void RebuildItemPreview( igdeGDPreviewManager &pvmgr, igdeGDPreviewListener *listener );
	virtual void GetSelectedItemInfo( decString &info );
};

#endif
