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
      //'changelog' => 'Show release link / link to changelog for the recommended version of a affected projects.',
      //'show-versions' => 'Show used / current and recommended versions for a affected project..',
      'ignore' => array(
        'description' => 'Comma seperated list of projects which should be ignored, useful for patched, modified modules etc.',
        'value' => 'required',
        'example-value' => 'project1,...,projectN',
      ),
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
 * Print out nag
 *
 * @return int 
 *   Exit status of the referenced functionn
 */
function drush_nagios() {
  drush_print(drush_drupal_major_version());
  if (drush_get_option('db-updates') ==  TRUE) {
    drush_print('db-updates Option set');
    drush_include_engine('drupal', 'update', drush_drupal_major_version());
    if (drush_drupal_major_version() == 7) {
       update_main_prepare();
       $pending = update_get_update_list();
       if (count($pending) != 0) {
         drush_print('HAS DB-UPDATES');
         die();
       }
       else {
         drush_print('HAS NO DB-UPDATES');
         die();
       }
    }
    else if(drush_drupal_major_version() == 6) {
      update_main_prepare();

      $start = array();
      $has_updates = FALSE;
      $modules = drupal_get_installed_schema_version(NULL, FALSE, TRUE);
      foreach ($modules as $module => $schema_version) {
        $updates = drupal_get_schema_versions($module);
        // Skip incompatible module updates completely, otherwise test schema versions.
        if (!update_check_incompatibility($module) && $updates !== FALSE && $schema_version >= 0) {
          // module_invoke returns NULL for nonexisting hooks, so if no updates
          // are removed, it will == 0.
          $last_removed = module_invoke($module, 'update_last_removed');
          if ($schema_version < $last_removed) {
            drush_set_error('PROVISION_DRUPAL_UPDATE_FAILED', dt( $module . ' module can not be updated. Its schema version is ' . $schema_version . '. Updates up to and including ' . $last_removed . ' have been removed in this release. In order to update ' . $module . ' module, you will first <a href="http://drupal.org/upgrade">need to upgrade</a> to the last version in which these updates were available.'));
            continue;
          }

          $updates = drupal_map_assoc($updates);
          foreach (array_keys($updates) as $update) {
            if ($update > $schema_version) {
              $start[$module] = $update;
              break;
            }
          }

          // Record any pending updates. Used for confirmation prompt.
          foreach (array_keys($updates) as $update) {
            if ($update > $schema_version) {
              if (class_exists('ReflectionFunction')) {
                // The description for an update comes from its Doxygen.
                $func = new ReflectionFunction($module . '_update_' . $update);
                $description = str_replace(array("\n", '*', '/'), '', $func->getDocComment());
              }
              if (empty($description)) {
                $description = dt('description not available');
              }

              $pending[$module][] = array("$update - " . trim($description));
              $has_updates = TRUE;
            }
          }  
        }  
      }

      if ($has_updates) {        
        drush_print('HAS DB-UDATES');
        die();
      }
      else {
        drush_print('HAS NO DB-UDATES');
        die();
      }
    }
  }
  $update_info = _nagios_get_update_info();
  $parsed_update_info = _nagios_parse_update_info($update_info);
  _nagios_print_message($parsed_update_info);
  return _nagios_exit_status($parsed_update_info);    
}

/** @} */ /** End of defgroup callbacks */

