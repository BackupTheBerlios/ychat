#!/usr/bin/perl

# The yChat Project (2003)
#
# This script generates source code and project statistics 

use strict;

my %stats = ( 	dirs => 0,
		files => 0,
		srcfiles => 0,
                scriptfiles => 0,
                htmlfiles => 0,
                txtfiles => 0,
		modulefiles => 0,
		htmlfiles => 0,
		linesofsrc => 0,
		linesofscripts => 0,
                linesoftxt => 0,
		linesofhtml => 0 );

&recursive('.');

$stats{linestotal} = $stats{linesofsrc} 
                   + $stats{linesofscripts}   
                   + $stats{linesoftxt}   
                   + $stats{linesofhtml};
   
foreach ( sort keys %stats )
{
 print "$_ = " . $stats{$_} . "\n";
}


sub recursive
{
 my $shift = shift;
 my @dir = &dopen($shift);
 
 foreach (@dir)
 {
  next if /^\.$/ or /^\.{2}$/;

  if ( -f "$shift/$_" )
  {
   $stats{files}++;
   &filestats("$shift/$_");
  }
  elsif ( -d "$shift/$_" )
  {
   $stats{dirs}++;
   &recursive("$shift/$_");
  }
 }
}

sub filestats
{
 my $shift = shift;
 if ( $shift =~ /\.(cpp|h)$/ )
 {
  $stats{srcfiles}++;
  $stats{linesofsrc} += countlines($shift);
 }
 elsif ( $shift =~ /\.(html|css)$/ )
 {
  $stats{htmlfiles}++;
  $stats{linesofhtml} += countlines($shift);
 }
 elsif ( $shift =~ /(\.pl|\.sh|configure.*|Makefile.*)$/ )
 {
  $stats{scriptfiles}++;
  $stats{linesofscripts} += countlines($shift);
 }
 elsif ( $shift =~ /(\.txt|README|INSTALL|COPYING|ChangeLog)$/ )
 {
  $stats{txtfiles}++;
  $stats{linesoftxt} += countlines($shift);
 }
 elsif ( $shift =~ /\.so$/ )
 {
  $stats{modulefiles}++;
 }
}

sub countlines
{
 my $shift = shift;
 my @file = fopen($shift);
 return $#file; 
}

sub dopen
{
 my $shift = shift;
 opendir DIR, $shift or die "$!: $shift\n";
 my @dir = readdir(DIR);
 closedir DIR;
 return @dir;
}

sub fopen
{
 my $shift = shift;
 open FILE, $shift or die "$!: $shift\n";
 my @file = <FILE>; 
 close FILE;
 return @file;
}
