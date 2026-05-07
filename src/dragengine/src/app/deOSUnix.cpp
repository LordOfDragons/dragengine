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

#include "../dragengine_configuration.h"

#ifdef OS_UNIX_X11

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <locale.h>
#include <sys/time.h>
#include <dlfcn.h>
#include <X11/Xproto.h>
#include <X11/extensions/Xrandr.h>
#include <X11/Xresource.h>

#include "deOSUnix.h"
#include "../deEngine.h"
#include "../common/exceptions.h"
#include "../common/string/decString.h"
#include "../logger/deLogger.h"
#include "../systems/deInputSystem.h"
#include "../systems/deScriptingSystem.h"
#include "../systems/modules/input/deBaseInputModule.h"



// error handling function
static deOSUnix *tempGlobalOS = nullptr;

static int errorHandler(Display *dpy, XErrorEvent *e){
	// we need a way to get hold of a default logger. best way would be a global logger
	// as the logger class is reference counted. this does not guarantee that the log
	// is written to the correct engine object but this is anyways not possible since
	// x errors are asynchronous to begin with.
	
	char errorText[1024];
	XGetErrorText(dpy, e->error_code, errorText, sizeof(errorText));
	
	const char * requestCodeName = "??";
	const char * minorCodeName = "-";
	
	switch(e->request_code){
	case X_CreateWindow: requestCodeName = "CreateWindow"; break;              
	case X_ChangeWindowAttributes: requestCodeName = "ChangeWindowAttributes"; break;        
	case X_GetWindowAttributes: requestCodeName = "GetWindowAttributes"; break;     
	case X_DestroyWindow: requestCodeName = "DestroyWindow"; break;
	case X_DestroySubwindows: requestCodeName = "DestroySubwindows"; break;   
	case X_ChangeSaveSet: requestCodeName = "ChangeSaveSet"; break;
	case X_ReparentWindow: requestCodeName = "ReparentWindow"; break;
	case X_MapWindow: requestCodeName = "MapWindow"; break;
	case X_MapSubwindows: requestCodeName = "MapSubwindows"; break;
	case X_UnmapWindow: requestCodeName = "UnmapWindow"; break;
	case X_UnmapSubwindows: requestCodeName = "UnmapSubwindows"; break;  
	case X_ConfigureWindow: requestCodeName = "ConfigureWindow"; break;  
	case X_CirculateWindow: requestCodeName = "CirculateWindow"; break;  
	case X_GetGeometry: requestCodeName = "GetGeometry"; break;
	case X_QueryTree: requestCodeName = "QueryTree"; break;
	case X_InternAtom: requestCodeName = "InternAtom"; break;
	case X_GetAtomName: requestCodeName = "GetAtomName"; break;
	case X_ChangeProperty: requestCodeName = "ChangeProperty"; break; 
	case X_DeleteProperty: requestCodeName = "DeleteProperty"; break; 
	case X_GetProperty: requestCodeName = "GetProperty"; break;
	case X_ListProperties: requestCodeName = "ListProperties"; break; 
	case X_SetSelectionOwner: requestCodeName = "SetSelectionOwner"; break;    
	case X_GetSelectionOwner: requestCodeName = "GetSelectionOwner"; break;    
	case X_ConvertSelection: requestCodeName = "ConvertSelection"; break;   
	case X_SendEvent: requestCodeName = "SendEvent"; break;
	case X_GrabPointer: requestCodeName = "GrabPointer"; break;
	case X_UngrabPointer: requestCodeName = "UngrabPointer"; break;
	case X_GrabButton: requestCodeName = "GrabButton"; break;
	case X_UngrabButton: requestCodeName = "UngrabButton"; break;
	case X_ChangeActivePointerGrab: requestCodeName = "ChangeActivePointerGrab"; break;          
	case X_GrabKeyboard: requestCodeName = "GrabKeyboard"; break;
	case X_UngrabKeyboard: requestCodeName = "UngrabKeyboard"; break; 
	case X_GrabKey: requestCodeName = "GrabKey"; break;
	case X_UngrabKey: requestCodeName = "UngrabKey"; break;
	case X_AllowEvents: requestCodeName = "AllowEvents"; break;       
	case X_GrabServer: requestCodeName = "GrabServer"; break;      
	case X_UngrabServer: requestCodeName = "UngrabServer"; break;        
	case X_QueryPointer: requestCodeName = "QueryPointer"; break;        
	case X_GetMotionEvents: requestCodeName = "GetMotionEvents"; break;           
	case X_TranslateCoords: requestCodeName = "TranslateCoords"; break;                
	case X_WarpPointer: requestCodeName = "WarpPointer"; break;       
	case X_SetInputFocus: requestCodeName = "SetInputFocus"; break;         
	case X_GetInputFocus: requestCodeName = "GetInputFocus"; break;         
	case X_QueryKeymap: requestCodeName = "QueryKeymap"; break;       
	case X_OpenFont: requestCodeName = "OpenFont"; break;    
	case X_CloseFont: requestCodeName = "CloseFont"; break;     
	case X_QueryFont: requestCodeName = "QueryFont"; break;
	case X_QueryTextExtents: requestCodeName = "QueryTextExtents"; break;     
	case X_ListFonts: requestCodeName = "ListFonts"; break;  
	case X_ListFontsWithInfo: requestCodeName = "ListFontsWithInfo"; break; 
	case X_SetFontPath: requestCodeName = "SetFontPath"; break; 
	case X_GetFontPath: requestCodeName = "GetFontPath"; break; 
	case X_CreatePixmap: requestCodeName = "CreatePixmap"; break;        
	case X_FreePixmap: requestCodeName = "FreePixmap"; break;      
	case X_CreateGC: requestCodeName = "CreateGC"; break;    
	case X_ChangeGC: requestCodeName = "ChangeGC"; break;    
	case X_CopyGC: requestCodeName = "CopyGC"; break;  
	case X_SetDashes: requestCodeName = "SetDashes"; break;     
	case X_SetClipRectangles: requestCodeName = "SetClipRectangles"; break;             
	case X_FreeGC: requestCodeName = "FreeGC"; break;  
	case X_ClearArea: requestCodeName = "ClearArea"; break;             
	case X_CopyArea: requestCodeName = "CopyArea"; break;    
	case X_CopyPlane: requestCodeName = "CopyPlane"; break;     
	case X_PolyPoint: requestCodeName = "PolyPoint"; break;     
	case X_PolyLine: requestCodeName = "PolyLine"; break;    
	case X_PolySegment: requestCodeName = "PolySegment"; break;       
	case X_PolyRectangle: requestCodeName = "PolyRectangle"; break;         
	case X_PolyArc: requestCodeName = "PolyArc"; break;   
	case X_FillPoly: requestCodeName = "FillPoly"; break;    
	case X_PolyFillRectangle: requestCodeName = "PolyFillRectangle"; break;             
	case X_PolyFillArc: requestCodeName = "PolyFillArc"; break;       
	case X_PutImage: requestCodeName = "PutImage"; break;    
	case X_GetImage: requestCodeName = "GetImage"; break; 
	case X_PolyText8: requestCodeName = "PolyText8"; break;     
	case X_PolyText16: requestCodeName = "PolyText16"; break;      
	case X_ImageText8: requestCodeName = "ImageText8"; break;      
	case X_ImageText16: requestCodeName = "ImageText16"; break;       
	case X_CreateColormap: requestCodeName = "CreateColormap"; break;          
	case X_FreeColormap: requestCodeName = "FreeColormap"; break;        
	case X_CopyColormapAndFree: requestCodeName = "CopyColormapAndFree"; break;               
	case X_InstallColormap: requestCodeName = "InstallColormap"; break;           
	case X_UninstallColormap: requestCodeName = "UninstallColormap"; break;             
	case X_ListInstalledColormaps: requestCodeName = "ListInstalledColormaps"; break;                  
	case X_AllocColor: requestCodeName = "AllocColor"; break;      
	case X_AllocNamedColor: requestCodeName = "AllocNamedColor"; break;           
	case X_AllocColorCells: requestCodeName = "AllocColorCells"; break;           
	case X_AllocColorPlanes: requestCodeName = "AllocColorPlanes"; break;            
	case X_FreeColors: requestCodeName = "FreeColors"; break;      
	case X_StoreColors: requestCodeName = "StoreColors"; break;       
	case X_StoreNamedColor: requestCodeName = "StoreNamedColor"; break;           
	case X_QueryColors: requestCodeName = "QueryColors"; break;       
	case X_LookupColor: requestCodeName = "LookupColor"; break;       
	case X_CreateCursor: requestCodeName = "CreateCursor"; break;        
	case X_CreateGlyphCursor: requestCodeName = "CreateGlyphCursor"; break;             
	case X_FreeCursor: requestCodeName = "FreeCursor"; break;      
	case X_RecolorCursor: requestCodeName = "RecolorCursor"; break;         
	case X_QueryBestSize: requestCodeName = "QueryBestSize"; break;         
	case X_QueryExtension: requestCodeName = "QueryExtension"; break;          
	case X_ListExtensions: requestCodeName = "ListExtensions"; break;          
	case X_ChangeKeyboardMapping: requestCodeName = "ChangeKeyboardMapping"; break;
	case X_GetKeyboardMapping: requestCodeName = "GetKeyboardMapping"; break;
	case X_ChangeKeyboardControl: requestCodeName = "ChangeKeyboardControl"; break;                
	case X_GetKeyboardControl: requestCodeName = "GetKeyboardControl"; break;             
	case X_Bell: requestCodeName = "Bell"; break;
	case X_ChangePointerControl: requestCodeName = "ChangePointerControl"; break;
	case X_GetPointerControl: requestCodeName = "GetPointerControl"; break;
	case X_SetScreenSaver: requestCodeName = "SetScreenSaver"; break;          
	case X_GetScreenSaver: requestCodeName = "GetScreenSaver"; break;          
	case X_ChangeHosts: requestCodeName = "ChangeHosts"; break;       
	case X_ListHosts: requestCodeName = "ListHosts"; break;     
	case X_SetAccessControl: requestCodeName = "SetAccessControl"; break;               
	case X_SetCloseDownMode: requestCodeName = "SetCloseDownMode"; break;
	case X_KillClient: requestCodeName = "KillClient"; break; 
	case X_RotateProperties: requestCodeName = "RotateProperties"; break;
	case X_ForceScreenSaver: requestCodeName = "ForceScreenSaver"; break;
	case X_SetPointerMapping: requestCodeName = "SetPointerMapping"; break;
	case X_GetPointerMapping: requestCodeName = "GetPointerMapping"; break;
	case X_SetModifierMapping: requestCodeName = "SetModifierMapping"; break;
	case X_GetModifierMapping: requestCodeName = "GetModifierMapping"; break;
	case X_NoOperation: requestCodeName = "NoOperation"; break;
	case 151: requestCodeName = "XInputExtension"; break;
	case 153: requestCodeName = "GLX"; break;
	default: break;
	}
	
	if(e->request_code == 153){
		switch(e->minor_code){
		case 1: minorCodeName = "GLXRender"; break;
		case 2: minorCodeName = "GLXRenderLarge"; break;
		case 3: minorCodeName = "GLXCreateContext"; break;
		case 4: minorCodeName = "GLXDestroyContext"; break;
		case 5: minorCodeName = "GLXMakeCurrent"; break;
		case 6: minorCodeName = "GLXIsDirect"; break;
		case 7: minorCodeName = "GLXQueryVersion"; break;
		case 8: minorCodeName = "GLXWaitGL"; break;
		case 9: minorCodeName = "GLXWaitX"; break;
		case 10: minorCodeName = "GLXCopyContext"; break;
		case 11: minorCodeName = "GLXSwapBuffers"; break;
		case 12: minorCodeName = "GLXUseXFont"; break;
		case 13: minorCodeName = "GLXCreateGLXPixmap"; break;
		case 14: minorCodeName = "GLXGetVisualConfigs"; break;
		case 15: minorCodeName = "GLXDestroyGLXPixmap"; break;
		case 16: minorCodeName = "GLXVendorPrivate"; break;
		case 17: minorCodeName = "GLXVendorPrivateWithReply"; break;
		case 18: minorCodeName = "GLXQueryExtensionsString"; break;
		case 19: minorCodeName = "GLXQueryServerString"; break;
		case 20: minorCodeName = "GLXClientInfo"; break;
		case 21: minorCodeName = "GLXGetFBConfigs"; break;
		case 22: minorCodeName = "GLXCreatePixmap"; break;
		case 23: minorCodeName = "GLXDestroyPixmap"; break;
		case 24: minorCodeName = "GLXCreateNewContext"; break;
		case 25: minorCodeName = "GLXQueryContext"; break;
		case 26: minorCodeName = "GLXMakeContextCurrent"; break;
		case 27: minorCodeName = "GLXCreatePbuffer"; break;
		case 28: minorCodeName = "GLXDestroyPbuffer"; break;
		case 29: minorCodeName = "GLXGetDrawableAttributes"; break;
		case 30: minorCodeName = "GLXChangeDrawableAttributes"; break;
		case 31: minorCodeName = "GLXCreateWindow"; break;
		case 32: minorCodeName = "GLXDestroyWindow"; break;
		case 33: minorCodeName = "GLXSetClientInfoARB"; break;
		case 34: minorCodeName = "GLXCreateContextAttribsARB"; break;
		case 35: minorCodeName = "GLXSetClientInfo2ARB"; break;
		default: break;
		}
	}
	
	/*
	info:
	XInputExtension: opcodes, see https://www.x.org/releases/X11R7.7/doc/libXi/inputlib.html
	*/

	decString errorMessage;
	errorMessage.Format("XError %s: request_code=%s(%d) minor_code=%s(%d)",
		errorText, requestCodeName, e->request_code, minorCodeName, e->minor_code);
	
	if(tempGlobalOS){
		tempGlobalOS->GetEngine()->GetLogger()->LogError("Engine-OSUnix", errorMessage);
		
	}else{
		printf("[XERR] %s\n", errorMessage.GetString());
	}
	
	// request_code = 155 => GLX (see /usr/include/GL/glxpoto.h)
	// otherwise see /usr/include/X11/Xproto.h
	return 0; // let me live... unless you segfault ^_^ 
}


#ifdef OS_UNIX_WAYLAND

static deWlDynLib v_WaylandLib("OSUNIX", "wayland-client", 0);

#define DE_DECL_WLIFACE(name) \
	static const deWlDynLibData<wl_interface> v_wl_##name##_interface(v_WaylandLib, "wl_" #name "_interface"); \
	extern "C" const wl_interface wl_##name##_interface = *v_wl_##name##_interface;

DE_DECL_WLIFACE(registry)
DE_DECL_WLIFACE(display)
DE_DECL_WLIFACE(callback)
DE_DECL_WLIFACE(compositor)
DE_DECL_WLIFACE(shm_pool)
DE_DECL_WLIFACE(shm)
DE_DECL_WLIFACE(buffer)
DE_DECL_WLIFACE(data_offer)
DE_DECL_WLIFACE(data_source)
DE_DECL_WLIFACE(data_device)
DE_DECL_WLIFACE(data_device_manager)
DE_DECL_WLIFACE(shell)
DE_DECL_WLIFACE(shell_surface)
DE_DECL_WLIFACE(surface)
DE_DECL_WLIFACE(seat)
DE_DECL_WLIFACE(pointer)
DE_DECL_WLIFACE(keyboard)
DE_DECL_WLIFACE(touch)
DE_DECL_WLIFACE(output)
DE_DECL_WLIFACE(region)
DE_DECL_WLIFACE(subcompositor)
DE_DECL_WLIFACE(subsurface)
DE_DECL_WLIFACE(fixes)

#undef DE_DECL_WLIFACE


static deWlDynLibFunc<wl_display*(const char*)> v_wl_display_connect(v_WaylandLib, "wl_display_connect");
extern "C" wl_display *wl_display_connect(const char *name){
	return v_wl_display_connect(name);
}

static deWlDynLibFunc<void(wl_display*)> v_wl_display_disconnect(v_WaylandLib, "wl_display_disconnect");
extern "C" void wl_display_disconnect(wl_display *display){
	v_wl_display_disconnect(display);
}

static deWlDynLibFunc<int(wl_display*)> v_wl_display_dispatch(v_WaylandLib, "wl_display_dispatch");
extern "C" int wl_display_dispatch(wl_display *display){
	return v_wl_display_dispatch(display);
}

static deWlDynLibFunc<int(wl_display*)> v_wl_display_dispatch_pending(v_WaylandLib, "wl_display_dispatch_pending");
extern "C" int wl_display_dispatch_pending(wl_display *display){
	return v_wl_display_dispatch_pending(display);
}

static deWlDynLibFunc<int(wl_display*)> v_wl_display_flush(v_WaylandLib, "wl_display_flush");
extern "C" int wl_display_flush(wl_display *display){
	return v_wl_display_flush(display);
}

static deWlDynLibFunc<int(wl_display*)> v_wl_display_roundtrip(v_WaylandLib, "wl_display_roundtrip");
extern "C" int wl_display_roundtrip(wl_display *display){
	return v_wl_display_roundtrip(display);
}

static deWlDynLibFunc<const wl_interface*(wl_proxy*)> v_wl_proxy_get_interface(v_WaylandLib, "wl_proxy_get_interface");
extern "C" const wl_interface *wl_proxy_get_interface(wl_proxy *proxy){
	return v_wl_proxy_get_interface(proxy);
}

static deWlDynLibFunc<wl_proxy*(wl_proxy*, uint32_t, const wl_interface*, uint32_t, uint32_t, union wl_argument*)>
	v_wl_proxy_marshal_array_flags(v_WaylandLib, "wl_proxy_marshal_array_flags");

extern "C" wl_proxy *wl_proxy_marshal_array_flags(wl_proxy *proxy, uint32_t opcode,
const wl_interface *interface, uint32_t version, uint32_t flags, union wl_argument *args){
	return v_wl_proxy_marshal_array_flags(proxy, opcode, interface, version, flags, args);
}


static deWlDynLibFunc<wl_proxy*(wl_proxy*, uint32_t, const wl_interface*, uint32_t, uint32_t, ...)>
	v_wl_proxy_marshal_flags(v_WaylandLib, "wl_proxy_marshal_flags");

extern "C" wl_proxy *wl_proxy_marshal_flags(wl_proxy *proxy, uint32_t opcode, const wl_interface *interface,
uint32_t version, uint32_t flags, ...){
	const struct wl_interface *proxyIface = wl_proxy_get_interface(proxy);
	if(!proxyIface || !proxyIface->methods){
		DETHROW_INFO(deeInvalidAction, decString::Formatted(
			"Cannot marshal Wayland message: no method information available for opcode {0}", opcode));
	}
	
	if(opcode >= (uint32_t)proxyIface->method_count) {
		DETHROW_INFO(deeInvalidAction, decString::Formatted(
			"Cannot marshal Wayland message: Opcode {0} out of bounds for interface {1} (count {2})",
			opcode, proxyIface->name, proxyIface->method_count));
	}
	
	const char *sig = proxyIface->methods[opcode].signature;
	if(!sig){
		DETHROW_INFO(deeInvalidAction, decString::Formatted(
			"Cannot marshal Wayland message: no signature information available for opcode {0} of interface {1}",
			opcode, proxyIface->name));
	}
	
	const int maxArgs = 20;
	union wl_argument args[maxArgs];
	
	va_list ap;
	va_start(ap, flags);
	
	int i = 0;
	while(*sig && i < maxArgs){
		if((*sig >= '0' && *sig <= '9') || *sig == '?'){
			sig++;
			continue;
		}
		
		switch(*sig){
		case 'i':
			args[i++].i = va_arg(ap, int32_t);
			break;
			
		case 'u':
			args[i++].u = va_arg(ap, uint32_t);
			break;
			
		case 'f':
			args[i++].f = va_arg(ap, wl_fixed_t);
			break;
			
		case 's':
			args[i++].s = va_arg(ap, const char*);
			break;
			
		case 'o':
			args[i++].o = va_arg(ap, wl_object*);
			break;
			
		case 'n':
			args[i++].n = va_arg(ap, uint32_t);
			break;
			
		case 'a':
			args[i++].a = va_arg(ap, wl_array*);
			break;
			
		case 'h':
			args[i++].h = va_arg(ap, int32_t);
			break;
		}
		sig++;
	}
	va_end(ap);
	
	return v_wl_proxy_marshal_array_flags(proxy, opcode, interface, version, flags, args);
}

static deWlDynLibFunc<uint32_t(wl_proxy*)> v_wl_proxy_get_version(v_WaylandLib, "wl_proxy_get_version");
extern "C" uint32_t wl_proxy_get_version(wl_proxy *proxy){
	return v_wl_proxy_get_version(proxy);
}

static deWlDynLibFunc<int(wl_proxy*, void (**)(void), void*)> v_wl_proxy_add_listener(v_WaylandLib, "wl_proxy_add_listener");
extern "C" int wl_proxy_add_listener(wl_proxy *proxy, void (**implementation)(void), void *data){
	return v_wl_proxy_add_listener(proxy, implementation, data);
}

static deWlDynLibFunc<void(wl_proxy*)> v_wl_proxy_destroy(v_WaylandLib, "wl_proxy_destroy");
extern "C" void wl_proxy_destroy(wl_proxy *proxy){
	v_wl_proxy_destroy(proxy);
}

static deWlDynLibFunc<void(wl_proxy*, wl_event_queue*)> v_wl_proxy_set_queue(v_WaylandLib, "wl_proxy_set_queue");
extern "C" void wl_proxy_set_queue(wl_proxy *proxy, wl_event_queue *queue){
	v_wl_proxy_set_queue(proxy, queue);
}

static deWlDynLibFunc<wl_event_queue*(wl_display*)> v_wl_display_create_queue(v_WaylandLib, "wl_display_create_queue");
extern "C" wl_event_queue *wl_display_create_queue(wl_display *display){
	return v_wl_display_create_queue(display);
}

static deWlDynLibFunc<void(wl_event_queue*)> v_wl_event_queue_destroy(v_WaylandLib, "wl_event_queue_destroy");
extern "C" void wl_event_queue_destroy(wl_event_queue *queue){
	v_wl_event_queue_destroy(queue);
}

static deWlDynLibFunc<int(wl_display*)> v_wl_display_get_fd(v_WaylandLib, "wl_display_get_fd");
extern "C" int wl_display_get_fd(wl_display *display){
	return v_wl_display_get_fd(display);
}

static deWlDynLibFunc<int(wl_display*, wl_event_queue*)> v_wl_display_prepare_read_queue(v_WaylandLib, "wl_display_prepare_read_queue");
extern "C" int wl_display_prepare_read_queue(wl_display *display, wl_event_queue *queue){
	return v_wl_display_prepare_read_queue(display, queue);
}

static deWlDynLibFunc<int(wl_display*)> v_wl_display_read_events(v_WaylandLib, "wl_display_read_events");
extern "C" int wl_display_read_events(wl_display *display){
	return v_wl_display_read_events(display);
}

static deWlDynLibFunc<void(wl_display*)> v_wl_display_cancel_read(v_WaylandLib, "wl_display_cancel_read");
extern "C" void wl_display_cancel_read(wl_display *display){
	v_wl_display_cancel_read(display);
}

static deWlDynLibFunc<int(wl_display*, wl_event_queue*)> v_wl_display_dispatch_queue_pending(v_WaylandLib, "wl_display_dispatch_queue_pending");
extern "C" int wl_display_dispatch_queue_pending(wl_display *display, wl_event_queue *queue){
	return v_wl_display_dispatch_queue_pending(display, queue);
}

static deWlDynLibFunc<int(wl_display*, wl_event_queue*)> v_wl_display_roundtrip_queue(v_WaylandLib, "wl_display_roundtrip_queue");
extern "C" int wl_display_roundtrip_queue(wl_display *display, wl_event_queue *queue){
	return v_wl_display_roundtrip_queue(display, queue);
}

#endif


// Class deOSUnix
///////////////////

// Constructor, Destructor
////////////////////////////

deOSUnix::deOSUnix() :
pDisplay(nullptr),
pScreen(0),
pCurWindow(0),
pEventMask(0),
pHostingMainWindow(0),
pHostingRenderWindow(0),
pScaleFactor(100)

#ifdef OS_UNIX_WAYLAND
,pWaylandRegistry(nullptr),
pWaylandXdgOutputManager(zxdg_output_manager_v1_interface, 1, zxdg_output_manager_v1_destroy),
pWaylandDisplay(nullptr),
pEnableWayland(true)
#endif
{
	// init locale
	setlocale(LC_ALL, "");
	
	try{
#ifdef OS_UNIX_WAYLAND
		// try to connect to Wayland display
		if(pTryConnectWayland()){
			pGetWaylandDisplayInformation();
			return;
		}
#endif
		
		// set error handler
		tempGlobalOS = this;
		XSetErrorHandler(&errorHandler);
		
		// open display
		const char *dispName = getenv("DISPLAY");
		if(!dispName){
			dispName = "";
		}
		pDisplay = XOpenDisplay(dispName);
		if(!pDisplay){
			printf("[OSUNIX] Cannot not open display %s\n", XDisplayName(dispName));
			DETHROW(deeInvalidAction);
		}
		
		pScreen = XDefaultScreen(pDisplay);
		pGetDisplayInformation();
		pScaleFactor = pGetGlobalScaling();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deOSUnix::~deOSUnix(){
	pCleanUp();
}



// Management
///////////////

decString deOSUnix::GetPathEngine(){
	const char * const envPath = getenv("DE_ENGINE_PATH");
	if(envPath){
		return decString(envPath);
	}
	
	return decString(DE_ENGINE_PATH);
}

decString deOSUnix::GetPathShare(){
	const char * const envPath = getenv("DE_SHARE_PATH");
	if(envPath){
		return decString(envPath);
	}
	
	return decString(DE_SHARE_PATH);
}

decString deOSUnix::GetPathSystemConfig(){
	const char * const envPath = getenv("DE_CONFIG_PATH");
	if(envPath){
		return decString(envPath);
	}
	
	return decString(DE_CONFIG_PATH);
}

decString deOSUnix::GetPathUserConfig(){
	const char *envPath = getenv("DE_CONFIG_PATH");
	if(envPath){
		return decString(envPath);
	}
	
	envPath = getenv("XDG_CONFIG_HOME");
	if(envPath){
		return decString(envPath) + "/dragengine";
	}
	
	return decString(pGetHomeDirectory() + "/.config/dragengine");
}

decString deOSUnix::GetPathUserCache(){
	const char *envPath = getenv("DE_CACHE_PATH");
	if(envPath){
		return decString(envPath);
	}
	
	envPath = getenv("XDG_CACHE_HOME");
	if(envPath){
		return decString(envPath) + "/dragengine";
	}
	
	return decString(pGetHomeDirectory() + "/.cache/dragengine");
}

decString deOSUnix::GetPathUserCapture(){
	const char * const envPath = getenv("DE_CAPTURE_PATH");
	if(envPath){
		return decString(envPath);
	}
	
	return decString(GetPathUserCache() + "/capture");
}



// Display information
////////////////////////

int deOSUnix::GetDisplayCount(){
	return pDisplayInformation.GetCount();
}

decPoint deOSUnix::GetDisplayCurrentResolution(int display){
	return pDisplayInformation.GetAt(display).currentResolution;
}

int deOSUnix::GetDisplayCurrentRefreshRate(int display){
	return pDisplayInformation.GetAt(display).currentRefreshRate;
}

int deOSUnix::GetDisplayResolutionCount(int display){
	return pDisplayInformation.GetAt(display).resolutionCount;
}

decPoint deOSUnix::GetDisplayResolution(int display, int resolution){
	DEASSERT_TRUE(resolution >= 0)
	DEASSERT_TRUE(resolution < pDisplayInformation.GetAt(display).resolutionCount)
	
	return pDisplayInformation.GetAt(display).resolutions[resolution];
}

int deOSUnix::GetDisplayCurrentScaleFactor(int display){
	DEASSERT_TRUE(display >= 0)
	DEASSERT_TRUE(display < pDisplayInformation.GetCount())
	
	return pScaleFactor;
}



// Casting
////////////

deOSUnix *deOSUnix::CastToOSUnix(){
	return this;
}



// Unix related
/////////////////

void deOSUnix::SetEventMask(long mask){
	pEventMask = mask;
	pSetWindowEventMask();
}

void deOSUnix::SetWindow(Window wnd){
	if(pCurWindow == wnd){
		return;
	}
	
	//XWindowAttributes xwa;
	
	// check if the is a window at the moment
	if(pCurWindow > 255 && pDisplay){
		// ungrab mouse pointer
		XUngrabPointer(pDisplay, CurrentTime);
		XUngrabKeyboard(pDisplay, CurrentTime);
	}
	
	// set new window
	pCurWindow = wnd;
//	pCurWindowWidth = 0;
//	pCurWindowHeight = 0;
	
	// set event mask
	pSetWindowEventMask();
	
	// show new window if not null
	if(pCurWindow > 255 && pDisplay){
		XMapWindow(pDisplay, pCurWindow);
		XFlush(pDisplay);
		// fetch geometry information
//		XGetWindowAttributes( pDisplay, pCurWindow, &xwa );
//		pCurWindowWidth = xwa.width;
//		pCurWindowHeight = xwa.height;
		// grab the mouse pointer and keyboard
		/* === We leave this out for the moment as the window manager looses
		   === control of the keyboard which is not desirable. a better solution
		   === has to be found here.
		int result = XGrabPointer(p_Display, p_CurWindow, True, 0, GrabModeAsync,
			GrabModeAsync, p_CurWindow, None, CurrentTime);
		if(result != GrabSuccess){
			printf("[AppDisplay] Cannot Grab the MousePointer! (%i)\n", result);
		}
		result = XGrabKeyboard(p_Display, p_CurWindow, False, GrabModeAsync, GrabModeAsync,
			CurrentTime);
		if(result != GrabSuccess){
			printf("[AppDisplay] Cannot Grab the Keyboard! (%i)\n", result);
		}
		*/
	}
}

void deOSUnix::SetHostingMainWindow(Window window){
	pHostingMainWindow = window;
}

void deOSUnix::SetHostingRenderWindow(Window window){
	pHostingRenderWindow = window;
}

bool deOSUnix::HasHostingMainWindow() const{
	return pHostingMainWindow > 255;
}

bool deOSUnix::HasHostingRenderWindow() const{
	return pHostingRenderWindow > 255;
}



void deOSUnix::ProcessEventLoop(bool sendToInputModule){
	if(!pDisplay){
		return;
	}
	
	deInputSystem &inpSys = *GetEngine()->GetInputSystem();
	deBaseInputModule &inputModule = *inpSys.GetActiveModule();
	XEvent event;
	
	while(XPending(pDisplay)){
		XNextEvent(pDisplay, &event);
		
// 		GetEngine()->GetLogger()->LogInfoFormat( "Dragengine", "event type %i", event.type );
		switch(event.type){
		//case Expose:
		//	break;
			
		case ConfigureNotify:
			//const int windowWidth = event.xconfigure.width;
			//const int windowHeight = event.xconfigure.height;
			
			// notify graphic module about the change
			// notify the script module about the change
			
			break;
			
		//case DestroyNotify:
		//	break;
			
		//case EnterNotify:
		//	break;
		
		//case LeaveNotify:
		//	break;
		}
		
		if(sendToInputModule){
			inputModule.EventLoop(event);
		}
	}
}

decString deOSUnix::GetUserLocaleLanguage(){
	const char * const l = setlocale(LC_ALL, nullptr);
	if(l){
		const decString ls(l);
		const int deli = ls.Find('_');
		if(deli != -1){
			return ls.GetLeft(deli).GetLower();
			
		}else{
			return ls.GetLower();
		}
	}
	return "en";
}

decString deOSUnix::GetUserLocaleTerritory(){
	const char * const l = setlocale(LC_ALL, nullptr);
	if(l){
		const decString ls(l);
		const int deli = ls.Find('_');
		if(deli != -1){
			const int deli2 = ls.Find('.', deli + 1);
			if(deli2 != -1){
				return ls.GetMiddle(deli + 1, deli2).GetLower();
				
			}else{
				return ls.GetMiddle(deli + 1).GetLower();
			}
			
		}else{
			return ls.GetLower();
		}
	}
	return "";
}


// Wayland related
////////////////////

#ifdef OS_UNIX_WAYLAND
void deOSUnix::SetEnableWayland(bool enable){
	pEnableWayland = enable;
}
#endif


// Private Functions
//////////////////////

void deOSUnix::pCleanUp(){
#ifdef OS_UNIX_WAYLAND
	pCleanUpWayland();
#endif
	
	// close display
	if(pDisplay){
		XCloseDisplay(pDisplay);
		pDisplay = nullptr;
	}
	
	// reset error handler
	if(tempGlobalOS == this){
		XSetErrorHandler(nullptr);
		tempGlobalOS = nullptr;
	}
}

void deOSUnix::pSetWindowEventMask(){
	if(pCurWindow > 255 && pDisplay){
		XSetWindowAttributes swa;
		swa.event_mask = pEventMask;
		XChangeWindowAttributes(pDisplay, pCurWindow, CWEventMask, &swa);
	}
}

decString deOSUnix::pGetHomeDirectory(){
	// the user configuration directory is located under the user home directory.
	// can be changed at runtime using an environment parameter.
	const char *envPath = getenv("HOME");
	if(envPath){
		return decString(envPath);
	}
	
	envPath = getenv("USER");
	if(envPath){
		return decString("/home/") + envPath;
	}
	
	envPath = getenv("LOGUSER");
	if(envPath){
		return decString("/home/") + envPath;
	}
	
	DETHROW(deeInvalidParam);
}

void deOSUnix::pGetDisplayInformation(){
	if(!pDisplay){
		return;
	}
	
	const int screenCount = XScreenCount(pDisplay);
	if(screenCount == 0){
		return; // should never happen
	}
	
	// determine the total count of screen resolutions of all screens
	int totalResolutionCount = 0;
	int resolutionCount;
	int i, j;
	
	for(i=0; i<screenCount; i++){
		XRRSizes(pDisplay, i, &resolutionCount);
		totalResolutionCount += resolutionCount;
	}
	
	if(totalResolutionCount == 0){
		return; // should never happen
	}
	
	// create arrays and fill in the actual values
	pDisplayResolutions = decTList<decPoint>(totalResolutionCount);
	pDisplayInformation = decTList<sDisplayInformation>(screenCount);
	
	for(i=0; i<screenCount; i++){
		sDisplayInformation di;
		di.resolutions = pDisplayResolutions.GetArrayPointer() + pDisplayResolutions.GetCount();
		
		const XRRScreenSize * const resolutions = XRRSizes(pDisplay, i, &resolutionCount);
		for(j=0; j<resolutionCount; j++){
			pDisplayResolutions.Add(decPoint(resolutions[j].width, resolutions[j].height));
		}
		
		// the calls XWidthOfScreen and XHeightOfScreen do not work well if used on screens
		// spawning multiple monitors. the reason is that XWidthOfScreen and XHeightOfScreen
		// return the size if the virtual screen across all monitors. we need though to know
		// the size of the individual monitor screens to properly size windows. according
		// to documentation of xrandr the first returned resolution is the default screen
		// resolution. fall back to XWidthOfScreen and XHeightOfScreen in case no resolutions
		// have been returned
		if(resolutionCount == 0){
			Screen * const screen = XScreenOfDisplay(pDisplay, i);
			di.currentResolution.x = XWidthOfScreen(screen);
			di.currentResolution.y = XHeightOfScreen(screen);
			
		}else{
			di.currentResolution = di.resolutions[0];
		}
		
		XRRScreenConfiguration * const screenInfo = XRRGetScreenInfo(pDisplay, XRootWindow(pDisplay, i));
		if(screenInfo){
			// unfortunately the default screen resolution in XRandr is not necessarily the
			// current screen resolution. we have to query it explicitly. if this fails stick
			// with the default screen resolution
			{
			Rotation rot;
			int index = XRRConfigCurrentConfiguration(screenInfo, &rot);
			if(index >= 0 && index < resolutionCount){
				di.currentResolution = di.resolutions[index];
			}
			}
			
			// get the current refresh rate
			di.currentRefreshRate = XRRConfigCurrentRate(screenInfo);
			XRRFreeScreenConfigInfo(screenInfo);
			
		}else{
			di.currentRefreshRate = 60; // XRandR not supported on display. XLib has no way to get refresh rate
		}
		
		di.resolutionCount = resolutionCount;
		pDisplayInformation.Add(di);
	}
}

int deOSUnix::pGetGlobalScaling() const{
	if(!pDisplay){
		return 100;
	}
	
	const char * const resourceString = XResourceManagerString(pDisplay);
	if(!resourceString){
		return 100;
	}
	
	// printf( "Entire DB: %s\n", resourceString );
	
	XrmInitialize(); // initialize db before calling Xrm* functions
	
	const XrmDatabase db = XrmGetStringDatabase(resourceString);
	
	XrmValue value;
	char *type = nullptr;
	const auto result = XrmGetResource(db, "Xft.dpi", "String", &type, &value);
	
	XrmDestroyDatabase(db);
	
	if(result != True){
		return 100;
	}
	if(!value.addr){
		return 100;
	}
	
	const double scale = 100.0 * atof(value.addr) / 96.0;
	// printf( "Scale: %g %d\n", scale, decMath::max( ( int )( scale / 25.0 + 0.5 ) * 25, 100 ) );
	return decMath::max((int)(scale / 25.0 + 0.5) * 25, 100);
}

#ifdef OS_UNIX_WAYLAND

deOSUnix::sWaylandOutputInfo::sWaylandOutputInfo() :
output(wl_output_interface, 2, wl_output_destroy),
currentWidth(0),
currentHeight(0),
currentRefreshHz(0),
hasCurrentMode(false),
xdgOutput(nullptr),
logicalWidth(0),
logicalHeight(0){
}

void deOSUnix::OnOutputMode(void *data, wl_output*, uint32_t flags, int32_t width,
int32_t height, int32_t refresh){
	auto &info = *reinterpret_cast<sWaylandOutputInfo*>(data);
	info.modes.Add({width, height, (refresh + 500) / 1000});
	if(flags & WL_OUTPUT_MODE_CURRENT){
		info.currentWidth = width;
		info.currentHeight = height;
		info.currentRefreshHz = (refresh + 500) / 1000;
		info.hasCurrentMode = true;
	}
}

void deOSUnix::OnRegistryGlobal(void *data, wl_registry *registry, uint32_t name,
const char *interface, uint32_t version){
	auto &osunix = *reinterpret_cast<deOSUnix*>(data);
	
	if(osunix.pWaylandXdgOutputManager.Bind(registry, name, interface, version)){
		
	}else if(strcmp(interface, wl_output_interface.name) == 0){
		auto info = deTUniqueReference<sWaylandOutputInfo>::New();
		if(info->output.Bind(registry, name, interface, version)){
			static const wl_output_listener listener = {
				.geometry = [](void*, wl_output*, int32_t, int32_t, int32_t,
					int32_t, int32_t, const char*, const char*, int32_t){},
				.mode = OnOutputMode,
				.done = [](void*, wl_output*){},
				.scale = [](void*, wl_output*, int32_t){},
				.name = [](void*, wl_output*, const char*){},
				.description = [](void*, wl_output*, const char*){}
			};
			
			wl_output_add_listener(info->output, &listener, info.Pointer());
			osunix.pWaylandOutputs.Add(std::move(info));
		}
	}
}

static void sWlRegistryGlobalRemove(void*, wl_registry*, uint32_t){}

void deOSUnix::pGetWaylandDisplayInformation(){
	DEASSERT_NOTNULL(pWaylandDisplay)
	
	pWaylandRegistry = wl_display_get_registry(pWaylandDisplay);
	if(!pWaylandRegistry){
		DETHROW_INFO(deeInvalidAction, "Failed to get Wayland registry");
	}
	
	static const wl_registry_listener vWlRegistryListener = {
		.global = OnRegistryGlobal,
		.global_remove = sWlRegistryGlobalRemove
	};
	
	wl_proxy_add_listener(reinterpret_cast<wl_proxy*>(pWaylandRegistry),
		(void(**)(void))&vWlRegistryListener, this);
	
	// receive globals and bind wl_output objects with their listeners
	wl_display_roundtrip(pWaylandDisplay);
	
	if(pWaylandOutputs.IsEmpty()){
		pCleanUpWayland();
		DETHROW_INFO(deeInvalidAction, "No Wayland outputs found");
	}
	
	// obtain xdg outputs and register listeners
	if(pWaylandXdgOutputManager){
		pWaylandOutputs.Visit([&](sWaylandOutputInfo &info){
			if(!info.output){
				return;
			}
			
			info.xdgOutput = zxdg_output_manager_v1_get_xdg_output(pWaylandXdgOutputManager, info.output);
			if(!info.xdgOutput){
				return;
			}
			
			static const zxdg_output_v1_listener listener = {
				.logical_position = [](void*, zxdg_output_v1*, int32_t, int32_t){},
				.logical_size = [](void *data, zxdg_output_v1*, int32_t width, int32_t height){
					auto &info2 = *reinterpret_cast<sWaylandOutputInfo*>(data);
					info2.logicalWidth = width;
					info2.logicalHeight = height;
				},
				.done = [](void*, zxdg_output_v1*){},
				.name = [](void*, zxdg_output_v1*, const char*){},
				.description = [](void*, zxdg_output_v1*, const char*){}
			};
			
			zxdg_output_v1_add_listener(info.xdgOutput, &listener, &info);
		});
	}
	
	// receive mode events from all bound wl_output objects
	wl_display_roundtrip(pWaylandDisplay);
	
	// count valid outputs and total mode entries
	int validOutputs = 0, totalModes = 0;
	pWaylandOutputs.Visit([&](const sWaylandOutputInfo &info){
		if(info.hasCurrentMode){
			validOutputs++;
			totalModes += decMath::max(info.modes.GetCount(), 1);
		}
	});
	
	if(validOutputs == 0){
		pCleanUpWayland();
		DETHROW_INFO(deeInvalidAction, "No valid Wayland outputs found");
	}
	
	// store display information
	pDisplayResolutions = decTList<decPoint>(totalModes);
	pDisplayInformation = decTList<sDisplayInformation>(validOutputs);
	
	bool scaleFactorFound = false;
	
	pWaylandOutputs.Visit([&](const sWaylandOutputInfo &info){
		if(!info.hasCurrentMode){
			return;
		}
		
		sDisplayInformation di{};
		di.currentResolution = {info.currentWidth, info.currentHeight};
		di.currentRefreshRate = info.currentRefreshHz;
		di.resolutions = pDisplayResolutions.GetArrayPointer() + pDisplayResolutions.GetCount();
		
		if(info.modes.IsNotEmpty()){
			info.modes.Visit([&](const sWaylandOutputMode &mode){
				pDisplayResolutions.Add({mode.width, mode.height});
			});
			di.resolutionCount = info.modes.GetCount();
			
		}else{
			pDisplayResolutions.Add(di.currentResolution);
			di.resolutionCount = 1;
		}
		
		pDisplayInformation.Add(di);
		
		if(info.logicalWidth > 0 && !scaleFactorFound){
			const double scale = 100.0 * (double)info.currentWidth / (double)info.logicalWidth;
			pScaleFactor = decMath::max((int)(scale / 25.0 + 0.5) * 25, 100);
			scaleFactorFound = true;
		}
	});
	
	pWaylandOutputs.RemoveAll();
}

bool deOSUnix::pTryConnectWayland(){
	if(!v_WaylandLib.RequiredLog()){
		return false;
	}
	
	bool v = v_wl_display_connect.RequiredLog();
	v &= v_wl_display_disconnect.RequiredLog();
	v &= v_wl_display_dispatch.RequiredLog();
	v &= v_wl_display_dispatch_pending.RequiredLog();
	v &= v_wl_display_flush.RequiredLog();
	v &= v_wl_display_roundtrip.RequiredLog();
	v &= v_wl_proxy_marshal_flags.RequiredLog();
	v &= v_wl_proxy_add_listener.RequiredLog();
	v &= v_wl_proxy_destroy.RequiredLog();
	v &= v_wl_registry_interface.RequiredLog();
	v &= v_wl_output_interface.RequiredLog();
	if(!v){
		return false;
	}
	
	// try connecting to Wayland display
	pWaylandDisplay = wl_display_connect(nullptr);
	if(!pWaylandDisplay){
		printf("[OSUNIX] Cannot connect to Wayland display. WAYLAND_DISPLAY not set or compositor not running\n");
		return false;
	}
	
	return true;
}

void deOSUnix::pCleanUpWayland(){
	pWaylandOutputs.RemoveAll();
	
	pWaylandXdgOutputManager.Unbind();
	
	if(pWaylandRegistry){
		wl_proxy_destroy(reinterpret_cast<wl_proxy*>(pWaylandRegistry));
		pWaylandRegistry = nullptr;
	}
	
	if(pWaylandDisplay){
		wl_display_disconnect(pWaylandDisplay);
		pWaylandDisplay = nullptr;
	}
}

#endif // OS_UNIX_WAYLAND

#endif // OS_UNIX_X11
