
 * Introduction
 * Requirements
 * Installation
 * Documentation

 

INTRODUCTION
------------

Current Maintainer: Florian "fl3a" Latzel <http://drupal.org/user/51103> 
Original Sponsor: ISL Individuelle System Lösungen <http://is-loesungen.de>


REQUIREMENTS
------------

Since this is a drush command, all you need is drush.


INSTALLATION
------------

You can put the multi in a number of places:

EXPERPT from a forgotten README.txt:
[...]
You can put your drush command file in a number of places:
  - In a .drush folder in your HOME folder.
  - Along with one of your existing modules. If your command is
    related to an existing module, this is the preferred option.
  - In a folder specified with the include option.
  - In /path/to/drush/commands (not a "Smart Thing to Do", but it would work).
 [...] 


DOCUMENTATION
-------------

Each multi command got a help.
  
To get specific help, type:
drush help nagios

Since the the code is extensively commented  
you can find the a Doxygen documentation within the interweb at 
http://is-loesungen.de/docu/drush_nagios/.
