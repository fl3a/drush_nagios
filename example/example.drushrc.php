<?php

/**
 * @file
 * Drush runtime config with command specific options
 * for drush_nagios (https://www.drupal.org/project/drush_nagios)
 * used on the monitored site or Drupal installation.
 *
 * This configuration overwrites previous settings
 * made in example.aliases.drushrc.php.
 *
 * Rename this file to drushrc.php and optionally copy it to one of the places
 * listed below in order of precedence:
 *
 * 1.  Drupal site folder (e.g. sites/{default|example.com}/drushrc.php).
 * 2.  Drupal /drush and sites/all/drush folders.
 *
 * @see http://api.drush.org/api/drush/examples%21example.drushrc.php/5.x
 * for examples of valid statements for a Drush runtime config (drushrc) file.
 */

/**
 * @todo Implement drush_nagios_monitored_server variable
 * for customizing output if available.
 */

/**
 * FQDN Host name of the server where the monitored site lives,
 * used within message output if available.
 */
// $options['variables']['drush_nagios_monitored_server'] = 'server.example.com';

/**
 * Command specific options for check-updates command.
 * - ignore
 *  - Comma seperated list of projects which should be ignored,
 * useful for patched, modified modules etc.
 * - ignore-locked
 *  - Ignore projects which are locked by `drush pm-update --lock`
 * (1, TRUE = on/0, FALSE = off).
 */
$command_specific['check-updates'] = array(
  'ignore' => 'project1,project2,projectN',
  'ignore-locked' => 1,
);

/**
 * Command specific options for check-drupal-requirements command.
 * - ignore
 *  - Facility or comma seperated list of facilities which should be ignored,
 */
$command_specific['check-drupal-requirements'] = array(
  'ignore' => 'facility1,facility2,facilityN',
);

/**
 * Command specific options for check-dbupdates command.
 * - severity
 *  - Severity if there are pending database updates
 * (1 = Warning (default), 2 = Critical).
 */
/*
$command_specific['check-db-updates'] = array(
  'severity' => '2',
);
*/