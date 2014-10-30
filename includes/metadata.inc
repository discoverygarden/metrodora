<?php
/**
 * @file
 * Holds metadata customizations for METROdora.
 */

/**
 * Constructs the parent/current object metadata.
 *
 * @param AbstractObject $object
 *   An AbstractObject representing an object within Fedora.
 *
 * @return array
 *   A renderable array representing the metadata.
 */
function metrodora_metadata_tab(AbstractObject $object) {
  module_load_include('inc', 'islandora', 'includes/metadata');
  $children = islandora_compound_object_get_parts($object);
  $pid = (!empty($children)) ? $children[0] : $object->id;
  $compound_object = islandora_object_load($pid);
  $compound_info = islandora_compound_object_retrieve_compound_info($compound_object);
  if ($compound_info) {
    $metadata_object = islandora_object_load($compound_info['parent_pid']);
  }
  else {
    $metadata_object = $object;
  }
  $tab['metadata'] = array();
  // Add Thumbnail to Object Description Tab except for compound objects.
  if (isset($object['TN']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['TN']) && !$compound_info) {
    $tab['#attached']['css'] = array(
      drupal_get_path('module', 'metrodora') . '/css/metrodora.objectdescription.css',
    );
    $image = array(
      '#prefix' => '<div class="islandora-object-description-thumbnail">',
      '#suffix' => '</div>',
      '#theme' => 'image',
      '#path' => url("islandora/object/{$object->id}/datastream/TN/view"),
      '#attributes' => array(
        'title' => $object->label,
      ),
    );
    $tab['metadata']['image'] = $image;
  }
  $tab['metadata']['markup'] = array(
    '#markup' => islandora_retrieve_metadata_markup($metadata_object, TRUE),
  );
  return $tab;
}

/**
 * Constructs the child compound object metadata.
 *
 * @param AbstractObject $object
 *   An AbstractObject representing an object within Fedora.
 *
 * @return array
 *   A renderable array representing the Drupal form.
 */
function metrodora_compound_child_metadata(AbstractObject $object) {
  module_load_include('inc', 'islandora', 'includes/metadata');
  $children = islandora_compound_object_get_parts($object);
  $pid = (!empty($children)) ? $children[0] : $object->id;
  $compound_object = islandora_object_load($pid);
  return array(
    'metadata' => array(
      '#markup' => islandora_retrieve_metadata_markup($compound_object, TRUE),
    ),
  );
}
