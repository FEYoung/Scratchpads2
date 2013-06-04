<?php
/**
 * feeds_tamper API definition for drupal 
 * Allow for mapping values from an incoming term to an itis term
 */
$plugin = array(
  'form' => 'itis_term_map_unacceptibility_form',
  'callback' => 'itis_term_map_unacceptibility_callback',
  'name' => 'Map unacceptibility',
  'multi' => 'loop',
  'category' => 'ITIS',
  'description' => 'This will allow you to map incoming patterns to ITIS unacceptibility terms. For example incoming "nn" could be mapped to ITIS "invalidly published, nomen nudum"'
);

/**
 * Form for building the Brahms to Scratchpad unacceptibility term mapping
 */
function itis_term_map_unacceptibility_form($importer, $element_key, $settings){
  $vocabulary = taxonomy_vocabulary_machine_name_load($importer->processor->config['vocabulary']);
  $biological_classification = $vocabulary->biological_classification;
  $form = array();
  $allowed_values = itis_term_allowed_values(array(
    'field_name' => 'field_unacceptability_reason'
  ), $biological_classification);
  foreach($allowed_values as $key){
    $form['scratch_map_' . $key] = array(
      '#type' => 'textfield',
      '#title' => t('Patterns to map to ' . $key),
      '#default_value' => isset($settings['scratch_map_' . $key]) ? $settings['scratch_map_' . $key] : ''
    );
  }
  $form['scratch_map_biological_classification'] = array(
    '#type' => 'hidden',
    '#value' => isset($biological_classification) ? $biological_classification : ''
  );
  return $form;
}

/**
 * Function to process the value conversion
 * Called by the feed (via Feed Tamper) to modify data.
 */
function itis_term_map_unacceptibility_callback($result, $item_key, $element_key, &$field, $settings){
  $allowed_values = itis_term_allowed_values(array(
    'field_name' => 'field_unacceptability_reason'
  ), $settings['scratch_map_biological_classification']);
  foreach($allowed_values as $key){
    if($settings['scratch_map_' . $key]){
      $scratch_map_array = explode(",", $settings['scratch_map_' . $key]);
      $scratch_map_array = array_map('trim', $scratch_map_array);
      if(in_array($field, $scratch_map_array)){
        $field = $key;
      }
    }
  }
}
