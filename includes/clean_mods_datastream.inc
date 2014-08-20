<?php

/**
 * @file
 * Derivative callback and supporting functions to clean MODS elements.
 */

/**
 * Derivative callback; Clean unused elements and update the MODS.
 */
function metrodora_clean_mods_datastream(AbstractObject $object, $force, $hook) {
  if (!isset($object['MODS'])) {
    return;
  }

  $original_doc = new DOMDocument();
  $original_doc->loadXML($object['MODS']->content);
  $transformed_doc = metrodora_run_xslt($original_doc,
  drupal_get_path('module', 'xml_form_builder') . '/self_transforms/islandora_cleanup_mods_extended.xsl');
  if (!metrodora_domdocuments_equal($original_doc, $transformed_doc)) {
    $object['MODS']->content = $transformed_doc->saveXML();
    return array(
      'success' => TRUE,
      'messages' => array(
        array(
          'message' => t('Removed unused element(s) from MODS datastream for @pid.'),
          'message_sub' => array(
            '@pid' => $object->id,
          ),
          'type' => 'dsm',
        ),
      ),
    );
  }
}

/**
 * Run the XSLT on the given DOMDocment.
 *
 * @param DOMDocument $doc
 *   The document to be transformed.
 * @param string $xslt_path
 *   The path to the XSLT to perform the transformation.
 *
 * @return DOMDocument
 *   The transformed document.
 */
function metrodora_run_xslt(DOMDocument $doc, $xslt_path) {
  $xsl_doc = new DOMDocument();
  $xsl_doc->load($xslt_path);
  $xslt_proc = new XSLTProcessor();
  $xslt_proc->importStyleSheet($xsl_doc);
  $xml_data = $xslt_proc->transformToDoc($doc);
  $xml_data->preserveWhiteSpace = FALSE;
  $xml_data->formatOutput = TRUE;
  return $xml_data;
}

/**
 * Naive check for equivalence between the contents of two DOMDocuments.
 *
 * NOTE: This function will alter whitespace and format the contents of both
 * documents input.
 *
 * @param DOMDocument $alpha
 *   A DOMDocument to compare.
 * @param DOMDocument $bravo
 *   A DOMDocument to compare.
 *
 * @return bool
 *   TRUE if the contents appear to be equal; otherwise, FALSE.
 */
function metrodora_domdocuments_equal(DOMDocument $alpha, DOMDocument $bravo) {
  $docs = array(
    $alpha,
    $bravo,
  );
  foreach ($docs as $doc) {
    $doc->formatOutput = TRUE;
    $doc->preserveWhiteSpace = FALSE;
    $doc->normalizeDocument();
  }

  return $alpha->saveXML($alpha->documentElement) == $bravo->saveXML($bravo->documentElement);
}
