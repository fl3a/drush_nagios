<?php
/**
 * @file 
 * Additional explaination of nagios cammand specific options
 */

/**
 * @ingroup nagios_command_specific
 * Comma seperated list of projects which should be ignored, useful for patched, modified modules etc 
 *
 * @see $aliases['dev'] for usual notation 
 */
$aliases['dev']['command-specific']['nagios']['ignore'] = 'project1,project2,projectN';

/**
 * @ingroup nagios_command_specific
 * Show used / current and recommended versions for a affected project  
 *
 * @see $aliases['dev'] for usual notation 
 */
$aliases['dev']['command-specific']['nagios']['show-versions'] = TRUE;

/**
 * @ingroup nagios_command_specific
 * Show release link / link to changelog for the recommended version of a affected projects
 *
 * @see $aliases['dev'] for usual notation 
 */
$aliases['dev']['command-specific']['nagios']['changelog'] = TRUE;;

