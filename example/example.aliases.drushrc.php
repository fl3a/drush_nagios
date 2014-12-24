<?php

/**
 * @file 
 * Drush site alias configuration with command specific options
 * for drush_nagios (https://www.drupal.org/project/drush_nagios)
 * used via NRPE or via ssh.
 *
 * Drush will search for aliases in any of these files using
 * the alias search path.  The following locations are examined
 * for alias files:
 *
 *   - In any path set in $options['alias-path'] in drushrc.php,
 *    -  or (equivalently) any path passed in via --alias-path=...
 *    -  on the command line.
 *   - If 'alias-path' is not set, then in one of the default locations:
 *    - /etc/drush
 *    - In the drush installation folder
 *    - Inside the 'aliases' folder in the drush installation folder
 *    - $HOME/.drush
 *   - Inside the sites folder of any bootstrapped Drupal site,
 *      or any local Drupal site indicated by an alias used as
 *      a parameter to a command
 *      
 * @see http://api.drush.org/api/drush/examples%21example.aliases.drushrc.php/5.x
 * for example of valid statements for an alias file.
 */

/**
 * Settings for connecting a (remote) site  on another host *through ssh*.
 */
$aliases['remote'] = array(
  'remote-user' => 'user',
  'remote-host' => 'server.example.com',
  'remote-os' => 'Linux',
  'ssh-options' => '-p 1337 -i /home/user/.ssh/identiy',
);

/**
 * Site alias 'dev' with command specific options for drush_nagios
 *
 * - Parent setting
 * @code $aliases['dev']['parent'] = '@remote';@endcode
 *   - If monitoring should be done *through ssh* you will need this option
 * in combination with above declared #$aliases['remote'].
 *   - If monitoring should be done through NRPE, you will not need this line and
 * above declared #$aliases['remote']`.
 * - Specific options for check-updates command
 *  - ignore
 * @code $aliases['dev']['command-specific']['check-updates']['ignore'] = 'project1,project2,projectN';@endcode
 *   - Comma seperated list of projects which should be ignored, useful for patched, modified modules etc.
 *  - ignore-locked
 * @code $aliases['dev']['command-specific']['check-updates']['ingnore-locked'] = 1';@endcode
 *   - Ignore projects which are locked by `drush pm-update --lock`.
 * - Specific options for check-drupal-requirements command
 *  - ignore
 * @code $aliases['dev']['command-specific']['check-drupal-requirements']['ignore'] = 'facility1,facility2,facilityN';@endcode
 *    - Facility or comma seperated list of facilities which should be ignored,
 */
$aliases['dev'] = array(
  'uri' => 'dev.example.com',
  'root' => '/path/to/drupal/root',
  'parent' => '@remote',

  'command-specific' => array (
    'check-updates' => array(
      'ignore' => 'project1,project2,projectN',
      'ignore-locked' => 1,
    ),
    'check-drupal-requirements' => array(
      'ignore' => 'facility1,facility2,facilityN',
    )
  ),
);
