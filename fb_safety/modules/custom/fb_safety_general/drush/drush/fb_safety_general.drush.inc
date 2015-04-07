<?php
/**
 * @file
 * Drush integration of HV notify.
 *
 */

/**
 * Implement hook_drush_help().
 */
function hiro_drush_help($section) {
  switch ($section) {
    case 'drush:hiro_process_data':
      return dt('Update Hiro data.');
  }
}

/**
 * Implement hook_drush_command().
 */
function hiro_drush_command() {
  $items = array();

  $items['hiro_process_data'] = array(
    'callback' => 'hiro_drush_process_data',
    'drupal dependencies' => array('hiro'),
    'description' => 'Update Hiro data.',
    'options' => array(
      'range' => 'Amount of media sellers to process; defaults to 10',
      'date' => 'Date to start the processing from (counts as the message date); defaults to current day. Note: format of the date can be any strtotime() compatible format (ex. 27-07-2012)',
      'days' => 'Amout of days to process from the date specified; defaults to 1',
      'testing' => 'Output information; defaults to false',
      'nids' => 'Specific node ID to process; defaults to false',
      'mids' => 'Reprocess specific messages'
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_CONFIGURATION,
    'aliases' => array('uhd'),
    'examples' => array(
      'drush uhd --range=10 --testing=false' =>' process 10 media sellers.',
      'drush uhd --nids=4,6,25 --range=1' =>' process media sellers #4, #6, #25.',
      'drush uhd --nids=4,6,25 --range=1 --days=5' =>' process media sellers #4, #6 and #25 five days back',
      'drush uhd --nids=1,6 --range=3' => ' process media sellers #1 to #4 and #6 to #9',
      'drush uhd --nids=4 --date="27-7-2012" --days=4' =>' process 10 media sellers starting from #4, from 27-7 to 31-7.',
      'drush uhd --days=4' => ' process 10 media sellers starting from the last one who was process, for today and the last three days.',
      'drush uhd --mids=1,2,3,4,5' => ' reprocess messages 1 through 5.',
    ),
  );

  $items['hiro_reset_data'] = array(
    'callback' => 'hiro_drush_reset_data',
    'drupal dependencies' => array('hiro'),
    'description' => 'Reset Hiro data. Should only be used if "Hiro update data" command fails to retrieve the correct data.',
    'options' => array(
      'date' => 'Date to start the processing from (counts as the message date); Note: format of the date can be any strtotime() compatible format (ex. 27-07-2012)',
      'nids' => 'Specific node ID to process; defaults to all',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_CONFIGURATION,
    'aliases' => array('rhd'),
    'examples' => array(
      'drush rhd --date="27-07-2014"' => ' Reset data for all of the media seller for that date.',
      'drush rhd --date="27-07-2014 --nids=27,56"' => ' Reset data for media sellers with node IDs 27,56 for that date.',
    ),
  );

  return $items;
}

/**
 * Callback function for starting the data processing.
 */
function hiro_drush_process_data() {
  $mids = drush_get_option('mids');
  if (!empty($mids)) {
    hiro_drush_process_by_mids(explode(',', $mids));
    return;
  }

  $options = array(
    'range' => 10,
    'date' => NULL,
    'testing' => FALSE,
    'nids' => FALSE,
    // Default value for 'days' is defined later.
    'days' => NULL,
  );

  foreach ($options as $option => $value) {
    $$option = drush_get_option($option, $value);
  }
  

  // Send notification email before processing.
  hiro_daily_notification_email();

  // If we are processing a date or days but not for a single media seller,
  // then we assume we want to process all existing media sellers.
  $range = ($days || $date) && !$nids ? 0 : $range;

  // Run for at least one day (default value).
  $days = $days ? $days : 1;

  // Start date - if a date option has been passed, defaults to it - otherwise
  // goes back the amount of days back from today (1 points to today).
  // A date was requested and we're assuming an hour was not specified.
  // Default to midday to avoid timezone clashes.
  $date_start = ($date) ? strtotime($date . " 12:00:00") : strtotime('now -' . ($days - 1) . ' days') ;

  $start_time = microtime(TRUE);

  $loggers = array();
  $nids = explode(',', $nids);

  watchdog('hiro', "Processing started from drush command.", array(), WATCHDOG_INFO);

  // Track runs with parameters that produced errors.
  $failed_parameters = array();

  for ($day = 0; $day < $days; $day++) {
    // Calculate dates.
    // Get current day timestamp.

    $current_day = strtotime(date(HIRO_DATETIME_FORMAT, $date_start) . " +$day days");
    // Calculate the date for the day after the current day that's being
    // processed.
    $day_after = strtotime(date(HIRO_DATETIME_FORMAT, $current_day) . " +1 days");

    // Processing for the current day.
    foreach ($nids as $nid) {
      // Trigger processing for the current node ID.
      $parameters =  implode(', ', array($range, date(HIRO_DATETIME_FORMAT, $current_day), date(HIRO_DATETIME_FORMAT, $day_after), $testing ? 'TRUE' : 'FALSE', $nid ? $nid : 'None'));
      drush_print('Processing with parameters: ' . $parameters);
      $logger = hiro_process_data($range, $current_day, $day_after, $testing, $nid);

      if ($logger->hasError()) {
        $failed_parameters[] = $parameters;
      }

      $loggers[] = $logger;
    }
  }

  $total_time = number_format(microtime(TRUE) - $start_time, 2);

  // Produce a single watchdog error for failed media sellers.
  if (!empty($failed_parameters)) {
    watchdog('hiro', "Some media sellers failed: <br/>" . implode( "<br/>", $failed_parameters), array(), WATCHDOG_ERROR);
  }

  // Send logs' data.
  $log = '<hr/>';
  foreach ($loggers as $logger) {
    foreach ($logger->send() as $line) {
      $log .= '<br/>' . htmlspecialchars($line);
    }
    $log .= '<br/><hr/><br/>';
  }

  $env = hiro_get_environment();
  $values['arguments']= array(
    '!log' => $log,
    '@env' => $env,
  );
  $message = message_create('drush_log', $values);
  // Send email to developers mail group.
  $options = array(
    'mail' => 'hiro-dev@gizra.com',
    'save on fail' => FALSE,
    'save on success' => FALSE,
  );

  message_notify_send_message($message, $options);

  watchdog('hiro', "Finished processing - total $total_time seconds.", array(), WATCHDOG_INFO);
}

/**
 * Reprocess messages' data.
 *
 * @param $mids
 *   Array of message ids.
 */
function hiro_drush_process_by_mids($mids) {
  foreach($mids as $mid) {
    $wrapper = entity_metadata_wrapper('message', $mid);

    $nid = $wrapper->field_media_seller_ref->getIdentifier();
    $start_timestamp = $wrapper->timestamp->value();
    $end_timestamp = strtotime(date(HIRO_DATE_FORMAT, $start_timestamp) . ' +1 day');

    hiro_process_data(1, $start_timestamp, $end_timestamp, FALSE, $nid, FALSE);
  }
}

function hiro_drush_reset_data() {
  $date = drush_get_option("date");
  if (!$date) {
    // No date, exit.
    drush_print("Must specify a date in an strtotime() acceptable format.");
    return;
  }

  $date .= " 12:00:00";
  $nids = drush_get_option("nids", array());
  if (!is_array($nids)) {
    $nids = explode(',', $nids);
  }

  hiro_reset_data($date, $nids);
}
