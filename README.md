# Drush Nagios (drush_nagios)

## Introduction

Isolated Nagios/Icinga functionality from [drush_multi][1],
for the usage as [Nagios][2]/i[Icinga][3] [NRPE Plugin][4] via [Drush][5], it just prints a message and exit with an exit status.

You can find the original [project page][6] at http://drupal.org/project/drush_nagios .

## Commands

drush_nagios includes the following commands:

## check-updates

Checks for pending updates including Drupal (core), modules and themes grouped by type/serverity of update followed by the corresonding modules.

This command has 2 option to ignore projects.
- Via `--ignore option`, followed by project or comma seperated list of projects
- Or via `--ignore-locked` option, which ignores project that are locked via `drush pm-update --lock`

In both cases the ignored projects does not affect the exit status but they got listed with their severity.

## check-db-updates

Checks for pending database updates.

## check-drupal

Anything at Drupals status report (Administer -> Reports -> Status report)
for met requirements and their serverity, such as:
- module updates
- last run of cron
- database schema
- files directory writability
- access to update.php
- files directory protection via .htaccess
- every project that implements `hook_requirements()`

Like in check-updates, this command has a `--ignore` option for ignoring requirements/facilities too with the same behaviour as above.

## Requirements

Since this is a Drush extension, you will need Drush >=v.5.

## Installation

### Manual method

Download from [project page][6] and extract this extension to a number of places:

- In a .drush folder in your HOME folder.
- Along with one of your existing modules. If your command is related to an existing module, this is the preferred option.
- In a folder specified with the include option.
- In /path/to/drush/commands (not a "Smart Thing to Do", but it would work).

### Drush method 

Since this is as drupal.org project, you might install it via drush, just type:
	
    drush dl drush_nagios

## Documentation

Every command got a help.
Type `drush help COMMAND` for options, examples and further informations.

- See [example.aliases.drushrc.php][7] for Drush alias configuration
- See [Drush Nagios (drush_nagios) Doxygen Dokumentation][8]

## Credits

Thanks to the mighty hundfred, friend and Bastard Operator from Hell,
who inspired me to write this (Nagios) Drush plugin.

This module was and written by [fl3a][9], [Florian Latzel][10], 
  
[Drupal-CMS-[Consulting  Architecture Development] and Open Source support in Cologne][9],      

proud member of [Reinblau, Drupal Agency in Berlin, Cologne and Munich][11].   

[1]:http://drupal.org/project/drush_multi
[2]:http://en.wikipedia.org/wiki/Nagios
[3]:http://en.wikipedia.org/wiki/Icinga
[4]:http://en.wikipedia.org/wiki/Nagios#NRPE
[5]:http://drush.ws
[6]:http://drupal.org/project/drush_nagios
[7]:http://cgit.drupalcode.org/drush_nagios/tree/example/example.aliases.drushrc.php
[8]:http://is-loesungen.de/docu/drush_nagios/index.html
[9]:https://drupal.org/user/51103
[10]:http://is-loesungen.de
[11]:http://reinblau.de
