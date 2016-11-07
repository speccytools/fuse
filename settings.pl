#!/usr/bin/perl -w

# settings.pl: generate settings.c from settings.dat
# Copyright (c) 2002-2015 Philip Kendall, Fredrick Meunier

# $Id$

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

# Author contact information:

# E-mail: philip-fuse@shadowmagic.org.uk

use strict;

use Fuse;

sub hashline ($) { '#line ', $_[0] + 1, '"', __FILE__, "\"\n" }

my %options;

my %fileAssoc = ( 'snapshot' => 1, 'tape_file' => 1, 'record_file' => 1,
                  'playback_file' => 1, 'dck_file' => 1, 'if2_file' => 1,
                  'simpleide_master_file' => 1, 'simpleide_slave_file' => 1,
                  'zxatasp_master_file' => 1, 'zxatasp_slave_file' => 1,
                  'zxcf_pri_file' => 1,
                  'plus3disk_file' => 1, 'betadisk_file' => 1 );
while(<>) {

    next if /^\s*$/;
    next if /^\s*#/;

    chomp;

    my( $name, $type, $default, $funcn, $short, $commandline, $configfile ) =
	split /\s*,\s*/;

    if( ( not defined $commandline ) || ( $commandline eq '' ) ) {
	$commandline = $name;
	$commandline =~ s/_/-/g;
    }

    if( ( not defined $configfile ) || ( $configfile eq '' ) ) {
	$configfile = $commandline;
	$configfile =~ s/-//g;
    }

    $options{$name} = { type => $type, default => $default, short => $short,
			commandline => $commandline,
			configfile => $configfile, funcn => $funcn };
}

print Fuse::GPL( 'settings.m: Handling configuration settings',
		 '2002 Philip Kendall, Fredrick Meunier' );

print << 'CODE';

/* This file is autogenerated from settings.dat by settings.pl.
   Do not edit unless you know what will happen! */

#include <config.h>

#include <stdio.h>
#include <string.h>

#import <Foundation/NSDictionary.h>
#import <Foundation/NSEnumerator.h>
#import <Foundation/NSString.h>
#import <Foundation/NSUserDefaults.h>

#import "FuseController.h"
#import "CAMachines.h"

#ifdef HAVE_GETOPT_LONG		/* Did our libc include getopt_long? */
#include <getopt.h>
#elif defined AMIGA || defined __MORPHOS__            /* #ifdef HAVE_GETOPT_LONG */
/* The platform uses GNU getopt, but not getopt_long, so we get
   symbol clashes on this platform. Just use getopt */
#else				/* #ifdef HAVE_GETOPT_LONG */
#include "compat.h"		/* If not, use ours */
#endif				/* #ifdef HAVE_GETOPT_LONG */

#include "fuse.h"
#include "infrastructure/startup_manager.h"
#include "machine.h"
#include "options.h"
#include "settings.h"
#include "settings_cocoa.h"
#include "spectrum.h"
#include "ui/ui.h"

/* The current settings of options, etc */
#include "utils.h"
settings_info settings_current;

/* The default settings of options, etc */
settings_info settings_default = {
CODE

    foreach my $name ( sort keys %options ) {
	next if $options{$name}->{type} eq 'null';
	next if $options{$name}->{type} eq 'nsarray';
        if( $options{$name}->{type} eq 'string' ) {
	  print "  /* $name */ (char *)$options{$name}->{default},\n";
 	} else {
	  print "  /* $name */ $options{$name}->{default},\n";
	}
    }
    print "  /* cocoa */ NULL,\n";

print << 'CODE';
  /* show_help */ 0,
  /* show_version */ 0,
};

static int settings_command_line( settings_info *settings, int *first_arg,
				  int argc, char **argv );

static void settings_copy_internal( settings_info *dest, settings_info *src );

/* Called on emulator startup */
int
settings_init( int *first_arg, int argc, char **argv )
{
  int error;

  error = read_config_file( &settings_current );
  if( error ) return error;

  error = settings_command_line( &settings_current, first_arg, argc, argv );
  if( error ) return error;

  return 0;
}

/* Fill the settings structure with sensible defaults */
void settings_defaults( settings_info *settings )
{
  BOOL value;

  NSMutableDictionary *defaultValues = [NSMutableDictionary dictionary];

  settings_copy_internal( settings, &settings_default );

CODE

foreach my $name ( sort keys %options ) {

    my $type = $options{$name}->{type};

    if( $type eq 'boolean' ) {
	print << "CODE";
  value = settings->$name ? YES : NO;
  [defaultValues setObject:@(value) forKey:@"$options{$name}->{configfile}"];
CODE
    } elsif( $type eq 'string' ) {
	print << "CODE";
  if( settings->$name )
    [defaultValues setObject:@(settings->$name) forKey:@"$options{$name}->{configfile}"];
  else
    [defaultValues setObject:@"" forKey:@"$options{$name}->{configfile}"];
CODE
    } elsif( $type eq 'numeric' ) {
	print << "CODE";
  [defaultValues setObject:@(settings->$name) forKey:@"$options{$name}->{configfile}"];
CODE
    } elsif( $type eq 'nsarray' ) {
	print << "CODE";
  if( settings->cocoa && settings->cocoa->$name )
    [defaultValues setObject:settings->cocoa->$name forKey:@"$options{$name}->{configfile}"];
  else
    [defaultValues setObject:[NSArray array] forKey:@"$options{$name}->{configfile}"];
CODE
    } elsif( $type eq 'nsdictionary' ) {
      # Do nothing
    } elsif( $type eq 'null' ) {
      # Do nothing
    } else {
	die "Unknown setting type `$type'";
    }
}

  print << 'CODE';

  [[NSUserDefaults standardUserDefaults] registerDefaults:defaultValues];

  [[NSUserDefaultsController sharedUserDefaultsController] setInitialValues:defaultValues];
}

/* Read options from the config file */

int
read_config_file( settings_info *settings )
{
  NSUserDefaults *defaults;
  
  defaults = [NSUserDefaults standardUserDefaults];

CODE

foreach my $name ( sort keys %options ) {

    my $type = $options{$name}->{type};
    my $funcn = $options{$name}->{funcn};

    if( $type eq 'boolean' ) {

	print << "CODE";
  settings->$name = [defaults boolForKey:@"$options{$name}->{configfile}"] ? 1 : 0;
CODE

    } elsif( $type eq 'numeric' ) {

	print << "CODE";
  settings->$name = [defaults integerForKey:@"$options{$name}->{configfile}"];
CODE

    } elsif( $type eq 'string' ) {

      if( !defined $fileAssoc{$name} ) {

	    print << "CODE";
  if( [[defaults stringForKey:@"$options{$name}->{configfile}"] isEqualToString:@""] == YES ) {
    free( settings->$name );
    settings->$name = NULL;
CODE
            if( not defined $funcn or $funcn eq '' ) {
              print << "CODE";
  } else
    settings_set_string( &settings->$name, [[defaults stringForKey:@"$options{$name}->{configfile}"] UTF8String] );
CODE
            } else {
              print << "CODE";
  } else {
    const char* value =
      $funcn(
                         [[defaults stringForKey:@"$options{$name}->{configfile}"] UTF8String] );
    settings_set_string( &settings->$name, value );
  }
CODE
            }

        }

    } elsif( $type eq 'nsarray' ) {

	print << "CODE";
  if( [defaults stringArrayForKey:@"$options{$name}->{configfile}"] != nil ) {
    NSEnumerator *enumerator = [[defaults stringArrayForKey:@"$options{$name}->{configfile}"] reverseObjectEnumerator];
    id recentFile;
            
    while ( (recentFile = [enumerator nextObject]) ) {
      [[FuseController singleton] performSelectorOnMainThread:\@selector(addRecentSnapshotWithString:)
                                  withObject:recentFile waitUntilDone:NO];
    }
  }
CODE

    } elsif( $type eq 'nsdictionary' ) {
      # Do nothing
    } elsif( $type eq 'null' ) {
      # Do nothing
    } else {
	die "Unknown setting type `$type'";
    }
}

print << 'CODE';

  return 0;
}

int
settings_write_config( settings_info *settings )
{
  NSUserDefaults *currentValues = [NSUserDefaults standardUserDefaults];
  BOOL value;

CODE

foreach my $name ( sort keys %options ) {

    my $type = $options{$name}->{type};

    if( $type eq 'boolean' ) {
	print << "CODE";
  value = settings->$name ? YES : NO;
  [currentValues setObject:@(value) forKey:@"$options{$name}->{configfile}"];
CODE
    } elsif( $type eq 'string' ) {
      if( !defined $fileAssoc{$name} ) {
          print << "CODE";
  if( settings->$name )
    [currentValues setObject:@(settings->$name) forKey:@"$options{$name}->{configfile}"];
  else
    [currentValues setObject:@"" forKey:@"$options{$name}->{configfile}"];
CODE
        }
    } elsif( $type eq 'numeric' ) {
	print << "CODE";
  [currentValues setObject:@(settings->$name) forKey:@"$options{$name}->{configfile}"];
CODE
    } elsif( $type eq 'nsarray' ) {
	print << "CODE";
  if( settings->cocoa && settings->cocoa->$name )
    [currentValues setObject:settings->cocoa->$name forKey:@"$options{$name}->{configfile}"];
  else
    [currentValues setObject:[NSMutableArray array] forKey:@"$options{$name}->{configfile}"];
CODE
    } elsif( $type eq 'nsdictionary' ) {
      # Do nothing
    } elsif( $type eq 'null' ) {
      # Do nothing
    } else {
	die "Unknown setting type `$type'";
    }
}

  print << 'CODE';

  [currentValues synchronize];

  return 0;
}

/* Read options from the command line */
static int
settings_command_line( settings_info *settings, int *first_arg,
                       int argc, char **argv )
{
#ifdef GEKKO
  /* No argv on the Wii. Just return */
  return 0;
#endif

#if !defined AMIGA && !defined __MORPHOS__

  struct option long_options[] = {

CODE

my $fake_short_option = 256;

foreach my $name ( sort keys %options ) {

    my $type = $options{$name}->{type};
    my $commandline = $options{$name}->{commandline};
    my $short = $options{$name}->{short};

    unless( $type eq 'boolean' or $short ) { $short = $fake_short_option++ }

    if( $type eq 'boolean' ) {

	print << "CODE";
    {    "$commandline", 0, &(settings->$name), 1 },
    { "no-$commandline", 0, &(settings->$name), 0 },
CODE
    } elsif( $type eq 'string' or $type eq 'numeric' ) {

	print "    { \"$commandline\", 1, NULL, $short },\n";
    } elsif( $type eq 'nsarray' ) {
	# Do nothing
    } elsif( $type eq 'nsdictionary' ) {
	# Do nothing
    } elsif( $type eq 'null' ) {
	# Do nothing
    } else {
	die "Unknown setting type `$type'";
    }
}

print << 'CODE';

    { "help", 0, NULL, 'h' },
    { "version", 0, NULL, 'V' },

    { 0, 0, 0, 0 }		/* End marker: DO NOT REMOVE */
  };

#endif      /* #ifndef AMIGA */

  while( 1 ) {

    int c;

#if defined AMIGA || defined __MORPHOS__
    c = getopt( argc, argv, "d:hm:o:p:f:r:s:t:v:g:j:V" );
#else                    /* #ifdef AMIGA */
    c = getopt_long( argc, argv, "d:hm:o:p:f:r:s:t:v:g:j:V", long_options, NULL );
#endif                   /* #ifdef AMIGA */

    if( c == -1 ) break;	/* End of option list */

    switch( c ) {

    case 0: break;	/* Used for long option returns */

CODE

$fake_short_option = 256;

foreach my $name ( sort keys %options ) {

    my $type = $options{$name}->{type};
    my $short = $options{$name}->{short};

    unless( $type eq 'boolean' or $short ) { $short = $fake_short_option++ }

    if( $type eq 'boolean' ) {
	# Do nothing
    } elsif( $type eq 'string' ) {
	print "    case $short: settings_set_string( &settings->$name, optarg ); break;\n";
    } elsif( $type eq 'numeric' ) {
	print "    case $short: settings->$name = atoi( optarg ); break;\n";
    } elsif( $type eq 'nsarray' ) {
	# Do nothing
    } elsif( $type eq 'nsdictionary' ) {
	# Do nothing
    } elsif( $type eq 'null' ) {
	# Do nothing
    } else {
	die "Unknown setting type `$type'";
    }
}

print << 'CODE';

    case 'h': settings->show_help = 1; break;
    case 'V': settings->show_version = 1; break;

    case ':':
    case '?':
      break;

    default:
      fprintf( stderr, "%s: getopt_long returned `%c'\n",
	       fuse_progname, (char)c );
      break;

    }
  }

  /* Store the location of the first non-option argument */
  *first_arg = optind;

  return 0;
}

/* Copy one settings object to another */
static void
settings_copy_internal( settings_info *dest, settings_info *src )
{
  settings_free( dest );

  dest->cocoa = calloc(sizeof(settings_cocoa), 1);

CODE

foreach my $name ( sort keys %options ) {

    my $type = $options{$name}->{type};

    if( $type eq 'boolean' or $type eq 'numeric' ) {
	print "  dest->$name = src->$name;\n";
    } elsif( $type eq 'string' ) {
	print << "CODE";
  dest->$name = NULL;
  if( src->$name ) {
    dest->$name = utils_safe_strdup( src->$name );
  }
CODE
    } elsif( $type eq 'nsarray' ) {
	print << "CODE";
  if( src->cocoa && src->cocoa->$name ) {
    dest->cocoa->$name = [NSMutableArray arrayWithArray:src->cocoa->$name];
    if( !dest->cocoa->$name ) { settings_free( dest ); }
  } else {
    dest->cocoa->$name = [NSMutableArray arrayWithCapacity:NUM_RECENT_ITEMS];
  }
CODE
    }
}

print << 'CODE';
}

int
settings_free( settings_info *settings )
{
CODE

foreach my $name ( sort keys %options ) {
    if( $options{$name}->{type} eq 'string' ) {
	print "  if( settings->$name ) {\n";
	print "    free( settings->$name );\n";
	print "    settings->$name = NULL;\n";
	print "  }\n";
    } elsif( $options{$name}->{type} eq 'nsarray' ) {
	print "  if( settings->cocoa && settings->cocoa->$name ) {\n";
	print "    [settings->cocoa->$name release];\n";
	print "    settings->cocoa->$name = nil;\n";
	print "  }\n";
    }
}

print << 'CODE';

  if( settings->cocoa ) free( settings->cocoa );
  settings->cocoa = NULL;

  return 0;
}

/* Copy one settings object to another */
void settings_copy( settings_info *dest, settings_info *src )
{
  settings_defaults( dest );
  settings_copy_internal( dest, src );
}

char **
settings_get_rom_setting( settings_info *settings, size_t which,
			  int is_peripheral )
{
  if( !is_peripheral ) {
    switch( which ) {
    case  0: return &( settings->rom_16_0     );
    case  1: return &( settings->rom_48_0     );
    case  2: return &( settings->rom_128_0    );
    case  3: return &( settings->rom_128_1    );
    case  4: return &( settings->rom_plus2_0  );
    case  5: return &( settings->rom_plus2_1  );
    case  6: return &( settings->rom_plus2a_0 );
    case  7: return &( settings->rom_plus2a_1 );
    case  8: return &( settings->rom_plus2a_2 );
    case  9: return &( settings->rom_plus2a_3 );
    case 10: return &( settings->rom_plus3_0  );
    case 11: return &( settings->rom_plus3_1  );
    case 12: return &( settings->rom_plus3_2  );
    case 13: return &( settings->rom_plus3_3  );
    case 14: return &( settings->rom_plus3e_0 );
    case 15: return &( settings->rom_plus3e_1 );
    case 16: return &( settings->rom_plus3e_2 );
    case 17: return &( settings->rom_plus3e_3 );
    case 18: return &( settings->rom_2048_0   );
    case 19: return &( settings->rom_2068_0   );
    case 20: return &( settings->rom_2068_1   );
    case 21: return &( settings->rom_ts2068_0 );
    case 22: return &( settings->rom_ts2068_1 );
    case 23: return &( settings->rom_pentagon_0 );
    case 24: return &( settings->rom_pentagon_1 );
    case 25: return &( settings->rom_pentagon_2 );
    case 26: return &( settings->rom_pentagon512_0 );
    case 27: return &( settings->rom_pentagon512_1 );
    case 28: return &( settings->rom_pentagon512_2 );
    case 29: return &( settings->rom_pentagon512_3 );
    case 30: return &( settings->rom_pentagon1024_0 );
    case 31: return &( settings->rom_pentagon1024_1 );
    case 32: return &( settings->rom_pentagon1024_2 );
    case 33: return &( settings->rom_pentagon1024_3 );
    case 34: return &( settings->rom_scorpion_0 );
    case 35: return &( settings->rom_scorpion_1 );
    case 36: return &( settings->rom_scorpion_2 );
    case 37: return &( settings->rom_scorpion_3 );
    case 38: return &( settings->rom_se_0 );
    case 39: return &( settings->rom_se_1 );
    default: return NULL;
    }
  } else {
    switch( which ) {
    case  0: return &( settings->rom_interface1 );
    case  1: return &( settings->rom_beta128 );
    case  2: return &( settings->rom_plusd );
    case  3: return &( settings->rom_didaktik80 );
    case  4: return &( settings->rom_disciple );
    case  5: return &( settings->rom_opus );
    case  6: return &( settings->rom_speccyboot );
    case  7: return &( settings->rom_usource );
    default: return NULL;
    }
  }
}

void
settings_set_string( char **string_setting, const char *value )
{
  /* No need to do anything if the two strings are in fact the
     same pointer */
  if( *string_setting == value ) return;

  if( *string_setting ) libspectrum_free( *string_setting );
  *string_setting = utils_safe_strdup( value );
}
CODE

print << 'CODE';

/* Comparison function to sort the machineroms array */
NSInteger
machineroms_compare( id dict1, id dict2, void *context )
{
  NSMutableDictionary *roms1 = (NSMutableDictionary*)dict1;
  NSMutableDictionary *roms2 = (NSMutableDictionary*)dict2;
  int index1 = machine_get_index( [[roms1 valueForKey:@"machine"] machineType] );
  int index2 = machine_get_index( [[roms2 valueForKey:@"machine"] machineType] );
  NSInteger retval = NSOrderedSame;
  if( index1 > index2 ) {
    retval = NSOrderedDescending;
  } else if( index1 < index2 ) {
    retval = NSOrderedAscending;
  }

  return retval;
}

/* Fill a NSMutableArray with rom information suitable for use with Cocoa
   bindings */
NSMutableArray*
settings_set_rom_array( settings_info *settings )
{
  NSMutableDictionary *roms;
CODE

my %machineRoms;
foreach my $name ( sort keys %options ) {
    if( $name =~ /^rom_.*_\d$/ ) {
      my ( $machine, $rom_number ) = ( $name =~ /^rom_(.*)_(\d)$/ );
      my ( $rom_name ) = ( $options{$name}->{default} =~ /strdup\( "(.*)" \)/ );
      $machineRoms{$machine}[$rom_number] = $rom_name;
    }
}

my $numMachines = keys %machineRoms;
print "  NSMutableArray *machineroms = [NSMutableArray arrayWithCapacity:machine_count+1];\n\n";

foreach my $machine ( sort keys %machineRoms ) {
  print "  roms = [NSMutableDictionary dictionaryWithObjectsAndKeys:\n";
  print '    [Machine machineForType:machine_get_type("'.$machine.'")], @"machine",'."\n";
  for( my $i = 0; $i <= $#{ $machineRoms{$machine} }; $i++ ) {
    print '    @(settings->rom_'.$machine."_".$i.'), @"rom'.$i."\",\n";
    print '    @(settings_default.rom_'.$machine."_".$i.'), @"default_rom'.$i."\",\n";
  }
  print "    NULL];\n";
  print "  [machineroms addObject:roms];\n";
  print "\n";
}

print << 'CODE';
  /* We assume that we got all machines in the array, this should always be
     true*/
  [machineroms sortUsingFunction:machineroms_compare context:nil];

  return machineroms;
}
CODE

print << 'CODE';

/* Fill the settings structure from the supplied NSMutableArray as above */
void
settings_get_rom_array( settings_info *settings, NSArray *machineroms )
{
  NSMutableDictionary *roms;

  if ( machineroms == nil ) {
    return;
  }
CODE

print "  for ( roms in machineroms ) {\n";
print "    int machineType = [[roms valueForKey:@\"machine\"] machineType];\n    ";
my $first=0;
foreach my $machine ( sort keys %machineRoms ) {
  if ( $first ) {
    print "    } else "
  } else {
    $first=1;
  }
  print 'if( machineType == machine_get_type( "'.$machine.'" ) ) {'."\n";
  for( my $i = 0; $i <= $#{ $machineRoms{$machine} }; $i++ ) {
    print '      settings_set_string( &settings->rom_'.$machine."_".$i.', [[roms valueForKey:@"rom'.$i.'"] UTF8String] );'."\n";
  }
}
print "    }\n";
print "  }\n";
print "}\n";
print "\n";

print << 'CODE';
static void
settings_end( void )
{
  /* Unset settings, tape, cartidges etc. to prevent confusion */
  if( settings_current.snapshot ) free( settings_current.snapshot );
  settings_current.snapshot = NULL;
  if( settings_current.tape_file ) free( settings_current.tape_file );
  settings_current.tape_file = NULL;
  if( settings_current.playback_file ) free( settings_current.playback_file );
  settings_current.playback_file = NULL;
  if( settings_current.plus3disk_file ) free( settings_current.plus3disk_file );
  settings_current.plus3disk_file = NULL;
  if( settings_current.betadisk_file ) free( settings_current.betadisk_file );
  settings_current.betadisk_file = NULL;
  if( settings_current.dck_file ) free( settings_current.dck_file );
  settings_current.dck_file = NULL;
  if( settings_current.if2_file ) free( settings_current.if2_file );
  settings_current.if2_file = NULL;

  if( settings_current.autosave_settings )
    settings_write_config( &settings_current );

  settings_free( &settings_current );
}

void
settings_register_startup( void )
{
  /* settings_init not yet managed by the startup manager */

  startup_manager_module dependencies[] = {
  /* Fuse for OS X requires that settings_end is called before memory is
     deallocated as settings need to look up machine names etc */
    STARTUP_MANAGER_MODULE_MEMORY,
    /* STARTUP_MANAGER_MODULE_SETUID, */
  };
  startup_manager_register( STARTUP_MANAGER_MODULE_SETTINGS_END, dependencies,
                            ARRAY_SIZE( dependencies ), NULL, NULL,
                            settings_end );
}

CODE
