/*
 * Copyright (c) 2010,2012 Gražvydas Ignotas
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the organization nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * This is a slight modification of the work done by Gražvydas Ignotas, instead
 * of minimising the program when the Pandora button is pressed, no attempt is
 * made to minimise/restore the desktop.  Though, pressing twice on the Pandora
 * button will kill the program and return to the desktop.
 *
 * Rico Tyrell
 */
#include <PandoraFullscreen.hpp>
#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <termios.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/kd.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#define __user
#include <omapfb.h>

namespace Aura
{
	static pthread_cond_t g_StartCond = PTHREAD_COND_INITIALIZER;
	static pthread_mutex_t g_StartMutex = PTHREAD_MUTEX_INITIALIZER;
	static AUR_BOOL g_HaveX = AUR_FALSE;
	static AUR_SINT32 g_DesktopSaved;
	static int g_KeyboardFileDescriptor = -1;
	static struct termios g_KeyboardTerminal;
	static struct omapfb_plane_info g_DesktopPlaneInfo;
	static struct fb_var_screeninfo g_DesktopScreenInfo;
	static Display *g_pDisplay;
	static Window g_Window;

	static void SignalMainThread( )
	{
		pthread_mutex_lock( &g_StartMutex );
		pthread_cond_signal( &g_StartCond );
		pthread_mutex_unlock( &g_StartMutex );
	}

	static Cursor TransparentCursor( Display *p_pDisplay, Window p_Window )
	{
		Cursor TransparentCursor;
		Pixmap Pix;
		XColor DummyColour;
		char Data = 0;
		
		memset( &DummyColour, 0, sizeof( DummyColour ) );
		Pix = XCreateBitmapFromData( p_pDisplay, p_Window, &Data, 1, 1 );
		TransparentCursor = XCreatePixmapCursor( p_pDisplay, Pix, Pix,
			&DummyColour, &DummyColour, 0, 0 );
		XFreePixmap( p_pDisplay, Pix );

		return TransparentCursor;
	}

	static AUR_UINT32 SaveFramebufferState( AUR_SINT32 p_Index,
		struct omapfb_plane_info *p_pPlaneInfo,
		struct fb_var_screeninfo *p_pScreenInfo )
	{
		char Buffer[ 16 ];
		int FileDescriptor = -1;

		snprintf( Buffer, sizeof( Buffer ), "/dev/fb%d", p_Index );

		FileDescriptor = open( Buffer, O_RDWR );

		if( FileDescriptor == -1 )
		{
			std::cout << "[Aura::SaveFramebufferState] <ERROR> "
				"Failed to open framebuffer" << std::endl;

			return AUR_FAIL;
		}

		if( ioctl( FileDescriptor, OMAPFB_QUERY_PLANE, p_pPlaneInfo ) )
		{
			std::cout << "[Aura::SaveFramebufferState] <ERROR> "
				"Failed to query the framebuffer plane" << std::endl;

			close( FileDescriptor );

			return AUR_FAIL;
		}

		if( ioctl( FileDescriptor, FBIOGET_VSCREENINFO, p_pScreenInfo ) == -1 )
		{
			std::cout << "[Aura::SaveFramebufferState] <ERROR> "
				"Failed to get the framebuffer's screen information" <<
				std::endl;

			close( FileDescriptor );

			return AUR_FAIL;
		}

		if( ioctl( FileDescriptor, OMAPFB_SETUP_PLANE, p_pPlaneInfo ) )
		{
			std::cout << "[Aura::SaveFramebufferState] <ERROR> "
				"Failed to set up the framebuffer plane" << std::endl;

			close( FileDescriptor );

			return AUR_FAIL;
		}

		close( FileDescriptor );

		return AUR_OK;
	}

	static AUR_UINT32 LoadFramebufferState( AUR_SINT32 p_Index,
		struct omapfb_plane_info *p_pPlaneInfo,
		struct fb_var_screeninfo *p_pScreenInfo )
	{
		struct omapfb_plane_info PlaneInfo;
		char Buffer[ 16 ];
		int FileDescriptor = -1;

		snprintf( Buffer, sizeof( Buffer ), "/dev/fb%d", p_Index );

		FileDescriptor = open( Buffer, O_RDWR );

		if( FileDescriptor == -1 )
		{
			return AUR_FAIL;
		}

		if( ioctl( FileDescriptor, OMAPFB_QUERY_PLANE, &PlaneInfo ) )
		{
			close( FileDescriptor );

			return AUR_FAIL;
		}

		PlaneInfo.enabled = 0;
		PlaneInfo.pos_x = 0;
		PlaneInfo.pos_y = 0;

		if( ioctl( FileDescriptor, OMAPFB_SETUP_PLANE, &PlaneInfo ) )
		{
			close( FileDescriptor );

			return AUR_FAIL;
		}

		if( ioctl( FileDescriptor, FBIOPUT_VSCREENINFO, p_pScreenInfo ) == -1 )
		{
			close( FileDescriptor );

			return AUR_FAIL;
		}

		if( ioctl( FileDescriptor, OMAPFB_SETUP_PLANE, p_pPlaneInfo ) )
		{
			close( FileDescriptor );

			return AUR_FAIL;
		}

		return AUR_OK;
	}

	static void *X11Handler( void *p_pArg )
	{
		XInitThreads( );

		g_pDisplay = XOpenDisplay( AUR_NULL );

		if( g_pDisplay == AUR_NULL )
		{
			std::cout << "[Aura::X11Handler] <ERROR> "
				"Failed to open display: " << XDisplayName( AUR_NULL ) <<
				std::endl;

			SignalMainThread( );

			return AUR_NULL;
		}

		g_HaveX = AUR_TRUE;

		int XScreen = DefaultScreen( g_pDisplay );
		AUR_UINT32 DisplayWidth = DisplayWidth( g_pDisplay, XScreen );
		AUR_UINT32 DisplayHeight = DisplayHeight( g_pDisplay, XScreen );

		g_Window = XCreateSimpleWindow( g_pDisplay,
			RootWindow( g_pDisplay, XScreen ),
			0, 0, DisplayWidth, DisplayHeight, 0,
			WhitePixel( g_pDisplay, XScreen ),
			WhitePixel( g_pDisplay, XScreen ) );

		XSetWindowAttributes WindowAttributes;

		WindowAttributes.override_redirect = True;
		WindowAttributes.cursor = TransparentCursor( g_pDisplay, g_Window );

		XChangeWindowAttributes( g_pDisplay, g_Window,
			CWOverrideRedirect | CWCursor, &WindowAttributes );
		XSelectInput( g_pDisplay, g_Window, ExposureMask | FocusChangeMask |
			KeyPressMask | KeyReleaseMask | PropertyChangeMask );
		XStoreName( g_pDisplay, g_Window, "Aura | Hidden Window" );

		XMapWindow( g_pDisplay, g_Window );

		XGrabKeyboard( g_pDisplay, g_Window, False, GrabModeAsync,
			GrabModeAsync, CurrentTime );
		XkbSetDetectableAutoRepeat( g_pDisplay, True, AUR_NULL );
		XSync( g_pDisplay, False );

		XEvent Event;
		AUR_BOOL Run = AUR_TRUE;

		while( Run )
		{
			XNextEvent( g_pDisplay, &Event );

			switch( Event.type )
			{
				case Expose:
				{
					SignalMainThread( );

					while( XCheckTypedEvent( g_pDisplay, Expose, &Event ) )
						;
					break;
				}
				case ClientMessage:
				{
					if( *XGetAtomName( g_pDisplay,
						Event.xclient.message_type ) == *"WM_PROTOCOLS" )
					{
						Run = AUR_FALSE;
					}
					break;
				}
				default:
				{
					break;
				}
			}
		}

		XDestroyWindow( g_pDisplay, g_Window );
		XCloseDisplay( g_pDisplay );

		return AUR_NULL;
	}

	static void HideConsoleStart( )
	{
		struct termios KeyboardTerminal;
		int Mode;

		g_KeyboardFileDescriptor = open( "/dev/tty", O_RDWR );

		if( g_KeyboardFileDescriptor == -1 )
		{
			std::cout << "[Aura::HideConsoleStart] <ERROR> "
				"Failed to open /dev/tty" << std::endl;

			return;
		}

		if( ioctl( g_KeyboardFileDescriptor, KDGETMODE, &Mode ) == -1 )
		{
			std::cout << "[Aura::HideConsoleStart] <ERROR> "
				"Failed to obtain the current  mode of the terminal" <<
				std::endl;

			close( g_KeyboardFileDescriptor );
			g_KeyboardFileDescriptor = -1;

			return;
		}

		if( tcgetattr( g_KeyboardFileDescriptor, &KeyboardTerminal ) == -1 )
		{
			std::cout << "[Aura::HideConsoleStart] <ERROR> "
				"Failed to get the terminal's attributes" << std::endl;

			close( g_KeyboardFileDescriptor );
			g_KeyboardFileDescriptor = -1;

			return;
		}

		g_KeyboardTerminal = KeyboardTerminal;

		KeyboardTerminal.c_lflag &= ~( ICANON | ECHO );
		KeyboardTerminal.c_iflag &=
			~( ISTRIP | IGNCR | ICRNL | INLCR |IXOFF | IXON );
		KeyboardTerminal.c_cc[ VMIN ] = 0;
		KeyboardTerminal.c_cc[ VTIME ] = 0;

		if( tcsetattr( g_KeyboardFileDescriptor, TCSAFLUSH,
			&KeyboardTerminal ) == -1 )
		{
			std::cout << "[Aura::HideConsoleStart] <ERROR> "
				"Failed to set the keyboard terminal attributes" << std::endl;

			close( g_KeyboardFileDescriptor );
			g_KeyboardFileDescriptor = -1;

			return;
		}

		if( ioctl( g_KeyboardFileDescriptor, KDSETMODE, KD_GRAPHICS ) == -1 )
		{
			std::cout << "[Aura::HideConsoleStart] <ERROR> "
				"Failed to set terminal graphics mode, reverting" << std::endl;

			tcsetattr( g_KeyboardFileDescriptor, TCSAFLUSH,
				&g_KeyboardTerminal );

			close( g_KeyboardFileDescriptor );
			g_KeyboardFileDescriptor = -1;

			return;
		}
	}

	static void HideConsoleEnd( )
	{
		if( g_KeyboardFileDescriptor < 0 )
		{
			return;
		}

		if( ioctl( g_KeyboardFileDescriptor, KDSETMODE, KD_TEXT ) == -1 )
		{
			std::cout << "[Aura::HideConsoleEnd] <WARN> "
				"Could not set keyboard terminal mode to text" << std::endl;
		}

		if( ioctl( g_KeyboardFileDescriptor, TCSAFLUSH,
			&g_KeyboardTerminal ) == -1 )
		{
			std::cout << "[Aura::HideConsoleEnd] <WARN> "
				"Could not revert the keyboard terminal" << std::endl;
		}

		close( g_KeyboardFileDescriptor );
		g_KeyboardFileDescriptor = -1;
	}

	pthread_t g_ThreadID;

	AUR_UINT32 EnterPandoraFullscreen( )
	{
		// Create a hidden X window to consume the XEvent messages
		char *pThreadName;
		pThreadName = new char[ 80 ];
		strncpy( pThreadName, "HiddenWindow", strlen( pThreadName ) );
		pthread_mutex_lock( &g_StartMutex );
		int Error = pthread_create( &g_ThreadID, AUR_NULL, X11Handler,
			pThreadName );
		delete [ ] pThreadName;

		if( Error )
		{
			std::cout << "[Aura::EnterPandoraFullscreen] <ERROR> "
				"Failed to create thread: " << Error << std::endl;

			return AUR_FAIL;
		}

		pthread_detach( g_ThreadID );

		if( SaveFramebufferState( 0, &g_DesktopPlaneInfo,
			&g_DesktopScreenInfo ) == AUR_OK )
		{
			g_DesktopSaved = AUR_TRUE;
		}
		else
		{
			std::cout << "[Aura::EnterPandoraFullscreen] <ERROR> "
				"Failed to save the initial desktop state" << std::endl;
		}

		pthread_cond_wait( &g_StartCond, &g_StartMutex );
		pthread_mutex_unlock( &g_StartMutex );

		// Did the program start in one of the terminals?
		if( g_HaveX == AUR_FALSE )
		{
			HideConsoleStart( );
		}

		return AUR_OK;
	}

	AUR_UINT32 ExitPandoraFullscreen( )
	{
		// Revert the terminal's settings if the program was not lauched from X
		if( g_HaveX == AUR_FALSE )
		{
			HideConsoleEnd( );
		}

		if( g_HaveX == AUR_TRUE )
		{
			XEvent Event;
			memset( &Event, 0, sizeof( Event ) );
			Event.xclient.type = ClientMessage;
			Event.xclient.window = g_Window;
			Event.xclient.message_type = XInternAtom( g_pDisplay,
				"WM_PROTOCOLS", True );
			Event.xclient.format = 32;
			Event.xclient.data.l[ 0 ] = XInternAtom( g_pDisplay,
				"WM_DELETE_WINDOW", False );
			Event.xclient.data.l[ 1 ] = CurrentTime;

			XSendEvent( g_pDisplay, g_Window, False, NoEventMask, &Event );
			XFlush( g_pDisplay );
		}

		pthread_join( g_ThreadID, AUR_NULL );

		if( g_DesktopSaved == AUR_TRUE )
		{
			if( LoadFramebufferState( 0, &g_DesktopPlaneInfo,
				&g_DesktopScreenInfo ) != AUR_OK )
			{
			}
		}

		return AUR_OK;
	}
}

