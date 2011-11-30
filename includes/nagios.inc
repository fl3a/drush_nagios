<?php
/**
 * @file
 * Helper functions for nagios commands
 */

/**
* @defgroup  exit_status Exit states
* Numeric error status definitions
* Error code definitions for interpreting the current error status.
* @see drush_set_error(), drush_get_error(), drush_get_error_log(), drush_cmp_error()
* @{
*/

/** Status 0 for nagios OK @see _NAGIOS_site() and _NAGIOS_root() */
define('NAGIOS_OK', 0);

/** Status 1 for nagios warning @see _NAGIOS_site() and _NAGIOS_root() */
define('NAGIOS_WARNING', 1);

/** Status 2 for nagios critical @see _NAGIOS_site() and _NAGIOS_root() */
define('NAGIOS_CRITICAL', 2);

/** @} */ /** End of defgroup exit_status */
 
/**
 * @defgroup helper_functions Helper functions
 * Helper functions
 * @{  
 */

/**
 * Get list of installed projects and return thier update status information via drush functions
 *
 * @return $update_info
 *   An array provided by drush´s _pm_get_update_info()
 *
 * @see drush_get_extensions()
 * @see drush_get_projects()
 * @see _pm_get_update_info()
 */
function _nagios_get_update_info() {
  drush_include_engine('update_info', 'drupal', NULL, DRUSH_BASE_PATH . '/commands/pm/update_info');
  include(DRUSH_BASE_PATH . '/commands/pm/updatecode.pm.inc');

  $updatable = FALSE;
  $updates = $security_updates = $not_supported = array();

  drush_bootstrap_max();

  // Get update status information.
  ob_start();
  // Get installed extensions and projects.
  $extensions = @drush_get_extensions();
  $projects = @drush_get_projects($extensions);

  // Get update status information.
  $update_info = @_pm_get_update_info($projects);
  ob_end_clean();
  
  return $update_info;
}

/**
 * Parses supplied update info and get type of update and corresponding informations
 *
 * @param $update_info
 *   Array, update informations
 *
 * @return $updates
 * @code
 * Array
 * (
 *    [has_updates] => 1
 *    [updates] => Array
 *         (
 *             [drupal] => Array
 *                (
 *                    [project_type] => core
 *                    [existing_version] => 7.8
 *                    [latest_version] => 7.9
 *                    [release_link] => http://drupal.org/node/1322736
 *                    [serverity] => Aktualisierung verfügbar
 *                )
 * 
 *            [mimemail] => Array
 *                (
 *                    [project_type] => module
 *                    [existing_version] => 7.x-1.x-dev
 *                    [latest_version] => 7.x-1.x-dev
 *                    [release_link] => http://drupal.org/node/95867
 *                    [serverity] => Aktualisierung verfügbar
 *                )
 *
 *            [password_policy] => Array
 *                (
 *                    [project_type] => module
 *                    [existing_version] => 7.x-1.0-beta1
 *                    [latest_version] => 7.x-1.0-beta2
 *                    [release_link] => http://drupal.org/node/1331478
 *                    [serverity] => Aktualisierung verfügbar
 *                )
 * 
 *        )
 * 
 *    [security_updates] => Array
 *        (
 *            [ckeditor] => Array
 *                (
 *                    [project_type] => module
 *                    [existing_version] => 7.x-1.3
 *                    [latest_version] => 7.x-1.6
 *                    [release_link] => http://drupal.org/node/1337106
 *                    [serverity] => SECURITY UPDATE available
 *                )
 * 
 *        )
 * 
 *    [not_suported] => Array
 *        (
 *        )
 * 
 *    [ignored] => Array
 *        (
 *            [cnr] => Array
 *                (
 *                    [project_type] => module
 *                    [existing_version] => 7.x-4.1
 *                    [latest_version] => 7.x-4.22
 *                    [release_link] => http://drupal.org/node/1344032
 *                    [serverity] => Aktualisierung verfügbar
 *                )
 * 
 *            [devel] => Array
 *                (
 *                    [project_type] => module
 *                    [existing_version] => 7.x-1.0
 *                    [latest_version] => 7.x-1.2
 *                    [release_link] => http://drupal.org/node/1226936
 *                    [serverity] => SECURITY UPDATE available
 *                )
 * 
 *        )
 * 
 *)
 *@endcode 
 */
function _nagios_parse_update_info($update_info) {

  $updates = array();
  $updates['has_updates'] = FALSE;
  $updates['updates'] = array();
  $updates['security_updates'] = array();
  $updates['not_suported'] = array();
  $updates['ignored'] = array();

  $ignore = array();
  $ignore = explode(',', drush_get_option('ignore'));
  drush_print_r($ignore);
  
  foreach($update_info as $project) {

    // Severity / Type of update
    $update_type = pm_update_filter($project);

    // maschine readable name of the project
    $project_name = $project['name'];

    // Create array to append it later
    $update = array();
    $update['project_type']     =  $project['project_type'];
    $update['existing_version'] =  $project['existing_version'];
    $update['latest_version']   =  $project['latest_version'];
    $update['release_link']     =  $project['releases'][$project['latest_version']]['release_link'];
    $update['serverity']        =  dt($update_type);

    // Add project to $updates['ignored'] and skip this loop iteration
    if (in_array($project_name, $ignore)) {
      $updates['ignored'][$project_name] = $update;
      continue;
    }

    // Get type of update and append it to the corresponding array part
    switch ($update_type) {
      case dt('Update available') :
        $updates['updates'][$project_name] = $update;
        $updates['has_updates'] = TRUE;
        break;
      case dt('SECURITY UPDATE available'):
        $updates['security_updates'][$project_name] = $update;
        $updates['has_updates'] = TRUE;
        break;
      case dt('Installed version not supported'):
  	    $updates['not_supported'][$project_name] = $update;
        $updates['has_updates'] = TRUE;
        break;
    }        
  }
  return $updates;
}

/** @}*/

/** Nagios functions
 * Functions that deals with Nagios
 * @see @ref nagios_help
 * @{  
 */

/**
 * Monitores a single Drupal site for module and core updates  
 *
 * Print a message and returs status_code 
 *
 * @todo Monitor for pending database updates
 *  
 * @return $status_code
 * NAGIOS_OK (0) if monitored site is up to date
 *
 * NAGIOS_WARNING (1) if there are updates or unsupported modules available
 *
 * NAGIOS_CRITICAL(2) if there are security updates available
 */
function _nagios_site() {
  
  $update_info = _nagios_get_update_info();
  $updates = _nagios_parse_update_info($update_info);
 
  // debug
  drush_print_r($updates);
  drush_print_r(array_keys($updates['updates']));
  drush_print(implode(',',array_keys($updates['updates'])));

  die();

  // Get uri for Available updates
  switch (drush_drupal_major_version()) {
    case 5:
      $updates_path = '/?q=admin/logs/updates';
      break;
    case 6:
    case 7:
      $updates_path = '/?q=admin/reports/updates';
      break;
  }
  
  // Build message and status
  if($updatable) {
    // CRITICAL / SECURITY UPDATES
    if (!empty($security_updates)) {
      $status_msg = dt('DRUPAL SITE CRITICAL - !uri @ !site_root: ', 
        array(
        '!uri'      => drush_get_context('DRUSH_URI'),
        '!site_root'=> drush_get_context('DRUSH_DRUPAL_ROOT') . '/' . drush_get_context('DRUSH_DRUPAL_SITE_ROOT'),            
        )
      );
      $status_msg .= dt('SECURITY UPDATES available for !modules', 
        array(
          '!modules' => implode(',', $security_updates),
        )
      );
      if (!empty($updates)) {
        $status_msg .= dt(', UPDATES available for !modules', 
          array(
          '!modules' => implode(',', $updates)
          )
        );          
      }
      if (!empty($not_supported)) {
        $status_msg .= dt(', Installed version not supported: !modules',
          array(
            '!module' => implode(',', $not_supported)
          )
        );  
      }
      $status_msg .= dt(', see http://!update_uri for details.', 
        array(
          '!update_uri' => drush_get_context('DRUSH_URI') . $updates_path          
        )
      );
      $status_code = NAGIOS_CRITICAL;
    }
    // WARNING / Updates or Unsupported modules
    else if (!empty($updatable) || !empty($not_supported)) {
      $status_msg = dt('DRUPAL SITE WARNING - !uri @ !site_root: ', 
        array(
        '!uri'      => drush_get_context('DRUSH_URI'),
        '!site_root'=> drush_get_context('DRUSH_DRUPAL_ROOT') . '/' . drush_get_context('DRUSH_DRUPAL_SITE_ROOT'),            
        )
      );
      if (!empty($updates)) {
        $status_msg .= dt('Updates available for !modules', 
          array(
          '!modules' => implode(',', $updates)
          )
        );          
      }
      if (!empty($not_supported)) {
        if (!empty($updates)) {
          $status_msg .= ', ';
        }
        $status_msg .= dt('Installed version not supported: !modules',
          array(
            '!module' => implode(',', $not_supported)
          )
        );  
      }
      $status_msg .= dt(', see http://!update_uri for details.', 
        array(
          '!update_uri' => drush_get_context('DRUSH_URI') . $updates_path          
        )
      );
      $status_code = NAGIOS_WARNING;
    }
  }
  else {  
    // OK / No updates
    $status_msg =  dt('DRUPAL SITE OK - !uri @ !site_root: Everything is uptodate.',
      array(
        '!uri'      => drush_get_context('DRUSH_URI'),
        '!site_root'=> drush_get_context('DRUSH_DRUPAL_ROOT') . '/' . drush_get_context('DRUSH_DRUPAL_SITE_ROOT'),
      )
    );
    $status_code = NAGIOS_OK;
  }  
    
  // Print message and exit with status
  drush_print($status_msg);  
  return $status_code;  
}
/** @} */ /** End of defgroup nagios */
 
/**
 * @defgroup filesystem Filesystem functions
 * Functions that do search and scan operation on the local file system.
 * @{  
 */

/** @} */ /** End of defgroup helper */