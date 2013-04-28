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

  drush_bootstrap_max();  
  // Get installed extensions and projects.
  $extensions = drush_get_extensions();
  $projects = drush_get_projects($extensions);

  // Set quiet due getting update status information produces unwanted output.
  drush_set_option('quiet', 1, 'all');
  drush_set_context('DRUSH_QUIET', 1);

  // Get update status information.
  $update_info = _pm_get_update_info($projects);
  
  // Unset quiet.
  drush_set_option('quiet', 0, 'all');
  drush_set_context('DRUSH_QUIET', 0);
  
  return $update_info;  
}

/**
 * Parses supplied update info and get type of update and corresponding informations
 *
 * @param array $update_info
 *   Update information
 *
 * @return array
 *   An array with an update indicator 'has_updates' (boolean) 
 *   and keyed on the following serverities which contain the corresponding projects
 *   - updates
 *   - security_updates
 *   - not_supported
 *   - ignored (ignore Option)
 *   - update_not_checked
 *   - update_revoked
 *   - up_to_date
 *   - unknown 
 *
 *  Projects array keys
 *  - project_type
 *  - existing_version
 *  - latest_version
 *  - release_link
 *  - serverity
 *
 * Example called with --ignore=cnr,devel :
 *
 * @code
 * Array
 * (
 *    [has_updates] => 1
 *    [updates] => Array
 *         (
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
 *    [not_supported] => Array
 *        (
 *        )
 *
 *    [ignored] => Array
 *        (
 *        )
 *
 *    [update_not_checked] => Array
 *        (
 *        )
 *
 *    [update_revoked] => Array
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
 *    [up_to_date] => Array
 *        (
 *             [drupal] => Array
 *                (
 *                    [project_type] => core
 *                    [existing_version] => 7.9
 *                    [latest_version] => 7.9
 *                    [release_link] => http://drupal.org/node/1322736
 *                    [serverity] => Aktualisierung verfügbar
 *                )
 * 
 *        )
 *
 *)
 *@endcode 
 */
function _nagios_parse_update_info($update_info) {

  $updates = array();
  
  // Update indicator 
  $updates['has_updates'] = FALSE;
  
  // Serverities  
  $updates['updates'] = array();
  $updates['security_updates'] = array();
  $updates['not_supported'] = array();
  $updates['ignored'] = array();  
  $updates['update_not_checked'] = array();
  $updates['update_revoked'] = array();
  $updates['up_to_date'] = array();
  $updates['unknown'] = array();  
  
  $ignore = array();
  $ignore = explode(',', drush_get_option('ignore'));
  
  foreach($update_info as $project) {

    // Severity / Type of update
    ob_start();    
    $update_type = pm_update_filter($project);
    ob_end_clean();

    // maschine readable name of the project
    $project_name = $project['name'];

    // Create array to append it later
    $update = array();
    $update['project_type'] =  $project['project_type'];
    $update['existing_version'] =  $project['existing_version'];
    
    // Skip building array on these serverities...
    if ($update_type != dt('Unknown') || $update_type != dt('Unable to check status')) {
      $update['latest_version'] =  $project['latest_version'];
      $update['release_link'] =  $project['releases'][$project['latest_version']]['release_link'];
      $update['serverity'] =  dt($update_type);
    }

    // Add project to $updates['ignored'] and skip this loop iteration
    if (in_array($project_name, $ignore)) {
      $updates['ignored'][$project_name] = $update;
      continue;
    }

    // Get type of update and append it to the array
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
      case dt('Unable to check status'):
        $updates['update_not_checked'][$project_name] = $update;
        break;
      case dt('Installed version REVOKED'):
        $updates['update_revorked'][$project_name] = $update;
        $updates['has_updates'] = TRUE;        
        break;  
      case dt('Up to date'):
        $updates['up_to_date'][$project_name] = $update;
        break; 
      default:
        $updates['unknown'][$project_name] = $update;
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
 * Print a message for nagios 
 */
function _nagios_print_message($parsed_update_info) {
  
  // Message prefix
  $status_msg = dt('!site_root: ',
    array(
      '!site_root'=> drush_get_context('DRUSH_DRUPAL_ROOT') . '/' . drush_get_context('DRUSH_DRUPAL_SITE_ROOT'),
    )
  );
  
  // Build updates message
  if ($parsed_update_info['has_updates']) {
   
    unset($parsed_update_info['has_updates']);    
    
     // Traverse serverities
    foreach ($parsed_update_info as $serverity => $projects){
      
      // Skip empty and uneeded serverities
      if (count($projects) == 0 || $serverity == 'up_to_date' || $serverity == 'unknown') {
        continue;
      }
      $ignored = array();
      if ($serverity == 'ignored') {
        foreach ($projects as $name => $project) {
          $ignored[] = $name . '(' . $project['serverity'] . ')';
        }
        $status_msg .= dt(' IGNORED BY SETTINGS: !modules',
          array(
            '!modules' => implode(',', $ignored),
          )
        );        
      }
      else {
        $status_msg .= dt(' !type: !modules',
          array(
            '!type' => strtoupper(str_replace('_', ' ', $serverity)),
            '!modules' => implode(',', array_keys($projects)),
          )
        );     
      }
    }
  }
  else {
    $status_msg = $status_msg . dt('Everythings OK...');
  }
 
  // Print message
  drush_print($status_msg);  
}


/**
 * Return exit status for nagios
 *
 * @param array $parsed_update_info 
 * 
 * @return int  
 *   - NAGIOS_OK (0) if monitored site is up to date, 
 *   - NAGIOS_WARNING (1) if there are updates or unsupported modules available or 
 *   - NAGIOS_CRITICAL(2) if there are security updates available
 */
function _nagios_exit_status($parsed_update_info) {

  if ($parsed_update_info['has_updates']) {
    $security_updates = (count($parsed_update_info['security_updates']) != 0);
    $updates = (count($parsed_update_info['security_updates']) != 0);
    $not_supported = (count($parsed_update_info['not_supported']) != 0);
    
    if ($security_updates) {
      return NAGIOS_CRITICAL;
    }
    elseif ($updates || $not_supported) {
      return NAGIOS_WARNING;
    }    
  }  
  else {
    return NAGIOS_OK;
  }
} 

/** @} */ /** End of defgroup nagios */
 
/**
 * @defgroup filesystem Filesystem functions
 * Functions that do search and scan operation on the local file system.
 * @{  
 */

/** @} */ /** End of defgroup helper */

