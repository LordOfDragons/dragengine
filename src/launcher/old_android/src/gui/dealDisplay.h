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

#ifndef _DEALGUI_H_
#define _DEALGUI_H_

#include "dealBasics.h"
#include "config/dealConfiguration.h"
#include "filesystem/deVirtualFileSystem.h"
#include "common/string/decString.h"

class dealLauncher;
class dealFont;
class dealWidget;
class dealWidgetLayout;
class dealShader;
class dealDialog;
class dealGuiTheme;


/**
 * \brief Android display.
 */
class dealDisplay{
public:
	/** \brief Shader parameters for shape shaders. */
	enum eShaderParametersShape{
		espsTransform,
		espsTCTransform,
		espsColorTransform,
		espsColorTransform2,
		espsGamma,
		espsClipRect,
		espsTCClamp
	};
	
private:
	dealLauncher &pLauncher;
	
	EGLDisplay pDisplay;
	EGLSurface pSurface;
	EGLContext pContext;
	EGLConfig pConfig;
	int pWidth;
	int pHeight;
	
	GLuint pVBOShapes;
	
	dealGuiTheme *pDefaultGuiTheme;
	dealFont *pFontDefault;
	int pFontSizeDefault;
	dealShader *pShaderShape;
	dealShader *pShaderShapeTex;
	
	dealWidgetLayout *pContent;
	dealDialog *pDialog;
	dealWidget *pCaptureWidget;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create display. */
	dealDisplay(dealLauncher &launcher);
	
	/** \brief Clean up display. */
	~dealDisplay();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Launcher. */
	inline dealLauncher &GetLauncher() const{return pLauncher;}
	
	/** \brief Screen width. */
	inline int GetWidth() const{return pWidth;}
	
	/** \brief Screen height. */
	inline int GetHeight() const{return pHeight;}
	
	/** \brief Display is ready. */
	bool Ready() const;
	
	
	
	/** \brief Initialize display. */
	void Init();
	
	/** \brief Close display. */
	void Close();
	
	/** \brief Application configuration changed like screen rotated. */
	void ConfigChanged();
	
	
	
	/** \brief Content widget. */
	inline dealWidgetLayout *GetContent() const{return pContent;}
	
	/** \brief Dialog or \em NULL if there are none. */
	inline dealDialog *GetDialog() const{return pDialog;}
	
	/** \brief Set dialog or \em NULL to show none. */
	void SetDialog(dealDialog *dialog);
	
	/** \brief Capture widget or \em NULL if not set. */
	inline dealWidget *GetCaptureWidget() const{return pCaptureWidget;}
	
	/** \brief Set capture widget or \em NULL if not set. */
	void SetCaptureWidget(dealWidget *widget);
	
	
	
	/** \brief Paint display if dirty. */
	void Paint();
	
	/** \brief Activate VBO shapes. */
	void ActivateVBOShapes();
	
	/** \brief Screen orientation changed. */
	void ScreenOrientationChanged();
	
	
	
	/** \brief Default gui theme. */
	inline dealGuiTheme *GetDefaultGuiTheme() const{return pDefaultGuiTheme;}
	
	/** \brief Default font. */
	inline dealFont *GetDefaultFont() const{return pFontDefault;}
	
	/** \brief Default font size. */
	inline int GetDefaultFontSize() const{return pFontSizeDefault;}
	
	/** \brief Shape shader. */
	inline dealShader *GetShapeShader() const{return pShaderShape;}
	
	/** \brief Textured shape shader. */
	inline dealShader *GetTexturedShapeShader() const{return pShaderShapeTex;}
	/*@}*/
	
private:
	void pCleanUp();
	void pLoadDefaultAssets();
	void pCreateShapesVBO();
	void pCreateShapeShader();
	void pCreateShapeTexShader();
};

#endif
 
