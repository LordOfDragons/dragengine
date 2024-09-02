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

#ifndef _MEVIEW3D_H_
#define _MEVIEW3D_H_

#include <deigde/gui/igdeViewRenderWindow.h>
#include <deigde/gui/event/igdeMouseKeyListenerReference.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/canvas/deCanvasTextReference.h>
#include <dragengine/resources/canvas/deCanvasViewReference.h>
#include <dragengine/resources/font/deFontReference.h>

class meView3DListener;
class meViewEditor;

class meWorld;
class meWindowMain;


/**
 * \brief 3D view of the world.
 */
class meView3D : public igdeViewRenderWindow{
private:
	meWindowMain &pWindowMain;
	meView3DListener *pListener;
	
	meWorld *pWorld;
	
	igdeMouseKeyListenerReference pListenerEditor;
	meViewEditor *pEditor;
	
	int pFPSHistory[ 30 ];
	float pFPSRedrawCanvasDelay;
	
	deFontReference pFontStats;
	deCanvasViewReference pCanvasFPS;
	deCanvasTextReference pCanvasFPSText;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create view. */
	meView3D( meWindowMain &windowMain );
	
protected:
	/** \brief Clean up view. */
	virtual ~meView3D();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Window. */
	inline meWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Reset view. */
	void ResetView();
	
	/** \brief World. */
	inline meWorld *GetWorld() const{ return pWorld; }
	
	/** \brief Set world render. */
	void SetWorld( meWorld *world );
	
	/** \brief Create canvas. */
	virtual void CreateCanvas();
	
	/** \brief Resize canvas to fit window size. */
	virtual void OnResize();
	
	/** \brief Game like frame update. */
	virtual void OnFrameUpdate( float elapsed );
	
	/** \brief Mode changed. */
	void ModeChanged();
	
	/** \brief Active camera changed. */
	void ActiveCameraChanged();
	
	/** \brief Editor or NULL. */
	inline meViewEditor *GetEditor() const{ return pEditor; }
	/*@}*/
};

#endif
