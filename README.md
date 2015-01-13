# Drush Nagios (drush_nagios)

## Introduction

Isolated Nagios/Icinga functionality from [drush_multi][1],
for [Nagios][2]/[Icinga][3] monitoring via [NRPE Plugin][4] or ssh as [Drush][5] plugin. 

It just prints a message and exit with an exit status (severity).

You can find the original [project page][6] at http://drupal.org/project/drush_nagios .

## Commands

drush_nagios includes the following commands:

## check-updates

Checks for pending updates including Drupal (core), modules and themes grouped by type/serverity of update followed by the corresonding modules.

This command has 2 option to ignore projects.
- Via `--ignore` option, followed by project or comma seperated list of projects
- Or via `--ignore-locked` option, which ignores project that are locked via `drush pm-update --lock`

In both cases the ignored projects does not affect the exit status but they got listed with their severity.

## check-db-updates

Checks for pending database updates.

## check-drupal-requirements

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

Ignored requirements does not affect the exit status but they got listed with their severity.

## Requirements

Since this is a Drush extension, you will need Drush greater or equal Drush version 5.

## Installation

### Manual method

Download from [project page][6] and extract this extension to a number of places:

- In a .drush folder in your HOME folder.
- Along with one of your existing modules. If your command is related to an existing module, this is the preferred option.
- In a folder specified with the include option.
- In /path/to/drush/commands (not a "Smart Thing to Do", but it would work).

### Drush method 

Since this is as drupal.org project, you might install it via drush, just type:
	
    drush @none dl drush_nagios

## Configuration

- See example.aliases.drushrc.php at examples directory for drush_nagios specific settings within an alias file.
- See example.drushrc.php at examples directory for drush_nagios specific settings within a Drush runtime config (drushrc) file.

## Documentation

Every command got a help, for options, examples and further informations just type:

    drush help COMMAND

- See [example.aliases.drushrc.php][7] for example of valid statements for an alias file.
- See [example.drushrc.php][8] for examples of valid statements for a Drush runtime config (drushrc) file.
- See [Drush Nagios (drush_nagios) Doxygen Dokumentation][9]

## Maintainers

- Florian Latzel (fl3a) - https://drupal.org/user/51103

## Credits

Thanks to the mighty hundfred, friend and Bastard Operator from Hell,
who inspired me to write this (Nagios) Drush plugin.

[1]:http://drupal.org/project/drush_multi
[2]:http://en.wikipedia.org/wiki/Nagios
[3]:http://en.wikipedia.org/wiki/Icinga
[4]:http://en.wikipedia.org/wiki/Nagios#NRPE
[5]:http://drush.ws
[6]:http://drupal.org/project/drush_nagios
[7]:http://api.drush.org/api/drush/examples%21example.aliases.drushrc.php/5.x
[8]:http://api.drush.org/api/drush/examples%21example.drushrc.php/5.x
[9]:http://is-loesungen.de/docu/drush_nagios/index.html