<?php
/**
 * @file 
 * Additional explaination of nagios cammand specific options
 */


/**
 * @defgroup nagios_command_specific aliases.drush.inc
 * Nagios command specific options for drush site aliases
 * @{
 */

/**
 * Example site alias "dev" with command specific options for nagios, usual aliases.drushrc.php notation
 */
$aliases['dev'] = array(
  'uri' => 'dev.mydrupalsite.com',
  'root' => '/path/to/drupal/root',

  'command-specific' => array (
    'nagios' => array(
      'ignore' => 'project1,project2,projectN', 
      'show-versions' => TRUE,
      'changelog' => TRUE, 
    ),
  )
);

/**
 * Comma seperated list of projects which should be ignored, useful for patched, modified modules etc 
 *
 * @see $aliases['dev'] for usual notation 
 */
$aliases['dev']['command-specific']['nagios']['ignore'] = 'project1,project2,projectN';

/**
 * Show used / current and recommended versions for a affected project  
 *
 * @see $aliases['dev'] for usual notation 
 */
$aliases['dev']['command-specific']['nagios']['show-versions'] = TRUE;

/**
 * Show release link / link to changelog for the recommended version of a affected projects
 *
 * @see $aliases['dev'] for usual notation 
 */
$aliases['dev']['command-specific']['nagios']['changelog'] = TRUE;;

/** @} */ /** End of nagios_command_specific */
