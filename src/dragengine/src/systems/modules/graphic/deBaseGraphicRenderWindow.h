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

#ifndef _DEBASEGRAPHICRENDERWINDOW_H_
#define _DEBASEGRAPHICRENDERWINDOW_H_

#include "../../../dragengine_export.h"


/**
 * \brief Graphic module render window peer.
 */
class DE_DLL_EXPORT deBaseGraphicRenderWindow{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deBaseGraphicRenderWindow();
	
	/** \brief Clean up peer. */
	virtual ~deBaseGraphicRenderWindow();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Host window changed. */
	virtual void HostWindowChanged();
	
	/** \brief Size of render window changed. */
	virtual void SizeChanged();
	
	/** \brief Title of render window changed. */
	virtual void TitleChanged();
	
	/** \brief Full screen changed. */
	virtual void FullScreenChanged();
	
	/** \brief Notification of script on resize changed. */
	virtual void ScriptNotifyChanged();
	
	/** \brief Painting enabled changed. */
	virtual void PaintEnabledChanged();
	
	/** \brief Icon changed. */
	virtual void IconChanged();
	/*@}*/
};

#endif
