<?php

include_once 'includes/nagios.inc';

/**
 * @file
 * The nagios implementation.   
 */

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
      return 'For the usage as nagios plugin: Monitores a Drupal site and print a message and return with an exit status.

Checks on the the with -l / --uri supplied site for core and module updates.

If the Drupal site is not up to date, this command will print the type of the update(Security update, update, unsupported release) including the corresponding modules, drupal core and a link to the \'Available updates\' page within drupal.


Exit status

* NAGIOS_OK (0) if monitored Drupal site is up to date
* NAGIOS_WARNING (1) if there is a \'Bug fix\'/Update available
* NAGIOS_CRITICAL(2) if there is a \'Security update\' available';      
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

  $items['nagios'] = array(
    'callback' => 'drush_nagios',
    'description' => 'For the usage as nagios plugin: Monitores a Drupal site and print a message and return with an exit status.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_SITE,
    'examples' => array(
      'drush -r /path/to/drupal nagios' =>
      "We assume that drupal-6.14 is the recommended release. (6.14 was a security update).\n" . 
      "With an older version, eg. drupal-6.13 we get:\n" .
      "'DRUPAL ROOT CRITICAL: drupal-6.13 @ /path/to/drupal/6.x\n" .
      "- drupal-6.14 (Security update) from 2009-09-16 21:40 available, see http://drupal.org/node/579476 for details.'\n" .
      "with exit status 2\n" .
      "Or if we are running the recommended version:\n" .
      "' DRUPAL ROOT OK: drupal-6.14 @ /path/to/drupal/6.x is up to date.'\n" .
      "with exit status 0.",
      'drush -r /path/to/drupal nagios --file=/path/to/file' => 'Same as above with a specified file.',
      'drush -r /path/to/drupal -l example.com nagios' => 
      "Assuming we have got security updates, updates and a unsupported release of a module,\n" .
      "it will print the following output and exit with NAGIOS_CRITICAL (2) because of the security update:\n" .
      "DRUPAL SITE CRITICAL - example.com @ /path/to/drupal/6.x/sites/example.com:\n" .
      "SECURITY UPDATES available for image,lightbox2, Updates available for drupal,bueditor," .
      "imce, Installed version not supported: transliterations, see http://example.com/?q=admin/reports/updates for datails.\n" .
      "\nIf there are no updates available, this will produce to following output:\n" .
      "DRUPAL SITE OK - example.com @ /path/to/drupal/6.x/sites/example.com: Everything is up to date."
     ), 
    'options' => array(
      '--changelog' => 'Show release link / link to changelog for the recommended version of a affected projects.',
      '--show-versions' => 'Show used / current and recommended versions for a affected project..',
      '--ignore' => 'Comma seperated list of projects which should be ignored, useful for patched, modified modules etc.',
      /** @todo '--db-updates' => 'Show pending database updates.', */
    ),    
  );
  return $items;
}
/** @} */ /** End of defgroup drush_hooks */

/**
 * @defgroup drush_callbacks Drush callbacks
 * Implementation of general drush callbacks
 * @{  
 */

/**
 * Command callback
 *
 * @return int 
 *   Exit status of the referenced function
 */
function drush_nagios() {
  $update_info = _nagios_get_update_info();
  _nagios_print_message($update_info);
  return _nagios_exit_status($update_status);    
}

/** @} */ /** End of defgroup callbacks */

