<?php

/**
 * Implementation of hook_feeds_tamper_default().
 */
function silver_feeds_tamper_default(){
  // Array of "tampers".
  $export = array();
  if(($importers = feeds_importer_load_all()) != FALSE){
    foreach($importers as $id => $importer){
      // Get the class of the processor, and only go any further if it is
      // taxonomy or node based.
      $processor_class = get_class($importer->processor);
      $entity_guess = strtolower(substr($processor_class, 5, strpos($processor_class, 'Processor') - 5));
      switch($entity_guess){
        case 'term':
          $entity_guess = 'taxonomy';
        default:
          foreach($importer->processor->config['mappings'] as $mapping){
            // Load the field that the mapping is associated with
            if(strpos($mapping['target'], ':')){
              $field_name = explode(':', $mapping['target']);
              $field = field_info_field($field_name[0]);
            }else{
              $field = field_info_field($mapping['target']);
            }
            if($field && ($field['cardinality'] == -1 || $field['cardinality'] > 1)){
              if(!in_array($mapping['target'], array(
                'guid',
                'parentguid'
              ))){
                $feeds_tamper = new stdClass();
                $feeds_tamper->disabled = FALSE;
                $feeds_tamper->api_version = 2;
                $feeds_tamper->id = $id . '-' . $mapping['target'] . '-split_multi_values';
                $feeds_tamper->importer = $id;
                $feeds_tamper->source = $mapping['source'];
                $feeds_tamper->plugin_id = 'explode';
                $feeds_tamper->settings = array(
                  'separator' => '|',
                  'limit' => '',
                  'real_separator' => '|'
                );
                $feeds_tamper->weight = 0;
                $feeds_tamper->description = 'Explode';
                $export[$feeds_tamper->id] = $feeds_tamper;
              }
            }
          }
          $feeds_tamper = new stdClass();
          $feeds_tamper->disabled = FALSE;
          $feeds_tamper->api_version = 2;
          $feeds_tamper->id = $id . '-guid-calculate_hash';
          $feeds_tamper->importer = $id;
          $feeds_tamper->source = 'GUID';
          $feeds_tamper->plugin_id = 'hash';
          $feeds_tamper->settings = array(
            'overwrite' => 0
          );
          $feeds_tamper->weight = 0;
          $feeds_tamper->description = 'Calculate hash';
          $export[$feeds_tamper->id] = $feeds_tamper;
          /*
           * Tamper the rank
           */
          $feeds_tamper = new stdClass();
          $feeds_tamper->disabled = FALSE;
          $feeds_tamper->api_version = 2;
          $feeds_tamper->id = $id . '-map_rank';
          $feeds_tamper->importer = $id;
          $feeds_tamper->source = 'Rank';
          $feeds_tamper->plugin_id = 'itis_term_map_rank';
          $feeds_tamper->settings = array(
          );
          $feeds_tamper->weight = 0;
          $feeds_tamper->description = 'Map rank';
          $export[$feeds_tamper->id] = $feeds_tamper;
          /*
           * Tamper unacceptibility
          */
          $feeds_tamper = new stdClass();
          $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
          $feeds_tamper->api_version = 2;
          $feeds_tamper->id = $id . '-map_unacceptibility';
          $feeds_tamper->importer = $id;
          $feeds_tamper->source = 'Unacceptability Reason';
          $feeds_tamper->plugin_id = 'itis_term_map_unacceptibility';
          $feeds_tamper->settings = array(
          );
          $feeds_tamper->weight = 0;
          $feeds_tamper->description = 'Map unacceptibility';
          $export[$feeds_tamper->id] = $feeds_tamper;
          /*
           * Tamper unit indicator 3
          */
          $feeds_tamper = new stdClass();
          $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
          $feeds_tamper->api_version = 2;
          $feeds_tamper->id = $id . '-map_unit_indicator_3';
          $feeds_tamper->importer = $id;
          $feeds_tamper->source = 'Unit indicator 3';
          $feeds_tamper->plugin_id = 'itis_term_map_unit_indicator_3';
          $feeds_tamper->settings = array(
          );
          $feeds_tamper->weight = 0;
          $feeds_tamper->description = 'Map unit indicator 3';
          $export[$feeds_tamper->id] = $feeds_tamper;
          /*
           * Tamper unit indicator 4
          */
          $feeds_tamper = new stdClass();
          $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
          $feeds_tamper->api_version = 2;
          $feeds_tamper->id = $id . '-map_unit_indicator_4';
          $feeds_tamper->importer = $id;
          $feeds_tamper->source = 'Unit indicator 4';
          $feeds_tamper->plugin_id = 'itis_term_map_unit_indicator_4';
          $feeds_tamper->settings = array(
          );
          $feeds_tamper->weight = 0;
          $feeds_tamper->description = 'Map unit indicator 4';
          $export[$feeds_tamper->id] = $feeds_tamper;
          break;
      }
    }
  }
  return $export;
}