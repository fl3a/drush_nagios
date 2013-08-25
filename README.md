# Drush Nagios (drush_nagios)

## Introduction

Isolated Nagios/Icinga functionality from [drush_multi][1]

For the usage as [Nagios][2]/[Icinga][3] [NRPE Plugin][4].

It just prints a message and exit with an exit status.

## Commands

drush_nagios includes the following command:

## check-updates

Checks for pending updates.

## check-db-updates

Checks for pending database updates.

## Installation

### Manual method

Download and extract this extension to a number of places:

- In a .drush folder in your HOME folder.
- Along with one of your existing modules. If your command is related to an existing module, this is the preferred option.
- In a folder specified with the include option.
- In /path/to/drush/commands (not a "Smart Thing to Do", but it would work).

### Drush method 

Just install it via drush, type:
	
	drush dl drush_nagios

## Documentation

Every command got a help.
Type `drush help COMMAND` for options and further informations.

See also [example.aliases.drushrc.php][5].

## Credits

Thanks to the mighty hundfred, friend and Bastard Operator from Hell,
who inspired me to write this (Nagios) Drush plugin.

This module was sponsored and written by [ISL Florian Latzel][5].

[1]:http://drupal.org/project/drush_multi
[2]:http://en.wikipedia.org/wiki/Nagios
[3]:http://en.wikipedia.org/wiki/Icinga
[4]:http://en.wikipedia.org/wiki/Nagios#NRPE
[5]:https://github.com/fl3a/drush_nagios/blob/master/example/example.aliases.drushrc.php
[6]:http://is-loesungen.de
