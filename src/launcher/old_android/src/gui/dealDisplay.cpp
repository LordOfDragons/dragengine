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

#include "android_native_app_glue.h"

#include <stdio.h>

#include <android/window.h>

#include "dealDialog.h"
#include "dealDisplay.h"
#include "dealFont.h"
#include "dealShader.h"
#include "dealImage.h"
#include "widgets/dealGuiTheme.h"
#include "widgets/dealWidgetLayout.h"
#include "widgets/dealWidgetLayoutFlow.h"
#include "widgets/dealWidgetImage.h"
#include "widgets/dealWidgetLogFile.h"
#include "widgets/test/dealWTestLayout.h"
#include "widgets/test/dealWTestLayout2.h"
#include "../dealLauncher.h"
#include "../common/exceptions.h"
#include "../logger/deLogger.h"

#define LOGSOURCE "DEAndroidLauncher"



// Class dealDisplay
//////////////////////

// Constructors, destructors
//////////////////////////////

dealDisplay::dealDisplay(dealLauncher &launcher) :
pLauncher(launcher),

pDisplay(EGL_NO_DISPLAY),
pSurface(EGL_NO_SURFACE),
pContext(EGL_NO_CONTEXT),
pWidth(0),
pHeight(0),

pVBOShapes(0),

pDefaultGuiTheme(NULL),
pFontDefault(NULL),
pFontSizeDefault(0),
pShaderShape(NULL),
pShaderShapeTex(NULL),

pContent(NULL),
pDialog(NULL),
pCaptureWidget(NULL){
}

dealDisplay::~dealDisplay(){
	pCleanUp();
}



// Management
///////////////

bool dealDisplay::Ready() const{
	return pContext != EGL_NO_DISPLAY;
}



void dealDisplay::Init(){
	if(pSurface != EGL_NO_SURFACE){
		return;
	}
	
	// update window flags in case the engine ran in the mean time altering them.
	// first parameter adds flags while second one removes flags
	// 
	// AWINDOW_FLAG_ALLOW_LOCK_WHILE_SCREEN_ON
	//   window can be locked while app is in use
	// 
	// AWINDOW_FLAG_KEEP_SCREEN_ON 
	//   prevent from dimming and locking while app is in use
	// 
	// AWINDOW_FLAG_LAYOUT_IN_SCREEN
	//   window fills entire screen
	// 
	// AWINDOW_FLAG_FULLSCREEN
	//   hide status bars and alike
//	ANativeActivity_setWindowFlags( pLauncher.GetAndroidApp().activity,
//		0, AWINDOW_FLAG_KEEP_SCREEN_ON | AWINDOW_FLAG_FULLSCREEN);
	ANativeActivity_setWindowFlags(pLauncher.GetAndroidApp().activity,
		AWINDOW_FLAG_FULLSCREEN, AWINDOW_FLAG_KEEP_SCREEN_ON);
	
	// check if context bound data has to be loaded
	const bool loadDefaultResources = (pDisplay == EGL_NO_DISPLAY);
	
	// initialize display
	if(pDisplay == EGL_NO_DISPLAY){
		pLauncher.GetLogger().LogInfo(LOGSOURCE, "Init display");
		pDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
		if(pDisplay == EGL_NO_DISPLAY){
			DETHROW(deeInvalidParam);
		}
		
		if(!eglInitialize(pDisplay, 0, 0)){
			DETHROW(deeInvalidParam);
		}
		
		// choose configuration
		const EGLint attribs[] = {
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
			EGL_RED_SIZE, 8,
			EGL_BLUE_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_ALPHA_SIZE, 8,
			EGL_DEPTH_SIZE, 24,
			EGL_NONE
		};
		EGLint configCount;
		if(!eglChooseConfig(pDisplay, attribs, &pConfig, 1, &configCount)){
			DETHROW(deeInvalidParam);
		}
		if(configCount == 0){
			DETHROW(deeInvalidParam);
		}
		
		pLauncher.GetLogger().LogInfo(LOGSOURCE, "EGL Configuration:");
		GLint value;
		EGLB_CHECK(pLauncher, eglGetConfigAttrib(pDisplay, pConfig, EGL_RED_SIZE, &value));
		pLauncher.GetLogger().LogInfoFormat(LOGSOURCE, "- Red: %d", value);
		EGLB_CHECK(pLauncher, eglGetConfigAttrib(pDisplay, pConfig, EGL_GREEN_SIZE, &value));
		pLauncher.GetLogger().LogInfoFormat(LOGSOURCE, "- Green: %d", value);
		EGLB_CHECK(pLauncher, eglGetConfigAttrib(pDisplay, pConfig, EGL_BLUE_SIZE, &value));
		pLauncher.GetLogger().LogInfoFormat(LOGSOURCE, "- Blue: %d", value);
		EGLB_CHECK(pLauncher, eglGetConfigAttrib(pDisplay, pConfig, EGL_ALPHA_SIZE, &value));
		pLauncher.GetLogger().LogInfoFormat(LOGSOURCE, "- Alpha: %d", value);
		EGLB_CHECK(pLauncher, eglGetConfigAttrib(pDisplay, pConfig, EGL_DEPTH_SIZE, &value));
		pLauncher.GetLogger().LogInfoFormat(LOGSOURCE, "- Depth: %d", value);
		
		// set buffer geometry
		EGLint format;
		if(!eglGetConfigAttrib(pDisplay, pConfig, EGL_NATIVE_VISUAL_ID, &format)){
			DETHROW(deeInvalidParam);
		}
		if(ANativeWindow_setBuffersGeometry(pLauncher.GetAndroidApp().window, 0, 0, format)){
			DETHROW(deeInvalidParam);
		}
	}
	
	// create surface
	pSurface = eglCreateWindowSurface(pDisplay, pConfig, pLauncher.GetAndroidApp().window, NULL);
	if(pSurface == EGL_NO_SURFACE){
		DETHROW(deeInvalidParam);
	}
	
	// create context if not existing. it can be kept around while frozen
	if(pContext == EGL_NO_CONTEXT){
		const EGLint eglAttribList[] = {
			EGL_CONTEXT_CLIENT_VERSION, 2,
			EGL_NONE
		};
		pContext = eglCreateContext(pDisplay, pConfig, NULL, eglAttribList);
		if(pContext == EGL_NO_CONTEXT){
			DETHROW(deeInvalidParam);
		}
	}
	
	// make surface current. we have to make it current each render loop
	if(eglMakeCurrent(pDisplay, pSurface, pSurface, pContext) == EGL_FALSE){
		DETHROW(deeInvalidParam);
	}
	
	// query display parameters
	if(!eglQuerySurface(pDisplay, pSurface, EGL_WIDTH, &pWidth)){
		DETHROW(deeInvalidParam);
	}
	if(!eglQuerySurface(pDisplay, pSurface, EGL_HEIGHT, &pHeight)){
		DETHROW(deeInvalidParam);
	}
	pLauncher.GetLogger().LogInfoFormat(LOGSOURCE, "Display size %ix%i", pWidth, pHeight);
	
	// set some default opengl states
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	OGL_CHECK(pLauncher, glEnable(GL_CULL_FACE));
	OGL_CHECK(pLauncher, glShadeModel(GL_SMOOTH));
	OGL_CHECK(pLauncher, glDisable(GL_DEPTH_TEST));
	OGL_CHECK(pLauncher, glEnable(GL_SCISSOR_TEST));
	OGL_CHECK(pLauncher, glEnable(GL_BLEND));
	OGL_CHECK(pLauncher, glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	pLauncher.GetLogger().LogInfo(LOGSOURCE, "Display ready");
	
	// load default resources (for the time being)
	if(loadDefaultResources){
		pCreateShapesVBO();
		pLoadDefaultAssets();
		pCreateShapeShader();
		pCreateShapeTexShader();
	}
	
	// create content widget
	if(!pContent){
		pContent.TakeOver(new dealWidgetLayout(*this));
		pContent->SetSize(decPoint(pWidth, pHeight));
		pContent->SetBackgroundColor(decColor());
	}
}

void dealDisplay::Close(){
	if(pDisplay == EGL_NO_DISPLAY){
		return;
	}
	
	if(pSurface == EGL_NO_SURFACE){
		return;
	}
	
	pLauncher.GetLogger().LogInfo(LOGSOURCE, "Close window");
	EGLB_CHECK(pLauncher, eglMakeCurrent(pDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT));
	
	EGLB_CHECK(pLauncher, eglDestroySurface(pDisplay, pSurface));
	pSurface = EGL_NO_SURFACE;
}

void dealDisplay::ConfigChanged(){
}



void dealDisplay::SetDialog(dealDialog *dialog){
	if(pDialog == dialog){
		return;
	}
	
	SetCaptureWidget(NULL);
	
	if(pDialog){
		pDialog->OnDeactivate();
	}
	
	pDialog = dialog;
	
	if(dialog){
		dialog->AddReference();
		dialog->GetContent()->SetSize(decPoint(pWidth, pHeight));
		dialog->OnActivate();
	}
}

void dealDisplay::SetCaptureWidget(dealWidget *widget){
	if(widget == pCaptureWidget){
		return;
	}
	
	
	pCaptureWidget = widget;
}



void dealDisplay::Paint(){
	if(pDisplay == EGL_NO_DISPLAY || pSurface == EGL_NO_SURFACE){
		return;
	}
	
	// make surface current. we have to make it current each render loop
	EGLB_CHECK(pLauncher, eglMakeCurrent(pDisplay, pSurface, pSurface, pContext));
	
	// check if screen size is still the same. listening to config changed is not going to
	// work since android delays screen changes for an undefinite time.
	int width = 0;
	int height = 0;
	EGLB_CHECK(pLauncher, eglQuerySurface(pDisplay, pSurface, EGL_WIDTH, &width));
	EGLB_CHECK(pLauncher, eglQuerySurface(pDisplay, pSurface, EGL_HEIGHT, &height));
	
	if(width != pWidth || height != pHeight){
		pLauncher.GetLogger().LogInfoFormat(LOGSOURCE, "Screen size changed: %ix%i -> %ix%i",
			pWidth, pHeight, width, height);
		pWidth = width;
		pHeight = height;
		ScreenOrientationChanged();
	}
	
	// render
	OGL_CHECK(pLauncher, glViewport(0, 0, pWidth, pHeight));
	OGL_CHECK(pLauncher, glScissor(0, 0, pWidth, pHeight));
	
	OGL_CHECK(pLauncher, glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	OGL_CHECK(pLauncher, glClear(GL_COLOR_BUFFER_BIT));
	
	ActivateVBOShapes();
	
	dealWidget *widget = pContent;
	if(pDialog){
		widget = pDialog->GetContent();
	}
	
	if(widget){
		dealWidget::sRenderContext context;
		
		context.transform = decTexMatrix::CreateScale(
			2.0f / (float)(pWidth - 1), -2.0f / (float)(pHeight - 1))
				* decTexMatrix::CreateTranslation( -1.0f, 1.0f );
		context.color.Set(1.0f, 1.0f, 1.0f, 1.0f);
		context.screenPosition.Set(0, 0);
		context.clipBaseScreenY = (float)(pHeight - 1);
		context.viewFrom.Set(0, 0);
		context.viewTo.Set(pWidth - 1, pHeight - 1);
		
		widget->Render(context);
	}
	
	// swap buffers
	EGLB_CHECK(pLauncher, eglSwapBuffers(pDisplay, pSurface));
}

void dealDisplay::ActivateVBOShapes(){
	if(!pVBOShapes){
		DETHROW(deeInvalidParam);
	}
	
	OGL_CHECK(pLauncher, glEnableVertexAttribArray(0));
	OGL_CHECK(pLauncher, glBindBuffer(GL_ARRAY_BUFFER, pVBOShapes));
	OGL_CHECK(pLauncher, glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)0));
}

void dealDisplay::ScreenOrientationChanged(){
	// resize content if active
	if(pContent){
		pContent->SetSize(decPoint(pWidth, pHeight));
	}
	
	if(pDialog){
		pDialog->GetContent()->SetSize(decPoint(pWidth, pHeight));
	}
}



// Private functions
//////////////////////

void dealDisplay::pCleanUp(){
	Close();
	
	if(pDialog){
		pDialog->OnDeactivate();
	}
	
	
	
	if(pVBOShapes){
		glDeleteBuffers(1, &pVBOShapes);
	}
	
	if(pDisplay != EGL_NO_DISPLAY){
		EGLB_CHECK(pLauncher, eglMakeCurrent(pDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT));
		
		if(pContext != EGL_NO_CONTEXT){
			EGLB_CHECK(pLauncher, eglDestroyContext(pDisplay, pContext));
			pContext = EGL_NO_CONTEXT;
		}
		
		EGLB_CHECK(pLauncher, eglTerminate(pDisplay));
		pDisplay = EGL_NO_DISPLAY;
	}
}

void dealDisplay::pLoadDefaultAssets(){
	pFontDefault.TakeOver(new dealFont(*this, "fonts/nimbus_sans_30.xml"));
	//pFontDefault = new dealFont( *this, "fonts/nimbus_sans_30_bold.xml" );
	//pFontDefault = new dealFont( *this, "fonts/nimbus_sans_30_border.xml" );
	pFontSizeDefault = (int)((float)decMath::min(pWidth, pHeight) / 15.0f);
	
	pDefaultGuiTheme.TakeOver(new dealGuiTheme(*this));
}

void dealDisplay::pCreateShapesVBO(){
	if(pVBOShapes){
		return;
	}
	
	GLfloat vbodata[14];
	
	// the actual geometry points are obtain by calculating x'=x*scale+offset.
	
	// set up vbo data for a point shape
	vbodata[0] = 0.0f; // p1.x = x
	vbodata[1] = 0.0f; // p1.y = y
	
	// set up vbo data for a line shape
	vbodata[2] = 0.0f; // p1.x = x1
	vbodata[3] = 0.0f; // p1.y = y1
	
	vbodata[4] = 1.0f; // p2.x = x2
	vbodata[5] = 1.0f; // p2.y = y2
	
	// set up vbo data for a rectangular shape
	vbodata[6] = 0.0f; // p1.x = x1
	vbodata[7] = 0.0f; // p1.y = y1
	
	vbodata[8] = 0.0f; // p2.x = x1
	vbodata[9] = 1.0f; // p2.y = y2
	
	vbodata[10] = 1.0f; // p3.x = x2
	vbodata[11] = 1.0f; // p3.y = y2
	
	vbodata[12] = 1.0f; // p4.x = x2
	vbodata[13] = 0.0f; // p4.y = y1
	
	// create vbo
	OGL_CHECK(pLauncher, glGenBuffers(1, &pVBOShapes));
	if(!pVBOShapes){
		DETHROW(deeOutOfMemory);
	}
	OGL_CHECK(pLauncher, glBindBuffer(GL_ARRAY_BUFFER, pVBOShapes));
	OGL_CHECK(pLauncher, glBufferData(GL_ARRAY_BUFFER, sizeof(vbodata),
		(const GLvoid *)&vbodata, GL_STATIC_DRAW));
	OGL_CHECK(pLauncher, glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void dealDisplay::pCreateShapeShader(){
	pShaderShape.TakeOver(new dealShader(*this));
	pShaderShape->CompileVertexProgramAsset("shaders/shape/vertex.glsl");
	pShaderShape->CompileFragmentProgramAsset("shaders/shape/fragment.glsl");
	pShaderShape->BindAttributeLocation("inPosition", 0);
	pShaderShape->Link();
	pShaderShape->Activate();
	pShaderShape->ResolveParameter("pTransform"); // espsTransform
	pShaderShape->ResolveParameter("pTCTransform"); // espsTCTransform, unused in this shader
	pShaderShape->ResolveParameter("pColorTransform"); // espsColorTransform
	pShaderShape->ResolveParameter("pColorTransform2"); // espsColorTransform2
	pShaderShape->ResolveParameter("pGamma"); // espsGamma
	pShaderShape->ResolveParameter("pClipRect"); // espsClipRect
	pShaderShape->ResolveParameter("pTCClamp"); // espsTCClamp
}

void dealDisplay::pCreateShapeTexShader(){
	pShaderShapeTex.TakeOver(new dealShader(*this));
	pShaderShapeTex->CompileVertexProgramAsset("shaders/shapetex/vertex.glsl");
	pShaderShapeTex->CompileFragmentProgramAsset("shaders/shapetex/fragment.glsl");
	pShaderShapeTex->BindAttributeLocation("inPosition", 0);
	pShaderShapeTex->Link();
	pShaderShapeTex->Activate();
	pShaderShapeTex->BindTextureUnitLocation("texColor", 0);
	pShaderShapeTex->ResolveParameter("pTransform"); // espsTransform
	pShaderShapeTex->ResolveParameter("pTCTransform"); // espsTCTransform
	pShaderShapeTex->ResolveParameter("pColorTransform"); // espsColorTransform
	pShaderShapeTex->ResolveParameter("pColorTransform2"); // espsColorTransform2
	pShaderShapeTex->ResolveParameter("pGamma"); // espsGamma
	pShaderShapeTex->ResolveParameter("pClipRect"); // espsClipRect
	pShaderShapeTex->ResolveParameter("pTCClamp"); // espsTCClamp
}
