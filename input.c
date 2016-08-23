/* input.c: generalised input events layer for Fuse
   Copyright (c) 2004 Philip Kendall

   $Id$

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

   Author contact information:

   E-mail: philip-fuse@shadowmagic.org.uk

*/

#include <config.h>

#include "fuse.h"
#include "input.h"
#include "keyboard.h"
#include "peripherals/joystick.h"
#include "settings.h"
#include "snapshot.h"
#include "tape.h"
#include "ui/ui.h"
#include "utils.h"

static int keypress( const input_event_key_t *event );
static int keyrelease( const input_event_key_t *event );
static int do_joystick( const input_event_joystick_t *joystick_event,
			int press );

int
input_event( const input_event_t *event )
{

  switch( event->type ) {

  case INPUT_EVENT_KEYPRESS: return keypress( &( event->types.key ) );
  case INPUT_EVENT_KEYRELEASE: return keyrelease( &( event->types.key ) );

  case INPUT_EVENT_JOYSTICK_PRESS:
    return do_joystick( &( event->types.joystick ), 1 );
  case INPUT_EVENT_JOYSTICK_RELEASE:
    return do_joystick( &( event->types.joystick ), 0 );

  }

  ui_error( UI_ERROR_ERROR, "unknown input event type %d", event->type );
  return 1;

}

static const char* keyname( input_key k ) {
    switch ( k ) {
      case INPUT_KEY_NONE: return "INPUT_KEY_NONE";
      case INPUT_KEY_Tab: return "INPUT_KEY_Tab";
      case INPUT_KEY_Return: return "INPUT_KEY_Return";
      case INPUT_KEY_Escape: return "INPUT_KEY_Escape";
      case INPUT_KEY_space: return "INPUT_KEY_space";
      case INPUT_KEY_exclam: return "INPUT_KEY_exclam";
      case INPUT_KEY_numbersign: return "INPUT_KEY_numbersign";
      case INPUT_KEY_dollar: return "INPUT_KEY_dollar";
      case INPUT_KEY_ampersand: return "INPUT_KEY_ampersand";
      case INPUT_KEY_apostrophe: return "INPUT_KEY_apostrophe";
      case INPUT_KEY_parenleft: return "INPUT_KEY_parenleft";
      case INPUT_KEY_parenright: return "INPUT_KEY_parenright";
      case INPUT_KEY_asterisk: return "INPUT_KEY_asterisk";
      case INPUT_KEY_plus: return "INPUT_KEY_plus";
      case INPUT_KEY_comma: return "INPUT_KEY_comma";
      case INPUT_KEY_minus: return "INPUT_KEY_minus";
      case INPUT_KEY_period: return "INPUT_KEY_period";
      case INPUT_KEY_slash: return "INPUT_KEY_slash";
      case INPUT_KEY_0: return "INPUT_KEY_0";
      case INPUT_KEY_1: return "INPUT_KEY_1";
      case INPUT_KEY_2: return "INPUT_KEY_2";
      case INPUT_KEY_3: return "INPUT_KEY_3";
      case INPUT_KEY_4: return "INPUT_KEY_4";
      case INPUT_KEY_5: return "INPUT_KEY_5";
      case INPUT_KEY_6: return "INPUT_KEY_6";
      case INPUT_KEY_7: return "INPUT_KEY_7";
      case INPUT_KEY_8: return "INPUT_KEY_8";
      case INPUT_KEY_9: return "INPUT_KEY_9";
      case INPUT_KEY_colon: return "INPUT_KEY_colon";
      case INPUT_KEY_semicolon: return "INPUT_KEY_semicolon";
      case INPUT_KEY_less: return "INPUT_KEY_less";
      case INPUT_KEY_equal: return "INPUT_KEY_equal";
      case INPUT_KEY_greater: return "INPUT_KEY_greater";
      case INPUT_KEY_A: return "INPUT_KEY_A";
      case INPUT_KEY_B: return "INPUT_KEY_B";
      case INPUT_KEY_C: return "INPUT_KEY_C";
      case INPUT_KEY_D: return "INPUT_KEY_D";
      case INPUT_KEY_E: return "INPUT_KEY_E";
      case INPUT_KEY_F: return "INPUT_KEY_F";
      case INPUT_KEY_G: return "INPUT_KEY_G";
      case INPUT_KEY_H: return "INPUT_KEY_H";
      case INPUT_KEY_I: return "INPUT_KEY_I";
      case INPUT_KEY_J: return "INPUT_KEY_J";
      case INPUT_KEY_K: return "INPUT_KEY_K";
      case INPUT_KEY_L: return "INPUT_KEY_L";
      case INPUT_KEY_M: return "INPUT_KEY_M";
      case INPUT_KEY_N: return "INPUT_KEY_N";
      case INPUT_KEY_O: return "INPUT_KEY_O";
      case INPUT_KEY_P: return "INPUT_KEY_P";
      case INPUT_KEY_Q: return "INPUT_KEY_Q";
      case INPUT_KEY_R: return "INPUT_KEY_R";
      case INPUT_KEY_S: return "INPUT_KEY_S";
      case INPUT_KEY_T: return "INPUT_KEY_T";
      case INPUT_KEY_U: return "INPUT_KEY_U";
      case INPUT_KEY_V: return "INPUT_KEY_V";
      case INPUT_KEY_W: return "INPUT_KEY_W";
      case INPUT_KEY_X: return "INPUT_KEY_X";
      case INPUT_KEY_Y: return "INPUT_KEY_Y";
      case INPUT_KEY_Z: return "INPUT_KEY_Z";
      case INPUT_KEY_bracketleft: return "INPUT_KEY_bracketleft";
      case INPUT_KEY_bracketright: return "INPUT_KEY_bracketright";
      case INPUT_KEY_asciicircum: return "INPUT_KEY_asciicircum";
//    case INPUT_KEY_dead_circumflex: return "INPUT_KEY_dead_circumflex";
      case INPUT_KEY_a: return "INPUT_KEY_a";
      case INPUT_KEY_b: return "INPUT_KEY_b";
      case INPUT_KEY_c: return "INPUT_KEY_c";
      case INPUT_KEY_d: return "INPUT_KEY_d";
      case INPUT_KEY_e: return "INPUT_KEY_e";
      case INPUT_KEY_f: return "INPUT_KEY_f";
      case INPUT_KEY_g: return "INPUT_KEY_g";
      case INPUT_KEY_h: return "INPUT_KEY_h";
      case INPUT_KEY_i: return "INPUT_KEY_i";
      case INPUT_KEY_j: return "INPUT_KEY_j";
      case INPUT_KEY_k: return "INPUT_KEY_k";
      case INPUT_KEY_l: return "INPUT_KEY_l";
      case INPUT_KEY_m: return "INPUT_KEY_m";
      case INPUT_KEY_n: return "INPUT_KEY_n";
      case INPUT_KEY_o: return "INPUT_KEY_o";
      case INPUT_KEY_p: return "INPUT_KEY_p";
      case INPUT_KEY_q: return "INPUT_KEY_q";
      case INPUT_KEY_r: return "INPUT_KEY_r";
      case INPUT_KEY_s: return "INPUT_KEY_s";
      case INPUT_KEY_t: return "INPUT_KEY_t";
      case INPUT_KEY_u: return "INPUT_KEY_u";
      case INPUT_KEY_v: return "INPUT_KEY_v";
      case INPUT_KEY_w: return "INPUT_KEY_w";
      case INPUT_KEY_x: return "INPUT_KEY_x";
      case INPUT_KEY_y: return "INPUT_KEY_y";
      case INPUT_KEY_z: return "INPUT_KEY_z";
      case INPUT_KEY_bar: return "INPUT_KEY_bar";
      case INPUT_KEY_BackSpace: return "INPUT_KEY_BackSpace";
      case INPUT_KEY_KP_Enter: return "INPUT_KEY_KP_Enter";
      case INPUT_KEY_Up: return "INPUT_KEY_Up";
      case INPUT_KEY_Down: return "INPUT_KEY_Down";
      case INPUT_KEY_Left: return "INPUT_KEY_Left";
      case INPUT_KEY_Right: return "INPUT_KEY_Right";
      case INPUT_KEY_Insert: return "INPUT_KEY_Insert";
      case INPUT_KEY_Delete: return "INPUT_KEY_Delete";
      case INPUT_KEY_Home: return "INPUT_KEY_Home";
      case INPUT_KEY_End: return "INPUT_KEY_End";
      case INPUT_KEY_Page_Up: return "INPUT_KEY_Page_Up";
      case INPUT_KEY_Page_Down: return "INPUT_KEY_Page_Down";
      case INPUT_KEY_Caps_Lock: return "INPUT_KEY_Caps_Lock";
      case INPUT_KEY_F1: return "INPUT_KEY_F1";
      case INPUT_KEY_F2: return "INPUT_KEY_F2";
      case INPUT_KEY_F3: return "INPUT_KEY_F3";
      case INPUT_KEY_F4: return "INPUT_KEY_F4";
      case INPUT_KEY_F5: return "INPUT_KEY_F5";
      case INPUT_KEY_F6: return "INPUT_KEY_F6";
      case INPUT_KEY_F7: return "INPUT_KEY_F7";
      case INPUT_KEY_F8: return "INPUT_KEY_F8";
      case INPUT_KEY_F9: return "INPUT_KEY_F9";
      case INPUT_KEY_F10: return "INPUT_KEY_F10";
      case INPUT_KEY_F11: return "INPUT_KEY_F11";
      case INPUT_KEY_F12: return "INPUT_KEY_F12";
      case INPUT_KEY_Shift_L: return "INPUT_KEY_Shift_L";
      case INPUT_KEY_Shift_R: return "INPUT_KEY_Shift_R";
      case INPUT_KEY_Control_L: return "INPUT_KEY_Control_L";
      case INPUT_KEY_Control_R: return "INPUT_KEY_Control_R";
      case INPUT_KEY_Alt_L: return "INPUT_KEY_Alt_L";
      case INPUT_KEY_Alt_R: return "INPUT_KEY_Alt_R";
      case INPUT_KEY_Meta_L: return "INPUT_KEY_Meta_L";
      case INPUT_KEY_Meta_R: return "INPUT_KEY_Meta_R";
      case INPUT_KEY_Super_L: return "INPUT_KEY_Super_L";
      case INPUT_KEY_Super_R: return "INPUT_KEY_Super_R";
      case INPUT_KEY_Hyper_L: return "INPUT_KEY_Hyper_L";
      case INPUT_KEY_Hyper_R: return "INPUT_KEY_Hyper_R";
      case INPUT_KEY_Mode_switch: return "INPUT_KEY_Mode_switch";
      case INPUT_JOYSTICK_UP: return "INPUT_JOYSTICK_UP";
      case INPUT_JOYSTICK_DOWN: return "INPUT_JOYSTICK_DOWN";
      case INPUT_JOYSTICK_LEFT: return "INPUT_JOYSTICK_LEFT";
      case INPUT_JOYSTICK_RIGHT: return "INPUT_JOYSTICK_RIGHT";
      case INPUT_JOYSTICK_FIRE_1: return "INPUT_JOYSTICK_FIRE_1";
      case INPUT_JOYSTICK_FIRE_2: return "INPUT_JOYSTICK_FIRE_2";
      case INPUT_JOYSTICK_FIRE_3: return "INPUT_JOYSTICK_FIRE_3";
      case INPUT_JOYSTICK_FIRE_4: return "INPUT_JOYSTICK_FIRE_4";
      case INPUT_JOYSTICK_FIRE_5: return "INPUT_JOYSTICK_FIRE_5";
      case INPUT_JOYSTICK_FIRE_6: return "INPUT_JOYSTICK_FIRE_6";
      case INPUT_JOYSTICK_FIRE_7: return "INPUT_JOYSTICK_FIRE_7";
      case INPUT_JOYSTICK_FIRE_8: return "INPUT_JOYSTICK_FIRE_8";
      case INPUT_JOYSTICK_FIRE_9: return "INPUT_JOYSTICK_FIRE_9";
      case INPUT_JOYSTICK_FIRE_10: return "INPUT_JOYSTICK_FIRE_10";
      case INPUT_JOYSTICK_FIRE_11: return "INPUT_JOYSTICK_FIRE_11";
      case INPUT_JOYSTICK_FIRE_12: return "INPUT_JOYSTICK_FIRE_12";
      case INPUT_JOYSTICK_FIRE_13: return "INPUT_JOYSTICK_FIRE_13";
      case INPUT_JOYSTICK_FIRE_14: return "INPUT_JOYSTICK_FIRE_14";
      case INPUT_JOYSTICK_FIRE_15: return "INPUT_JOYSTICK_FIRE_15";
      default: return 0;
    }
}

static void
keyname_print( char* buf, size_t bufsz, input_key k ) {
    const char* s = keyname( k );
    if ( s ) {
        snprintf( buf, bufsz, "%s (code %d, asc '%c')", s, (int) k, (int)(k>=32&&k<=126?k:'.') );
    }
    else {
        snprintf( buf, bufsz, "key code %d, asc '%c'", (int) k, (int)(k>=32&&k<=126?k:'.') );
    }
}

static int rzx_is_downkey( int code ) {
    switch ( code ) {
        case INPUT_KEY_a:                                   return INPUT_KEY_1;
        case INPUT_KEY_c:                                   return INPUT_KEY_2;
        case INPUT_KEY_e:                                   return INPUT_KEY_3;
        case INPUT_KEY_g:                                   return INPUT_KEY_4;
        case INPUT_KEY_i:                                   return INPUT_KEY_5;
        case INPUT_KEY_k:                                   return INPUT_KEY_6;
        case INPUT_KEY_m:                                   return INPUT_KEY_7;
        case INPUT_KEY_o:                                   return INPUT_KEY_8;
        case INPUT_KEY_q:                                   return INPUT_KEY_9;
        case INPUT_KEY_s:                                   return INPUT_KEY_0;
        case INPUT_KEY_u:                                   return INPUT_KEY_q;
        case INPUT_KEY_w:                                   return INPUT_KEY_w;
        case INPUT_KEY_y:                                   return INPUT_KEY_e;
        case INPUT_KEY_Shift_L | INPUT_KEY_a:               return INPUT_KEY_r;
        case INPUT_KEY_Shift_L | INPUT_KEY_c:               return INPUT_KEY_t;
        case INPUT_KEY_Shift_L | INPUT_KEY_e:               return INPUT_KEY_y;
        case INPUT_KEY_Shift_L | INPUT_KEY_g:               return INPUT_KEY_u;
        case INPUT_KEY_Shift_L | INPUT_KEY_i:               return INPUT_KEY_i;
        case INPUT_KEY_Shift_L | INPUT_KEY_k:               return INPUT_KEY_o;
        case INPUT_KEY_Shift_L | INPUT_KEY_m:               return INPUT_KEY_p;
        case INPUT_KEY_Shift_L | INPUT_KEY_o:               return INPUT_KEY_a;
        case INPUT_KEY_Shift_L | INPUT_KEY_q:               return INPUT_KEY_s;
        case INPUT_KEY_Shift_L | INPUT_KEY_s:               return INPUT_KEY_d;
        case INPUT_KEY_Shift_L | INPUT_KEY_u:               return INPUT_KEY_f;
        case INPUT_KEY_Shift_L | INPUT_KEY_w:               return INPUT_KEY_g;
        case INPUT_KEY_Shift_L | INPUT_KEY_y:               return INPUT_KEY_h;
        case INPUT_KEY_0:                                   return INPUT_KEY_j;
        case INPUT_KEY_2:                                   return INPUT_KEY_k;
        case INPUT_KEY_4:                                   return INPUT_KEY_l;
        case INPUT_KEY_6:                                   return INPUT_KEY_Return;
        case INPUT_KEY_8:                                   return INPUT_KEY_Shift_L;
        case INPUT_KEY_Shift_L | INPUT_KEY_comma:           return INPUT_KEY_z;
        case INPUT_KEY_minus:                               return INPUT_KEY_x;
        case INPUT_KEY_bracketleft:                         return INPUT_KEY_c;
        case INPUT_KEY_semicolon:                           return INPUT_KEY_v;
        case INPUT_KEY_comma:                               return INPUT_KEY_b;
        case INPUT_KEY_slash:                               return INPUT_KEY_n;
        case INPUT_KEY_Shift_L | INPUT_KEY_bracketleft:     return INPUT_KEY_m;
        case INPUT_KEY_Shift_L | INPUT_KEY_1:               return INPUT_KEY_Control_R;
        case INPUT_KEY_Shift_L | INPUT_KEY_5:               return INPUT_KEY_space;
    }
    return 0;
}

static int rzx_is_upkey( int code ) {
    switch ( code ) {
        case INPUT_KEY_b:                                   return INPUT_KEY_1;
        case INPUT_KEY_d:                                   return INPUT_KEY_2;
        case INPUT_KEY_f:                                   return INPUT_KEY_3;
        case INPUT_KEY_h:                                   return INPUT_KEY_4;
        case INPUT_KEY_j:                                   return INPUT_KEY_5;
        case INPUT_KEY_l:                                   return INPUT_KEY_6;
        case INPUT_KEY_n:                                   return INPUT_KEY_7;
        case INPUT_KEY_p:                                   return INPUT_KEY_8;
        case INPUT_KEY_r:                                   return INPUT_KEY_9;
        case INPUT_KEY_t:                                   return INPUT_KEY_0;
        case INPUT_KEY_v:                                   return INPUT_KEY_q;
        case INPUT_KEY_x:                                   return INPUT_KEY_w;
        case INPUT_KEY_z:                                   return INPUT_KEY_e;
        case INPUT_KEY_Shift_L | INPUT_KEY_b:               return INPUT_KEY_r;
        case INPUT_KEY_Shift_L | INPUT_KEY_d:               return INPUT_KEY_t;
        case INPUT_KEY_Shift_L | INPUT_KEY_f:               return INPUT_KEY_y;
        case INPUT_KEY_Shift_L | INPUT_KEY_h:               return INPUT_KEY_u;
        case INPUT_KEY_Shift_L | INPUT_KEY_j:               return INPUT_KEY_i;
        case INPUT_KEY_Shift_L | INPUT_KEY_l:               return INPUT_KEY_o;
        case INPUT_KEY_Shift_L | INPUT_KEY_n:               return INPUT_KEY_p;
        case INPUT_KEY_Shift_L | INPUT_KEY_p:               return INPUT_KEY_a;
        case INPUT_KEY_Shift_L | INPUT_KEY_r:               return INPUT_KEY_s;
        case INPUT_KEY_Shift_L | INPUT_KEY_t:               return INPUT_KEY_d;
        case INPUT_KEY_Shift_L | INPUT_KEY_v:               return INPUT_KEY_f;
        case INPUT_KEY_Shift_L | INPUT_KEY_x:               return INPUT_KEY_g;
        case INPUT_KEY_Shift_L | INPUT_KEY_z:               return INPUT_KEY_h;
        case INPUT_KEY_1:                                   return INPUT_KEY_j;
        case INPUT_KEY_3:                                   return INPUT_KEY_k;
        case INPUT_KEY_5:                                   return INPUT_KEY_l;
        case INPUT_KEY_7:                                   return INPUT_KEY_Return;
        case INPUT_KEY_9:                                   return INPUT_KEY_Shift_L;
        case INPUT_KEY_Shift_L | INPUT_KEY_period:          return INPUT_KEY_z;
        case INPUT_KEY_equal:                               return INPUT_KEY_x;
        case INPUT_KEY_bracketright:                        return INPUT_KEY_c;
        case INPUT_KEY_Shift_L | INPUT_KEY_semicolon:       return INPUT_KEY_v;
        case INPUT_KEY_period:                              return INPUT_KEY_b;
        case INPUT_KEY_Shift_L | INPUT_KEY_slash:           return INPUT_KEY_n;
        case INPUT_KEY_Shift_L | INPUT_KEY_bracketright:    return INPUT_KEY_m;
        case INPUT_KEY_Shift_L | INPUT_KEY_4:               return INPUT_KEY_Control_R;
        case INPUT_KEY_Shift_L | INPUT_KEY_6:               return INPUT_KEY_space;
    }
    return 0;
}

static int rzx_key_down = 0;

#define RZXF_PRESSED    1
#define RZXF_RELEASED   2

static void rzx_press( input_key k, input_key* kout, int* kflg ) {

    int o;

    if ( k == INPUT_KEY_Shift_L ) rzx_key_down |= INPUT_KEY_Shift_L;
    if ( k >= 0 && k < 256      ) rzx_key_down =  ( rzx_key_down & ~255 ) | k;

    o = rzx_is_upkey( rzx_key_down );
    if ( o ) {
        *kout = (input_key) o;
        *kflg = RZXF_RELEASED;
        rzx_key_down = 0;
        return;
    }

    o = rzx_is_downkey( rzx_key_down );
    if ( o ) {
        *kout = (input_key) o;
        *kflg = RZXF_PRESSED;
        rzx_key_down = 0;
        return;
    }

}

static int
keypress( const input_event_key_t *event )
{
  int swallow;
  const keyboard_spectrum_keys_t *ptr;

  if( ui_widget_level >= 0 ) {
    ui_widget_keyhandler( event->native_key );
    return 0;
  }

  /* Escape => ask UI to end mouse grab, return if grab ended */
  if( event->native_key == INPUT_KEY_Escape && ui_mouse_grabbed ) {
    ui_mouse_grabbed = ui_mouse_release( 0 );
    if( !ui_mouse_grabbed ) return 0;
  }

  swallow = 0;
  /* Joystick emulation via keyboard keys */
  if ( event->spectrum_key == settings_current.joystick_keyboard_up ) {
    swallow = joystick_press( JOYSTICK_KEYBOARD, JOYSTICK_BUTTON_UP   , 1 );
  }
  else if( event->spectrum_key == settings_current.joystick_keyboard_down ) {
    swallow = joystick_press( JOYSTICK_KEYBOARD, JOYSTICK_BUTTON_DOWN , 1 );
  }
  else if( event->spectrum_key == settings_current.joystick_keyboard_left ) {
    swallow = joystick_press( JOYSTICK_KEYBOARD, JOYSTICK_BUTTON_LEFT , 1 );
  }
  else if( event->spectrum_key == settings_current.joystick_keyboard_right ) {
    swallow = joystick_press( JOYSTICK_KEYBOARD, JOYSTICK_BUTTON_RIGHT, 1 );
  }
  else if( event->spectrum_key == settings_current.joystick_keyboard_fire ) {
    swallow = joystick_press( JOYSTICK_KEYBOARD, JOYSTICK_BUTTON_FIRE , 1 );
  }

  if( swallow ) return 0;

  /*
  char tmp[256];
  keyname_print( tmp, sizeof(tmp), event->spectrum_key );
  printf( "keypress(): %s\n", tmp );
  */

  input_key kout = (input_key) 0; int kflg = 0;
  rzx_press( event->spectrum_key, &kout, &kflg );
  if ( kflg & RZXF_RELEASED ) {
      ptr = keyboard_get_spectrum_keys( kout );
      if( ptr ) {
        keyboard_release( ptr->key1 );
        keyboard_release( ptr->key2 );
      }
  }
  else if ( kflg & RZXF_PRESSED ) {
      ptr = keyboard_get_spectrum_keys( kout );
      if( ptr ) {
        keyboard_press( ptr->key1 );
        keyboard_press( ptr->key2 );
      }
  }

  /*
  ptr = keyboard_get_spectrum_keys( event->spectrum_key );

  if( ptr ) {
    keyboard_press( ptr->key1 );
    keyboard_press( ptr->key2 );
  }
  */

  ui_popup_menu( event->native_key );

  return 0;
}

static int
keyrelease( const input_event_key_t *event )
{
  const keyboard_spectrum_keys_t *ptr;

  /*
  char tmp[256];
  keyname_print( tmp, sizeof(tmp), event->spectrum_key );
  printf( "keyrelease(): %s\n", tmp );

  ptr = keyboard_get_spectrum_keys( event->spectrum_key );

  if( ptr ) {
    keyboard_release( ptr->key1 );
    keyboard_release( ptr->key2 );
  }
  */

  /* Joystick emulation via keyboard keys */
  if( event->spectrum_key == settings_current.joystick_keyboard_up ) {
    joystick_press( JOYSTICK_KEYBOARD, JOYSTICK_BUTTON_UP   , 0 );
  }
  else if( event->spectrum_key == settings_current.joystick_keyboard_down ) {
    joystick_press( JOYSTICK_KEYBOARD, JOYSTICK_BUTTON_DOWN , 0 );
  }
  else if( event->spectrum_key == settings_current.joystick_keyboard_left ) {
    joystick_press( JOYSTICK_KEYBOARD, JOYSTICK_BUTTON_LEFT , 0 );
  }
  else if( event->spectrum_key == settings_current.joystick_keyboard_right ) {
    joystick_press( JOYSTICK_KEYBOARD, JOYSTICK_BUTTON_RIGHT, 0 );
  }
  else if( event->spectrum_key == settings_current.joystick_keyboard_fire ) {
    joystick_press( JOYSTICK_KEYBOARD, JOYSTICK_BUTTON_FIRE , 0 );
  }

  return 0;
}

static keyboard_key_name
get_fire_button_key( int which, input_key button )
{
  switch( which ) {

  case 0:
    switch( button ) {
    case INPUT_JOYSTICK_FIRE_1 : return settings_current.joystick_1_fire_1;
    case INPUT_JOYSTICK_FIRE_2 : return settings_current.joystick_1_fire_2;
    case INPUT_JOYSTICK_FIRE_3 : return settings_current.joystick_1_fire_3;
    case INPUT_JOYSTICK_FIRE_4 : return settings_current.joystick_1_fire_4;
    case INPUT_JOYSTICK_FIRE_5 : return settings_current.joystick_1_fire_5;
    case INPUT_JOYSTICK_FIRE_6 : return settings_current.joystick_1_fire_6;
    case INPUT_JOYSTICK_FIRE_7 : return settings_current.joystick_1_fire_7;
    case INPUT_JOYSTICK_FIRE_8 : return settings_current.joystick_1_fire_8;
    case INPUT_JOYSTICK_FIRE_9 : return settings_current.joystick_1_fire_9;
    case INPUT_JOYSTICK_FIRE_10: return settings_current.joystick_1_fire_10;
    case INPUT_JOYSTICK_FIRE_11: return settings_current.joystick_1_fire_11;
    case INPUT_JOYSTICK_FIRE_12: return settings_current.joystick_1_fire_12;
    case INPUT_JOYSTICK_FIRE_13: return settings_current.joystick_1_fire_13;
    case INPUT_JOYSTICK_FIRE_14: return settings_current.joystick_1_fire_14;
    case INPUT_JOYSTICK_FIRE_15: return settings_current.joystick_1_fire_15;
    default: break;
    }
    break;

  case 1:
    switch( button ) {
    case INPUT_JOYSTICK_FIRE_1 : return settings_current.joystick_2_fire_1;
    case INPUT_JOYSTICK_FIRE_2 : return settings_current.joystick_2_fire_2;
    case INPUT_JOYSTICK_FIRE_3 : return settings_current.joystick_2_fire_3;
    case INPUT_JOYSTICK_FIRE_4 : return settings_current.joystick_2_fire_4;
    case INPUT_JOYSTICK_FIRE_5 : return settings_current.joystick_2_fire_5;
    case INPUT_JOYSTICK_FIRE_6 : return settings_current.joystick_2_fire_6;
    case INPUT_JOYSTICK_FIRE_7 : return settings_current.joystick_2_fire_7;
    case INPUT_JOYSTICK_FIRE_8 : return settings_current.joystick_2_fire_8;
    case INPUT_JOYSTICK_FIRE_9 : return settings_current.joystick_2_fire_9;
    case INPUT_JOYSTICK_FIRE_10: return settings_current.joystick_2_fire_10;
    case INPUT_JOYSTICK_FIRE_11: return settings_current.joystick_2_fire_11;
    case INPUT_JOYSTICK_FIRE_12: return settings_current.joystick_2_fire_12;
    case INPUT_JOYSTICK_FIRE_13: return settings_current.joystick_2_fire_13;
    case INPUT_JOYSTICK_FIRE_14: return settings_current.joystick_2_fire_14;
    case INPUT_JOYSTICK_FIRE_15: return settings_current.joystick_2_fire_15;
    default: break;
    }
    break;

  }

  ui_error( UI_ERROR_ERROR, "get_fire_button_key: which = %d, button = %d",
	    which, button );
  fuse_abort();
}

static int
do_joystick( const input_event_joystick_t *joystick_event, int press )
{
  int which;

#ifdef USE_WIDGET
  if( ui_widget_level >= 0 ) {
    if( press ) ui_widget_keyhandler( joystick_event->button );
    return 0;
  }

#ifndef GEKKO /* Home button opens the menu on Wii */
  switch( joystick_event->button ) {
  case INPUT_JOYSTICK_FIRE_2:
    if( press ) ui_popup_menu( INPUT_KEY_F1 );
    break;

  default: break;		/* Remove gcc warning */

  }
#endif  /* #ifndef GEKKO */

#endif				/* #ifdef USE_WIDGET */

  which = joystick_event->which;

  if( joystick_event->button < INPUT_JOYSTICK_FIRE_1 ) {

    joystick_button button;

    switch( joystick_event->button ) {
    case INPUT_JOYSTICK_UP   : button = JOYSTICK_BUTTON_UP;    break;
    case INPUT_JOYSTICK_DOWN : button = JOYSTICK_BUTTON_DOWN;  break;
    case INPUT_JOYSTICK_LEFT : button = JOYSTICK_BUTTON_LEFT;  break;
    case INPUT_JOYSTICK_RIGHT: button = JOYSTICK_BUTTON_RIGHT; break;

    default:
      ui_error( UI_ERROR_ERROR, "do_joystick: unknown button %d",
		joystick_event->button );
      fuse_abort();
    }

    joystick_press( which, button, press );

  } else {

    keyboard_key_name key;

    key = get_fire_button_key( which, joystick_event->button );

    if( key == KEYBOARD_JOYSTICK_FIRE ) {
      joystick_press( which, JOYSTICK_BUTTON_FIRE, press );
    } else {
      if( press ) {
	keyboard_press( key );
      } else {
	keyboard_release( key );
      }
    }

  }

  return 0;
}
