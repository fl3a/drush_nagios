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
 * @defgroup xml_xpath XML xpath functions
 * Functions that uses the xml files at http://updates.drupal.org/release-history/project/core.x with xpath
 * @{
 *  
 */

/**
 * Get the versions of a specified project between used/current and recommended version 
 * from a file or an url
 * 
 * @param $core 
 *   Core compatibility / drupal major version (5.x, 6.x, 7.x and so forth)
 *   but also possible for versions before drupal 5 like drupal/4.7.x or image/4.7.x etc without error
 * @param $project 
 *   Name of the project. Default 'drupal'
 * @param $path    
 *   Path to xml file which contains the release history.
 *   This could be an absolute path to file on your filesystem 
 *   or an url.
 *   Default 'http://updates.drupal.org/release-history/' 
 *   combined with supplied $project,  '/', supplied  $core and '.x'.
 * @return
 *   An array of SimpleXMLElement Objects 
 *   or FALSE via drush_set_error() if $core $project combination does not exist.
 * 
 *   EXAMPLE
 *   We assume 6 was supplied for core, 
 *   'drupal' for project and v.6.22 is the recommended version while we are using v.6.19:
 * @code
 *Array
 *(
 *    [0] => SimpleXMLElement Object
 *        (
 *            [name] => drupal 6.22
 *            [version] => 6.22
 *            [tag] => 6.22
 *            [version_major] => 6
 *            [version_patch] => 22
 *            [status] => published
 *            [release_link] => http://drupal.org/node/1168950
 *            [download_link] => http://ftp.drupal.org/files/projects/drupal-6.22.tar.gz
 *            [date] => 1306357016
 *            [mdhash] => 447490f3bc155fd1d5c2d47280c999a5
 *            [filesize] => 1100999
 *            [files] => SimpleXMLElement Object
 *                (
 *                    [file] => Array
 *                        (
 *                            [0] => SimpleXMLElement Object
 *                                (
 *                                    [url] => http://ftp.drupal.org/files/projects/drupal-6.22.tar.gz
 *                                    [archive_type] => tar.gz
 *                                    [md5] => 447490f3bc155fd1d5c2d47280c999a5
 *                                    [size] => 1100999
 *                                    [filedate] => 1306357016
 *                                )
 *
 *                            [1] => SimpleXMLElement Object
 *                                (
 *                                    [url] => http://ftp.drupal.org/files/projects/drupal-6.22.zip
 *                                    [archive_type] => zip
 *                                    [md5] => 1d7361d7c2171a29721835d1b402b97f
 *                                    [size] => 1269612
 *                                    [filedate] => 1306357016
 *                                )
 *
 *                        )
 *
 *                )
 *
 *            [terms] => SimpleXMLElement Object
 *                (
 *                    [term] => SimpleXMLElement Object
 *                        (
 *                            [name] => Release type
 *                            [value] => Bug fixes
 *                        )
 *
 *                )
 *
 *        )
 *
 *    [1] => SimpleXMLElement Object
 *        (
 *            [name] => drupal 6.21
 *            [version] => 6.21
 *            [tag] => 6.21
 *            [version_major] => 6
 *            [version_patch] => 21
 *            [status] => published
 *            [release_link] => http://drupal.org/node/1168908
 *            [download_link] => http://ftp.drupal.org/files/projects/drupal-6.21.tar.gz
 *            [date] => 1306354917
 *            [mdhash] => d01bf183438b2fbb666f54688704c2bd
 *            [filesize] => 1100218
 *            [files] => SimpleXMLElement Object
 *                (
 *                    [file] => Array
 *                        (
 *                            [0] => SimpleXMLElement Object
 *                                (
 *                                    [url] => http://ftp.drupal.org/files/projects/drupal-6.21.tar.gz
 *                                    [archive_type] => tar.gz
 *                                    [md5] => d01bf183438b2fbb666f54688704c2bd
 *                                    [size] => 1100218
 *                                    [filedate] => 1306354917
 *                                )
 *
 *                            [1] => SimpleXMLElement Object
 *                                (
 *                                    [url] => http://ftp.drupal.org/files/projects/drupal-6.21.zip
 *                                    [archive_type] => zip
 *                                    [md5] => 2fb008c0f69fe8a0ce083e54b836df70
 *                                    [size] => 1271166
 *                                    [filedate] => 1306354917
 *                                )
 *
 *                        )
 *
 *                )
 *
 *            [terms] => SimpleXMLElement Object
 *                (
 *                    [term] => SimpleXMLElement Object
 *                        (
 *                            [name] => Release type
 *                            [value] => Security update
 *                        )
 *
 *                )
 *
 *        )
 *
 *    [2] => SimpleXMLElement Object
 *        (
 *            [name] => drupal 6.20
 *            [version] => 6.20
 *            [tag] => 6.20
 *            [version_major] => 6
 *            [version_patch] => 20
 *            [status] => published
 *            [release_link] => http://drupal.org/node/999422
 *            [download_link] => http://ftp.drupal.org/files/projects/drupal-6.20.tar.gz
 *            [date] => 1292447789
 *            [mdhash] => a4f59401fbb3e20e3a03ac5fc11bd27c
 *            [filesize] => 1100246
 *            [files] => SimpleXMLElement Object
 *                (
 *                    [file] => Array
 *                        (
 *                            [0] => SimpleXMLElement Object
 *                                (
 *                                    [url] => http://ftp.drupal.org/files/projects/drupal-6.20.tar.gz
 *                                    [archive_type] => tar.gz
 *                                    [md5] => a4f59401fbb3e20e3a03ac5fc11bd27c
 *                                    [size] => 1100246
 *                                    [filedate] => 1292447789
 *                                )
 *
 *                            [1] => SimpleXMLElement Object
 *                                (
 *                                    [url] => http://ftp.drupal.org/files/projects/drupal-6.20.zip
 *                                    [archive_type] => zip
 *                                    [md5] => 06114b337ba9d682f44fa13d98ecfa46
 *                                    [size] => 1285970
 *                                    [filedate] => 1293228545
 *                                )
 *
 *                        )
 *
 *                )
 *
 *            [terms] => SimpleXMLElement Object
 *                (
 *                    [term] => SimpleXMLElement Object
 *                        (
 *                            [name] => Release type
 *                            [value] => Bug fixes
 *                        )
 *
 *                )
 *
 *        )
 *
 *)
 * @endcode
 */
//function _nagios_recommended_version($core, $project = 'drupal', $float = FALSE, $path = FALSE) {
function _nagios_versions_till_latest($core, $project = 'drupal', $path = FALSE) {
  
  if (!$path) {
    $path = 'http://updates.drupal.org/release-history/'. $project. '/'. $core;
  }    
  if ($xml = @simplexml_load_file($path)) {
    if ($error = $xml->xpath('/error')) {
  	  return drush_set_error('NAGIOS_COULD_NOT_LOAD_UPDATE_FILE', $error[0]);
    }
    else {
      $recommended_major = $xml->xpath("/project/recommended_major");
      if (!empty($recommended_major)) {
	      $xpath_releases = "/project/releases/release[status='published'][version_major=" . (string)$recommended_major[0] . "]";
	      $releases = $xml->xpath($xpath_releases);
	      //$release = (array)$releases[0];	    	   
	    }
	    else {
	      drush_log('No recommended version available, taking latest release', 'warning');
	      $xpath_releases = '/project/releases/release';
	      $releases = $xml->xpath($xpath_releases);
      }
      // truncate till current/used version and slice array, use index of current/used version as length 
      for($i = 0; $i < sizeof($releases); $i++) {
        $version = $releases[$i]->version;
        if ($version == drush_drupal_version()) {
          $releases = array_slice($releases, 0, $i);
          break;
        }
      }
	    return $releases;
	  }
  }
  else {
  	// We are not getting here since drupal.org always serves an XML response.
	  return drush_set_error('NAGIOS_DOWNLOAD_FAILED', 
	    dt('Could not download project status information from !url', 
	      array('!url' => $path)
	    )
	  );
  }  	
}

/** @} */

/** Nagios functions
 * Functions that deals with Nagios
 * @see @ref nagios_help
 * @{  
 */

/**
 * Monitores a Drupal installation for core updates
 * 
 * @return 
 * NAGIOS_OK(0) if monitored DRUPAL_ROOT is up to date
 * @return
 * or NAGIOS_WARNING (1), if there is new release that contains bug fixes  
 * @return
 * or NAGIOS_CRITICAL(2) if there is new release that contains a security updates
 */
function _nagios_root() {
  
  if ($file = drush_get_option('file')) { 
     if (!is_file($file)) {
       $file = FALSE;
     }
  }
  else {
    $file = FALSE;
  }
  if ($versions = _nagios_versions_till_latest(drush_drupal_major_version(). '.x', 'drupal', $file)) {
    $recommended_version = $versions[0];
    $current_version = (float) drush_drupal_version();
    if ($current_version < (float) $recommended_version->version) {
      $affected_versions = $update_type_indicator = array();
      foreach($versions as $version) {
        $update_type = $version->terms->term->value; 
        $update_type_indicator[] = (string)$update_type; 
        $affected_versions[] = $version->version . '(' . $update_type . ', '. date('Y-m-d', (int) $version->date) .', '.  $version->release_link . ' )'; 
      }
      if (in_array('Security update', $update_type_indicator)) {
        $status_signal = dt('DRUPAL ROOT CRITICAL');
        $status_code = NAGIOS_CRITICAL;
      }
      else {
        $status_signal = dt('DRUPAL ROOT WARNING');
        $status_code = NAGIOS_WARNING;
      }
      drush_print(  
        dt('!status_signal - !version @ !drupal_root: recommended version is !remcommended_version. Affected versions between !version and !remcommended_version: !updates.',
          array(
            '!status_signal' => $status_signal, 
            '!version'     => $current_version, 
            '!drupal_root' => drush_get_context('DRUSH_DRUPAL_ROOT'),
            '!remcommended_version' => $recommended_version->version,
            '!updates' => implode(', ', $affected_versions),
          )
        )
      );
      return $status_code;          
    }
    else {
      drush_print(
        dt('DRUPAL ROOT OK - !version @ !drupal_root is uptodate.',
          array(
          '!version' => $current_version, 
          '!drupal_root' => drush_get_context('DRUSH_DRUPAL_ROOT'),           
          )
        )
      );
      return NAGIOS_OK;           
    }   
  }
  else {
    return $recommended_version; // FALSE if _nagios_recommended_version() sets an error
  }
}

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
  
  drush_include_engine('update_info', 'drupal', NULL, DRUSH_BASE_PATH . '/commands/pm/update_info');
  include(DRUSH_BASE_PATH . '/commands/pm/updatecode.pm.inc');
  
  $updatable = FALSE;
  $updates = $security_updates = $not_supported = array();
  
  drush_bootstrap_max();
    
  // Get update status information.
  ob_start();
  // Get installed extensions and projects.
  $extensions = drush_get_extensions();
  $projects = drush_get_projects($extensions);
  
  // Get update status information.
  $update_info = _pm_get_update_info($projects);
  ob_end_clean();
  
  // Get name and type of update
  foreach($update_info as $project) {
    //drush_print_r($project);
    //exit;
    switch (pm_update_filter($project)) {
      case dt('Update available') :
        $updates[] = $project['name'];
        $updatable = TRUE;
        break;
      case dt('SECURITY UPDATE available'):
        $security_updates[] = $project['name'];
        $updatable = TRUE;
        break;
      case dt('Installed version not supported'):
        $not_supported[] = $project['name'];
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
        $status_msg .= dt(', Updates available for !modules', 
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
