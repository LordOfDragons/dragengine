/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2017, Plüss Roland ( roland@rptd.ch )
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglRRenderWindow.h"
#include "../canvas/render/deoglRCanvasView.h"
#include "../rendering/deoglRenderCanvasContext.h"
#include "../rendering/deoglRenderCanvas.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTContext.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTRenderers.h"

#include <dragengine/app/deOSMacOS.h>
#include <dragengine/common/exceptions.h>

#import <Cocoa/Cocoa.h>



// MacOS Specific
///////////////////

@interface deoglRRenderWindowWindow : NSWindow {
}
@end

@implementation deoglRRenderWindowWindow
// required to get mouse move and key events send to application
- (BOOL) canBecomeKeyWindow {
	return YES;
}

// required to get mouse move and key events send to application
- (BOOL) canBecomeMain {
	return YES;
}
@end



@interface deoglRRenderWindowDelegate : NSObject<NSWindowDelegate> {
	deoglRRenderWindow *pRenderWindow;
}
@end

@implementation deoglRRenderWindowDelegate
- (id) initRenderWindow:(deoglRRenderWindow*)renderWindow {
	if( self = [super init] ){
		pRenderWindow = renderWindow;
	}
	return self;
}

// required to get mouse move and key events send to application
- (void) windowDidBecomeKey:(NSNotification*)notification {
	// required to get mouse move and key events send to application. works only
	// if called after windowDidBecomeKey is invoked
	NSWindow * const window = pRenderWindow->GetWindow();
	[window setAcceptsMouseMovedEvents:YES];
	
	// tracking is not required on moden macos. maybe required on older macos
	/*
	NSTrackingAreaOptions trackingAreaOptions = NSTrackingActiveInKeyWindow
		| NSTrackingInVisibleRect | NSTrackingMouseMoved;
	NSView * const view = pRenderWindow->GetView();
	[view addTrackingArea:[[NSTrackingArea alloc] initWithRect:frame
		options:trackingAreaOptions owner:pView userInfo:nil]];
	*/
	
	// hide mouse cursor
	[NSCursor hide];
}

- (void) windowDidBecomeMain:(NSNotification*)notification {
	pRenderWindow->pMacOSDelegateWindowActivated();
}

- (void) windowDidResignMain:(NSNotification*)notification {
	pRenderWindow->pMacOSDelegateWindowDeactivated();
}

- (void) windowDidResize:(NSNotification*)notification {
	pRenderWindow->pMacOSDelegateWindowResized();
}

- (void) windowWillClose:(NSNotification*)notification {
	pRenderWindow->pMacOSDelegateWindowClosed();
}
@end



@interface deoglRRenderWindowView : NSView {
	deoglRRenderWindow *pRenderWindow;
}
@end

@implementation deoglRRenderWindowView
- (id) initWithFrame:(NSRect)frame :(deoglRRenderWindow *)renderWindow {
	if( self = [super initWithFrame:frame] ){
		pRenderWindow = renderWindow;
	}
	return self;
}

// required to get mouse move and key events send to application
- (BOOL) acceptsFirstResponder {
	return YES;
}

// required to get mouse move and key events send to application
- (BOOL) canBecomeKeyView {
	return YES;
}
@end



/**
 * Callback for rendering using display link.
 */
/*
static CVReturn fDisplayLinkCallback( CVDisplayLinkRef displayLink, const CVTimeStamp* now,
const CVTimeStamp* outputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext ){
	return [(__bridge *)displayLinkContext getFrameForTime:outputTime];
    return result;
}
*/


void deoglRRenderWindow::pMacOSCreateWindow(){
	// WARNING! CreateWindows is always called from the main thread. this is required since all
	//          UI related operations (especially window event handling) is done in the main
	//          thread. No OpenGL related objects are allowed to be touched in here.
	
	printf("pMacOSCreateWindow enter\n");
	
	// if the window exists we do nothing
	if( pWindow ){
		return;
	}
	
	NSRect frame = NSMakeRect( 0, 0, pWidth, pHeight );
	
	// for full screen NSBorderlessWindowMask .
	pWindow = [[deoglRRenderWindowWindow alloc] initWithContentRect:frame
		styleMask:(NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable)
		backing:NSBackingStoreBuffered defer:NO];
	[pWindow setBackgroundColor:[NSColor blackColor]];
	[pWindow setTitle:[NSString stringWithUTF8String:pTitle]];
	
	// create view and assign it to the window
	pView = [[deoglRRenderWindowView alloc] initWithFrame:frame :this]; // no autorelease. we snag the reference
	[pView setHidden:NO];
	[pView setNeedsDisplay:YES];
	
	[pWindow setContentView:pView];
	
	// required to get mouse move and key events send to application
	[pWindow setInitialFirstResponder:pView];
	
	// set delegate
	[pWindow setDelegate:[[deoglRRenderWindowDelegate alloc] initRenderWindow:this]];
	
	// acitvate application. required to get mouse move and key events send to application.
	[NSApp activateIgnoringOtherApps:YES];
	
	// make main app window and bring to front
	[pWindow makeKeyAndOrderFront:NSApp];
	
	// hide mouse pointer while inside
	//[NSCursor hide];
	
	printf("pMacOSCreateWindow exit\n");
}

void deoglRRenderWindow::pMacOSDestroyWindow(){
	printf("pMacOSDestroyWindow enter\n");
	
	if( pView ){
		// release view. works since we snagged a reference
		[pView release];
		pView = NULL;
	}
	
	if( pWindow ){
		// close window
		[pWindow close];
		
		// release window. works since we snagged a reference
		[pWindow release];
		pWindow = NULL;
	}
	
	printf("pMacOSDestroyWindow exit\n");
}

void deoglRRenderWindow::pMacOSSwapBuffers(){
	NSOpenGLContext *context = pRenderThread.GetContext().GetContext();
	[context flushBuffer];
	[pView setNeedsDisplay:YES];
}

void deoglRRenderWindow::pMacOSCenterOnScreen(){
	if( ! pWindow ){
		return;
	}
	[pWindow center];
}

void deoglRRenderWindow::pMacOSResizeWindow(){
	if( ! pWindow ){
		return;
	}
	
	printf("pMacOSResizeWindow enter\n");
	NSRect frame = [pWindow frame];
	NSRect resizedFrame = NSMakeRect( frame.origin.x, frame.origin.y, pWidth, pHeight );
	[pWindow setFrame:resizedFrame display:YES];
	printf("pMacOSResizeWindow exit\n");
}

void deoglRRenderWindow::pMacOSSetWindowTitle(){
	if( ! pWindow ){
		return;
	}
	printf("pMacOSSetWindowTitle enter\n");
	[pWindow setTitle:[NSString stringWithUTF8String:pTitle]];
	printf("pMacOSSetWindowTitle exit\n");
}

void deoglRRenderWindow::pMacOSUpdateFullscreen(){
	// nothing so far
}

void deoglRRenderWindow::pMacOSDelegateWindowClosed(){
	printf("pMacOSDelegateCloseWindow enter\n");
	
	// window is closing. drop the window without closing it
	pView = NULL;
	pWindow = NULL;
	
	// TODO tell the game engine about closing the window
	
	printf("pMacOSDelegateCloseWindow enter\n");
}

void deoglRRenderWindow::pMacOSDelegateWindowActivated(){
	printf("pMacOSDelegateWindowActivated\n");
	// TODO notify game scripts
}

void deoglRRenderWindow::pMacOSDelegateWindowDeactivated(){
	printf("pMacOSDelegateWindowDeactivated\n");
	// TODO notify game scripts
}

void deoglRRenderWindow::pMacOSDelegateWindowResized(){
	printf("pMacOSDelegateWindowResized\n");
	// TODO notify game scripts
}
