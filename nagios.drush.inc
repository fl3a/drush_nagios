<?php

include_once 'includes/nagios.inc';

/**
 * @file
 * The multi implementation.   
 */

/**
 * Implementation of hook_drush_init()
 * 
 * Check for needed drush_version.
 */
function nagios_drush_init() {
  $drush_version = array();
  $drush_version = explode('-', DRUSH_VERSION);
    
  if ($drush_version[0] < 4.0) {
    $drush_info = drush_read_drush_info();
    if ($drush_version['datestamp'] < 1293480992) {
      drush_log('Some command won`t work properly. You need a drush v. >=4.0[-dev -rc*] with a builddate newer or equal 27. Dec 2010 20:16:32 (1293480992).', 'warning');      
    }
  }  
}

/**
 * @defgroup drush_hooks Drush hooks
 * Implementation of general drush hooks
 * @{  
 */

/**
 * Implementation of hook_drush_help().
 * @param
 *   A string with the help section (prepend with 'drush:')
 * @return
 *   A string with the help text for your command.
 */
function nagios_drush_help($section) {
  switch ($section) {
    case 'drush:nagios':
      return 'For the usage as nagios plugin: Monitores a Drupal installation or a Drupal site and print a message and return with an exit status.

Monitoring of a Drupal installation
multi_nagios could be run on the with -r /--root supplied Drupal installation to check for core updates.

If the Drupal installation is not up to date, this command prints the recommended version, the type of the update (Bug fixes or Security update), the release date and the link to the release note of the recommended version.

Exit status

MULTI_NAGIOS_OK (0) if monitored Drupal is up to date
or MULTI_NAGIOS_WARNING (1) if there is a \'Bug fixes\' available
or MULTI_NAGIOS_CRITICAL(2) if there is a \'Security update\' available

Monitoring of a Drupal site
Checks on the the with -l / --uri supplied site for core and module updates.

If the Drupal site is not up to date, this command will print the type of the update(Security update, update, unsupported release) including the corresponding modules, drupal core and a link to the \'Available updates\' page within drupal.

Exit status

MULTI_NAGIOS_OK (0) if monitored site is up to date
or MULTI_NAGIOS_WARNING (1) if there are updates or unsupported modules available
or MULTI_NAGIOS_CRITICAL(2) if there are Security updates available';      
  }
}

/**
 * Implementation of hook_drush_command().
 * @See drush_parse_command() for a list of recognized keys.
 * @return
 *   An associative array describing your command(s).
 */
function nagios_drush_command() {
  $items = array();

  $items['nagios-core'] = array(
  );
  $items['nagios-modules'] = array(

  );
  $items['nagios-db'] = array(
 
  );

  // old
  $items['nagios'] = array(
    'callback' => 'drush_nagios',
    'aliases' => array('mna', 'nagios'), 
    'description' => 'For the usage as nagios plugin: Monitores a Drupal installation or a Drupal site and print a message and return with an exit status.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_ROOT,
    'examples' => array(
      'drush -r /path/to/drupal multi-nagios' =>
      "We assume that drupal-6.14 is the recommended release. (6.14 was a security update).\n" . 
      "With an older version, eg. drupal-6.13 we get:\n" .
      "'DRUPAL ROOT CRITICAL: drupal-6.13 @ /path/to/drupal/6.x\n" .
      "- drupal-6.14 (Security update) from 2009-09-16 21:40 available, see http://drupal.org/node/579476 for details.'\n" .
      "with exit status 2\n" .
      "Or if we are running the recommended version:\n" .
      "' DRUPAL ROOT OK: drupal-6.14 @ /path/to/drupal/6.x is up to date.'\n" .
      "with exit status 0.",
      'drush -r /path/to/drupal multi-nagios --file=/path/to/file' => 'Same as above with a specified file.',
      'drush -r /path/to/drupal -l example.com multi-nagios' => 
      "Assuming we have got security updates, updates and a unsupported release of a module,\n" .
      "it will print the following output and exit with MULTI_NAGIOS_CRITICAL (2) because of the security update:\n" .
      "DRUPAL SITE CRITICAL - example.com @ /path/to/drupal/6.x/sites/example.com:\n" .
      "SECURITY UPDATES available for image,lightbox2, Updates available for drupal,bueditor," .
      "imce, Installed version not supported: transliterations, see http://example.com/?q=admin/reports/updates for datails.\n" .
      "\nIf there are no updates available, this will produce to following output:\n" .
      "DRUPAL SITE OK - example.com @ /path/to/drupal/6.x/sites/example.com: Everything is up to date."
     ), 
    'options' => array(
      '--file' => "Path to release information file, this could be a (temp) file  to avoid nrpe-socket timeouts.\n" .
      "It is also possible to supply path via URI, like http://updates.drupal.org/release-history/drupal/6.x.\n" .
      "NOTE: This option is only available for checking the Drupal installation."
    ),    
  );
  return $items;
}

/**
 * Command callback for @ref multi_nagios_help
 * 
 * Calls _multi_nagios_site() if a valid site was supplied 
 * or _multi_nagios_root() if only the installation was supplied
 * 
 * @return 
 * Exit status of the referenced function
 * 
 * @see _multi_nagios_root()
 * @see _multi_nagios_site() *  
 */
function drush_nagios() {
 
  // get -l/--uri option
  $uri = drush_get_option(array('uri','l'));  
    
  if (!empty($uri)) {
    return _multi_nagios_site();    
  }
  else {
    // multi nagios was called with -r/--root only
    return _nagios_root();
  }
}

/** @} */ /** End of defgroup callbacks */

/**
 * @defgroup commands
 * Commands as defined in nagios_drush_command()
 * @{  
 */

 
/** @} */ /** End of defgroup commands */
