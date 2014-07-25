<?php

/**
 * @file
 * Drush command/hook implementation for updating existing DC records.
 */

/**
 * Implements hook_drush_command().
 */
function metrodora_drush_command() {
  $commands = array();

  $commands['metrodora-update-dc'] = array(
    'description' => dt("Update all existing objects' Dublin Core with METRO's MODS to DC Transform"),
    'drupal dependencies' => array(
      'islandora',
      'metrodora',
    ),
    'options' => array(
      'time' => array(
        'description' => 'The timestamp to be used to query against. Can use date +%s on the command line to get the current time.',
        'required' => TRUE,
      ),
    ),
    'examples' => array(
      'drush -u 1 metrodora-update-dc --time=1406300166' => dt("Updating all existing object's Dublin Core."),
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  return $commands;
}

/**
 * Command callback to update DC records.
 */
function drush_metrodora_update_dc() {
  batch_set(metrodora_update_dc_create_batch());
  drush_backend_batch_process();
}

/**
 * Constructs a batch used to update things via Drush.
 */
function metrodora_update_dc_create_batch() {
  return array(
    'operations' => array(
      array('metrodora_update_dc_batch_operation', array(drush_get_option('time'))),
    ),
    'title' => t('Updating Dublin Core records for objects...'),
    'init_message' => t('Preparing to update Dublin Core.'),
    'progress_message' => t('Time elapsed: @elapsed <br/>Estimated time remaining @estimate.'),
    'error_message' => t('An error has occurred.'),
    'file' => drupal_get_path('module', 'metrodora') . '/metrodora.drush.inc',
  );
}

/**
 * Constructs and performs the DC records batch operation.
 *
 * @param long $time
 *   The time to be used to query against.
 * @param array $context
 *   The context of the Drupal batch.
 */
function metrodora_update_dc_batch_operation($time, &$context) {
  $update = 10;
  $now = gmdate("Y-m-d\TH:i:s\Z", $time);
  $query = <<<EOQ
SELECT ?pid FROM <#ri>
WHERE {
  ?pid <fedora-view:disseminates> ?ds ;
       <info:fedora/fedora-system:def/view#lastModifiedDate> ?lmd .
  ?ds <fedora-view:disseminationType> <info:fedora/*/MODS> .
  FILTER(?lmd < '$now'^^xsd:dateTime)
}
EOQ;
  $connection = islandora_get_tuque_connection();
  $sandbox = &$context['sandbox'];
  if (!isset($sandbox['offset'])) {
    $sparql_count = $connection->repository->ri->countQuery($query, 'sparql');
    $sandbox['offset'] = 0;
    $sandbox['total'] = $sparql_count;
    if ($sandbox['total'] === 0) {
      return;
    }
  }

  $context['message'] = t('Processing results @start to @end.', array(
    '@start' => $sandbox['offset'],
    '@end' => min($sandbox['offset'] + $update, $sandbox['total']),
  ));

  $query .= "
  LIMIT $update
  ";
  module_load_include('inc', 'islandora', 'includes/derivatives');
  $results = $connection->repository->ri->sparqlQuery($query);
  if (empty($results)) {
    $context['finished'] = 1;
  }
  foreach ($results as $result) {
    $object = islandora_object_load($result['pid']['value']);
    $last_modified_before = $object->lastModifiedDate;
    islandora_do_derivatives($object, array(
      'force' => TRUE,
      'source_dsid' => 'MODS',
      'destination_dsid' => 'DC',
    ));
    $last_modified_after = $object->lastModifiedDate;
    $success = TRUE;
    if ($last_modified_before == $last_modified_after) {
      $success = FALSE;
    }
    if ($success) {
      drush_log(dt("Dublin Core record updated for @pid.", array('@pid' => $object->id)), 'success');
    }
    else {
      drush_log(dt("Dublin Core record update failed for @pid. Check the Drupal watchdog for detailed errors.", array('@pid' => $object->id)), 'error');
      $sandbox['finished'] = 1;
    }
  }
  $sandbox['offset'] += $update;
}
