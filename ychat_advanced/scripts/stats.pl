#!/usr/bin/perl

# The yChat Project (2003)
#
# This script generates source code and project statistics 

use strict;

use scripts::modules::file;

my %stats;

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
   $stats{numfilestotal}++;
   &filestats("$shift/$_");
  }
  elsif ( -d "$shift/$_" )
  {
   $stats{numdirstotal}++;
   &recursive("$shift/$_");
  }
 }
}

sub filestats
{
 my $shift = shift;
 if ( $shift =~ /\.(cpp|h)$/ )
 {
  $stats{numsrcfiles}++;
  $stats{linesofsrc} += countlines($shift);
 }
 elsif ( $shift =~ /\.(html|css)$/ )
 {
  $stats{numhtmlfiles}++;
  $stats{linesofhtml} += countlines($shift);
 }
 elsif ( $shift =~ /\.(gif|png|jpg)$/ )
 {
  $stats{numgfxfiles}++;
 }
 elsif ( $shift =~ /(\.pl|\.sh|configure.*|Makefile.*)$/ )
 {
  $stats{numscriptfiles}++;
  $stats{linesofscripts} += countlines($shift);
 }
 elsif ( $shift =~ /(\.txt|README|INSTALL|COPYING|ChangeLog)$/ )
 {
  $stats{numtxtfiles}++;
  $stats{linesoftxt} += countlines($shift);
 }
 elsif ( $shift =~ /\.so$/ )
 {
  $stats{nummodulefiles}++;
 }
}

sub countlines
{
 my $shift = shift;
 my @file = fopen($shift);
 return $#file; 
}
