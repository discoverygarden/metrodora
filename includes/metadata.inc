<?php
/**
 * @file
 * Holds metadata customizations for METROdora.
 */

/**
 * Constructs the parent compound object metadata.
 *
 * @param AbstractObject $object
 *   An AbstractObject representing an object within Fedora.
 *
 * @return array
 *   A renderable array representing the metadata.
 */
function metrodora_compound_parent_metadata(AbstractObject $object) {
  module_load_include('inc', 'islandora', 'includes/metadata');
  $children = islandora_compound_object_get_parts($object);
  $pid = (!empty($children)) ? $children[0] : $object->id;
  $compound_object = islandora_object_load($pid);
  $compound_info = islandora_compound_object_retrieve_compound_info($compound_object);
  $parent_object = islandora_object_load($compound_info['parent_pid']);
  return array(
    'metadata' => array(
      '#markup' => islandora_retrieve_metadata_markup($parent_object, TRUE),
    ),
  );
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
