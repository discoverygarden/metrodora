<?php
/**
 * @file
 * Utility functions that don't fit anywhere else.
 */

/**
 * Get the cmodels that should use the metadata tab.
 */
function metrodora_metadata_tab_cmodels() {
  return array(
    'islandora:sp_pdf',
    'islandora:sp_large_image_cmodel',
    'islandora:bookCModel',
    'islandora:sp_basic_image',
    'fedora-system:FedoraObject-3.0',
  );
}
