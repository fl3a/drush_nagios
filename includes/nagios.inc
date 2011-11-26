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
 *
 * @return $update_info
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
 * @return $updates
 */
function _nagios_parse_update_info($update_info) {

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
 * @todo Monitor for database updates
 *  
 * @return   
 * NAGIOS_OK (0) if monitored site is up to date
 * @return
 * or NAGIOS_WARNING (1) if there are updates or unsupported modules available
 * @return
 * or NAGIOS_CRITICAL(2) if there are security updates available
 */
function _nagios_site() {
  
  $update_info = _nagios_get_update_info();

  // Get project name and type of update
  foreach($update_info as $project) {
    switch (pm_update_filter($project)) {
      case dt('Update available') :
  	(drush_get_option('changelog')) 
          ? $updates[] = $project['name'] . '( ' . $project['releases'][$project['latest_version']]['release_link'] . ' )' 
          : $updates[] = $project['name'];
        $updatable = TRUE;
        break;
      case dt('SECURITY UPDATE available'):
  	(drush_get_option('changelog')) 
          ? $security_updates[] = $project['name'] . '( ' . $project['releases'][$project['latest_version']]['release_link'] . ' )' 
          : $security_updates[] = $project['name'];
        $updatable = TRUE;
        break;
      case dt('Installed version not supported'):
  	(drush_get_option('changelog')) 
          ? $not_supported[] = $project['name'] . '( ' . $project['releases'][$project['latest_version']]['release_link'] . ' )' 
          : $not_supported[] = $project['name'];
        $updatable = TRUE;
        break;
    }    
  }
  
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
