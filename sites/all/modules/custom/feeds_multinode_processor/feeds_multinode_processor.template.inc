<?php
define('FEEDS_EXCEL_CREATOR', 'Drupal - Excel parser');

/**
 * @file
 * 
 */
/**
 * Callback function to actually retrieve the file.
 */
function feeds_multinode_processor_get_populated_template(){
  // The path of the XLS file should be saved to a session.
  if(isset($_SESSION['feeds_xls']) && is_array($_SESSION['feeds_xls'])){
    // We should have the file path in the $results array, so we will output it
    // to the browser.    
    $session = $_SESSION;
    header('Content-Type: application/vnd.ms-excel');
    header('Content-Disposition: attachment;filename="' . $_SESSION['feeds_xls']['filename'] . '"');
    header('Cache-Control: max-age=0');
    readfile($_SESSION['feeds_xls']['csv_path'] . '.xls');
    // delete the file
    //drupal_unlink($_SESSION['feeds_xls']['csv_path'] . '.xls');
    unset($_SESSION['feeds_xls']);
    exit();
  }else{
    drupal_set_message(t('Generation of the populated Excel template has failed'), 'error');
    drupal_goto();
  }
}

/**
 * Get a PHPExcel cache object.  This gets the most efficient cache object that
 * this environment is capable of using.
 */
function _feeds_multinode_processor_initialise_phpexcel($save_memory = FALSE){
  // Load the library.
  if(function_exists('libraries_get_path')){
    $path = libraries_get_path('PHPExcel');
    $path = "$path/PHPExcel/IOFactory.php";
  }else{
    $path = drupal_get_path('module', 'feeds_xls') . '/PHPExcel/PHPExcel/IOFactory.php';
  }
  require_once $path;
  // We use the IGBinary cache method, as it is the quickest.  If we run in to
  // memory issues, then the SQLite3 function is far more efficient with memory.
  if($save_memory){
    $cache_method = PHPExcel_CachedObjectStorageFactory::cache_to_sqlite3;
  }else{
    $cache_method = PHPExcel_CachedObjectStorageFactory::cache_igbinary;
  }
  if(!PHPExcel_Settings::setCacheStorageMethod($cache_method)){
    $cache_method = PHPExcel_CachedObjectStorageFactory::cache_to_phpTemp;
    PHPExcel_Settings::setCacheStorageMethod($cache_method);
  }
  watchdog('feeds_xls', 'PHPExcel set to use %cache_method cache', array(
    '%cache_method' => $cache_method
  ), WATCHDOG_INFO);
}

/**
 * Callback function to provide the populated template.
 */
function feeds_multinode_processor_download_populated_template($feeds_importer, $bid = FALSE){
  get_class($feeds_importer->processor);
  if(get_class($feeds_importer->parser) == 'FeedsExcelParser'){
    if(empty($feeds_importer->parser->config['no_headers'])){
      // We can go ahead, so here we create the batch and start it.
      $batch = array(
        'title' => t('Creating multinode populated Excel template'),
        'operations' => array(
          array(
            'feeds_multinode_processor_create_csv_of_data',
            array(
              $feeds_importer
            )
          )
        ),
        'finished' => 'feeds_multinode_processor_redirect_to_populated_file',
        'file' => drupal_get_path('module', 'feeds_multinode_processor') . '/feeds_multinode_processor.template.inc'
      );
      batch_set($batch);
      batch_process();
    }else{
      drupal_set_message(t('The current configuration for this importer is set to use no headers.'), 'error');
      drupal_goto(arg(0) . '/' . arg(1));
    }
  }else{
    // This isn't an Excel importer.  Give an error message.
    drupal_set_message(t('The requested Feeds importer does not appear to be using the Excel parser.'), 'error');
    drupal_goto(arg(0) . '/' . arg(1));
  }
}

/**
 * Create a CSV file with data in which can later be converted to a proper XLS
 * file.
 */
function feeds_multinode_processor_create_csv_of_data($feeds_importer, &$context){
  if(!isset($context['sandbox']['progress'])){
    // Calculate the total number of rows to add to this file.
    // Use the processor class to guess what entity type we're importing (this
    // relies on other coders following a similar convention to the feeds
    // module).
    $processor_class = get_class($feeds_importer->processor);
    $bundle = '';
    $context['sandbox']['entity'] = '';
    $context['sandbox']['entity_ids'] = array();
    $context['sandbox']['row'] = 0;
    $context['sandbox']['content_types'] = $feeds_importer->processor->config['content_types'];
    $results = db_select('node', 'e')->fields('e', array(
      'nid'
    ))->condition('type', 'specimen_observation')->execute();
    $entity_key = 'nid';
    foreach($results as $row){
      $context['sandbox']['entity_ids'][] = $row->{$entity_key};
    }
    $context['sandbox']['entity'] = 'node';
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = count($context['sandbox']['entity_ids']);
    // Create the template before adding any rows to it. 
    $context['sandbox']['csv_path'] = drupal_tempnam(file_directory_temp(), $feeds_importer->id);
    // We need a few variables and stuff when finished, so we shove them in the
    // results array.
    $context['results']['csv_path'] = $context['sandbox']['csv_path'];
    // FIXME - repetition!
    $context['results']['filename'] = $feeds_importer->id . '.xls';
    $context['results']['importer'] = $feeds_importer->id;
    file_put_contents($context['results']['csv_path'], "\n");
    $context['finished'] = FALSE;
    $context['progress'] = 0;
  }
  // Load the csv file
  $csvf = fopen($context['sandbox']['csv_path'], 'a');
  // Load the entites
  $loop = 0;
  $ids_to_load = array();
  while($loop < 500 && ($id = array_pop($context['sandbox']['entity_ids'])) != FALSE){
    $ids_to_load[] = $id;
    $loop++;
  }
  $entities = entity_load($context['sandbox']['entity'], $ids_to_load);
  $merged = array();
  // Loop through each of the entities
  foreach($entities as $entity_id => $entity){
    // Get the location node reference items attached to this specimen
    if($location = field_get_items('node', $entity, 'field_location')){
      $location_node = entity_load('node', array(
        $location[0]['nid']
      ));
      $specimen_node[$entity_id] = $entity;
      $merged['location'] = $location_node;
      $merged['specimen_observation'] = $specimen_node;
      unset($specimen_node[$entity_id]);
    }
    // Create a row array for each entity.
    $row_values = array();
    foreach($feeds_importer->processor->config['mappings'] as $key => $mapping){
      $row_values[$key] = '';
      $target = _trimPrefix($mapping['target'], 1);
      $content_type = _trimPrefix($mapping['target'], 0);
      $entity = $merged[$content_type];
      $entity_id = key($merged[$content_type]);
      if(strpos($target, ':')){
        $target = explode(':', $target);
        // The country field is a little awkward, so we strip off the first
        // two parts.
        if(count($target) == 4){ //node:location:field_country:iso2
          array_shift($target);
          array_shift($target);
        }
        // First we check to see if we have a value, and proceed if we do.
        if(isset($entity[$entity_id]->{$target[0]}) && is_array($entity[$entity_id]->{$target[0]}) && count($entity[$entity_id]->{$target[0]})){
          $cell_values = array();
          // The date field users field:start/field:end, while the node_ref
          // field uses field:nid/field:title.  We guess which we're using.
          foreach($entity[$entity_id]->{$target[0]} as $language => $values){
            foreach($entity[$entity_id]->{$target[0]}[$language] as $value){
              switch($target[1]){
                case 'iso2':
                  $cell_values[] = $value['iso2'];
                  break;
                case 'nid':
                  $cell_values[] = $value['nid'];
                  break;
                case 'uid':
                  $cell_values[] = $value['uid'];
                  break;
                case 'start':
                  $cell_values[] = $value['value'];
                  break;
                case 'end':
                  $cell_values[] = $value['value2'];
                  break;
                default:
                  // We most likely have a field collection field.  Lets try to
                  // load the field and check the type.
                  $field = field_info_field($target[0]);
                  if($field['type'] == 'field_collection'){
                    // We restart the loop just so that we can get the IDs to
                    // all load at once
                    $ids = array();
                    foreach($entity[$entity_id]->{$target[0]}[$language] as $value){
                      $ids[] = $value['value'];
                    }
                    foreach(entity_load('field_collection_item', $ids) as $fc_entity){
                      foreach(array_keys($fc_entity->{$target[1]}) as $language){
                        foreach(array_keys($fc_entity->{$target[1]}[$language]) as $delta){
                          $cell_values[] = $fc_entity->{$target[1]}[$language][$delta]['value'];
                        }
                      }
                    }
                    break 3;
                  }
              }
            }
          }
          $row_values[$key] = _feeds_multinode_processor_get_value(implode('|', $cell_values));
        }
      }else{
        switch($target){
          case 'guid':
            // Look for this entity in the feeds_item table.  If not there, we generate an entry for it.
            $guid = FALSE;
            if(isset($entity->uuid)){
              $guid = $entity->uuid;
            }
            $feeds_item = feeds_multinode_processor_get_or_generate_feeds_item_entry($context['sandbox']['entity'], $entity_id, $feeds_importer->id, $guid);
            $row_values[$key] = _feeds_multinode_processor_get_value($feeds_item['guid']);         
            break;
          default:
            if(isset($entity[$entity_id]->{$target})){
              if(is_array($entity[$entity_id]->{$target}) && count($entity[$entity_id]->{$target})){
                $cell_values = array();
                foreach($entity[$entity_id]->{$target} as $language => $values){
                  $implosion = '|';
                  foreach($entity[$entity_id]->{$target}[$language] as $value){
                    foreach($value as $subkey => $item){
                      // Change to the allowed values array().
                      $field = field_info_field($target);
                      if($field && @isset($field['settings']['allowed_values'][$item])){
                        $item = $field['settings']['allowed_values'][$item];
                      }
                      if($item){
                        switch($subkey){
                          case 'tid':
                            // Links to a term.  We need to load the term, and get the
                            // value
                            $term = taxonomy_term_load($item);
                            $cell_values[] = $term->name;
                            break;
                          case 'licence':
                            $licences = creative_commons_get_licence_types();
                            $cell_values[] = $licences[$item];
                            break;
                          default:
                            $cell_values[] = $item;
                            break;
                          // The following cases should not be saved to the file
                          case 'safe_value':
                          case 'format':
                            break;
                          // Handle gm3 fields together.
                          case 'latitude':
                          case 'longitude':
                          case 'region_id':
                          case 'polygon':
                          case 'polyline':
                          case 'rectangle':
                          case 'gm3_type':
                            $field = field_info_field($target);
                            switch($field['type']){
                              // Polygon data.
                              case 'gm3_region':
                              case 'gm3_rectangle':
                              case 'gm3_polygon':
                              case 'gm3_polyline':
                                $implosion = "\n";
                                $cell_values[] = $item;
                                break;
                              case 'gm3_point':
                                // Only enter the data once!
                                if($subkey == 'latitude'){
                                  $implosion = "\n";
                                  $cell_values[] = "POINT:({$value['latitude']},{$value['longitude']})";
                                }
                                break 2;
                              case 'gm3_combination':
                                if($subkey == 'gm3_type'){
                                  // Get the field, and work out which one we have.
                                  $implosion = "\n";
                                  $type = array_pop(explode("_", $item));
                                  if($type == 'region'){
                                    if(isset($value['region_id']) && $value['region_id']){
                                      $cell_values[] = 'REGION:' . $value['region_id'];
                                    }
                                  }else{
                                    if(isset($value[$type]) && $value[$type]){
                                      $cell_values[] = strtoupper($type) . ':' . $value[$type];
                                    }
                                  }
                                }
                                break 2;
                            }
                        }
                      }
                    }
                  }
                }
                $row_values[$key] = _feeds_multinode_processor_get_value(implode($implosion, $cell_values));
              }elseif(is_string($entity[$entity_id]->{$target})){
                $row_values[$key] = _feeds_multinode_processor_get_value($entity[$entity_id]->{$target});
              }
            }
        }
      }
    }
    // Increase the row number
    $context['sandbox']['row']++;
    // Write out the fields
    fputcsv($csvf, $row_values);
  }
  fclose($csvf);
  // Set the progress
  $context['progress'] = $context['sandbox']['row'] / $context['sandbox']['max'];
  $context['finished'] = $context['progress'];
  $context['message'] = t('Added @current entities out of @total to the file.', array(
    '@current' => $context['sandbox']['row'],
    '@total' => $context['sandbox']['max']
  ));
  if(!count($context['sandbox']['entity_ids'])){
    $context['finished'] = TRUE;
    // Set a message warning the user that the download will take a little while
    // to create.
    $context['message'] = t('Please wait while your file is converted.  This can take a few minutes.');
  }
}

/**
 * Helper function to get a value that can be inserted into a cell.
 */
function _feeds_multinode_processor_get_value($value){
  if(substr($value, 0, 1) == '='){return "'" . $value;}
  return $value;
}

/**
 * Finish function for populate data.
 */
function feeds_multinode_processor_redirect_to_populated_file($success, $results, $operations){
  if($success){
    // Increase the memory limit
    ini_set('memory_limit', '1024M');
    ini_set('max_execution_time', 600);
    // Load the library, and set the cache type
    // TODO: We need to check for the size of the CSV file so that we can decide
    // if we need to save memory or not.
    _feeds_multinode_processor_initialise_phpexcel();
    // Open the file.
    $objReader = new PHPExcel_Reader_CSV();
    $objPHPExcel = $objReader->load($results['csv_path']);
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel5');
    feeds_multinode_processor_set_headers(feeds_importer_load($results['importer']), $objPHPExcel, $results['csv_path'] . '.xls');
    //drupal_unlink($results['csv_path']);
    $_SESSION['feeds_xls'] = $results;
    drupal_goto('feeds_xls/getfile');
  }
}

/**
 * Callback function to provide the template.
 */
function feeds_multinode_processor_download_template($feeds_importer){
  // Increase the memory limit
  ini_set('memory_limit', '1024M');
  ini_set('max_execution_time', 240);
  if(get_class($feeds_importer->parser) == 'FeedsExcelParser'){
    if(!$feeds_importer->parser->config['no_headers']){
      feeds_multinode_processor_download_template_helper($feeds_importer);
      exit();
    }else{
      drupal_set_message(t('The current configuration for this importer is set to use no headers.'), 'error');
      drupal_goto(arg(0) . '/' . arg(1));
    }
  }else{
    // This isn't an Excel importer.  Give an error message.
    drupal_set_message(t('The requested Feeds importer does not appear to be using the Excel parser.'), 'error');
    drupal_goto(arg(0) . '/' . arg(1));
  }
}

/**
 * Help text for the import template.
 */
function feeds_multinode_processor_field_type_help_text($field){
  switch($field['type']){
    case 'date':
      return "Dates should be entered in the format 'YYYY/MM/DD'.  You may need to specify that this column is a 'text' column type to ensure the formatting is preserved.";
    case 'default':
      return FALSE;
  }
}

function feeds_multinode_processor_set_headers($feeds_importer, $objPHPExcel, $output = 'php://output'){
  // Set properties
  $title = t('Template for !feeds_name', array(
    '!feeds_name' => $feeds_importer->config['name']
  ));
  $objPHPExcel->getProperties()->setCreator(FEEDS_EXCEL_CREATOR)->setLastModifiedBy(FEEDS_EXCEL_CREATOR)->setTitle($title);
  // Set active sheet index to the first sheet, so Excel opens this as the first sheet
  $objPHPExcel->setActiveSheetIndex(0);
  // Add some data
  $sheet = $objPHPExcel->getActiveSheet();
  // Add the Column headers.
  $required_cells = array();
  foreach($feeds_importer->processor->config['mappings'] as $key => $header){
    $first_letter = floor(($key) / 26);
    $second_letter = ($key % 26) + 1;
    $cell = chr(64 + $second_letter);
    if($first_letter){
      $cell = chr(64 + $first_letter) . $cell;
    }
    $column = $cell;
    $cell = "{$cell}1";
    $sheet->setCellValue($cell, _feeds_multinode_processor_get_value($header['source']));
    // Set the columnn width
    $sheet->getColumnDimension($column)->setAutoSize(TRUE);
    // Validation
    if(isset($feeds_importer->processor->config['content_type']) || isset($feeds_importer->processor->config['vocabulary'])){
      if(isset($feeds_importer->processor->config['content_type'])){
        $bundle = $feeds_importer->processor->config['content_type'];
        $entity_type = 'node';
      }else if(isset($feeds_importer->processor->config['vocabulary'])){
        $bundle = $feeds_importer->processor->config['vocabulary'];
        $entity_type = 'taxonomy_term';
      }
      if(strpos($header['target'], ':')){
        $temp_array = explode(':', $header['target']);
        switch($temp_array[1]){
          case 'guid':
          case 'tid':
            $field = FALSE;
            break;
          default:
            $field = field_info_field($temp_array[0]);
        }
      }else{
        $field = field_info_field($header['target']);
      }
      if($field){
        // Get the field instance, so that we can check if this field is required.
        $class = get_class($feeds_importer->processor);
        $field_instance = field_info_instance($entity_type, $field['field_name'], $bundle);
        $objValidation = $sheet->getDataValidation($column . '2:' . $column . '65536');
        $objValidation->setAllowBlank($field_instance['required'] ? FALSE : TRUE);
        if($help_text = feeds_multinode_processor_field_type_help_text($field)){
          $objValidation->setPromptTitle(t('Help'));
          $objValidation->setPrompt($help_text);
        }
        if($field_instance['required']){
          $required_cells[] = $column;
          $objValidation->setPromptTitle(t('Required'));
          $objValidation->setPrompt(t('This cell may not be left blank') . $help_text ? $help_text : '');
        }
        $objValidation->setShowInputMessage(TRUE);
        $objValidation->setShowErrorMessage(TRUE);
        $allowed_values = array();
        if(isset($field['settings']['allowed_values_function']) && function_exists($field['settings']['allowed_values_function'])){
          $allowed_values = call_user_func($field['settings']['allowed_values_function'], $field);
        }else if(isset($field['settings']['allowed_values']) && is_array($field['settings']['allowed_values'])){
          $allowed_values = $field['settings']['allowed_values'];
          // Especially for the creative_commons module
          // FIXME - there must be a nicer way of doing this.
          if($field['type'] == 'creative_commons'){
            $allowed_values = creative_commons_get_licence_types();
          }
        }else if($field['type'] == 'node_reference'){
          // If the view name is set, we can not do anything.
          if(!$field['settings']['view']['view_name']){
            $results = db_select('node', 'n')->condition('type', $field['settings']['referenceable_types'])->fields('n', array(
              'nid',
              'title'
            ))->execute();
            $target_parts = explode(':', $header['target']);
            foreach($results as $row){
              $allowed_values[] = $row->$target_parts[1];
            }
          }
        }
        if(count($allowed_values)){
          $objValidation->setType(PHPExcel_Cell_DataValidation::TYPE_LIST);
          $objValidation->setErrorStyle(PHPExcel_Cell_DataValidation::STYLE_STOP);
          $objValidation->setShowDropDown(TRUE);
          $objValidation->setErrorTitle('Input error');
          $objValidation->setError(t('Your input did not match one of the allowed values.'));
          $objValidation->setPromptTitle(t('Allowed input'));
          foreach($allowed_values as $allowed_value){
            $num_sheets = count($objPHPExcel->getAllSheets());
            if($num_sheets == 1){
              $objPHPExcel->createSheet();
              $objPHPExcel->getSheet(1)->setTitle('PermittedValues');
            }
            $row = 0;
            if(is_array($allowed_value) && isset($allowed_value['vocabulary'])){
              // Get the vocabulary so we can get the vid.
              $vocabulary = taxonomy_vocabulary_machine_name_load($allowed_value['vocabulary']);
              if($vocabulary){
                $results = db_select('taxonomy_term_data', 't')->fields('t', array(
                  'name'
                ))->condition('vid', $vocabulary->vid)->distinct()->execute();
                foreach($results as $term){
                  $row++;
                  $objPHPExcel->getSheet(1)->setCellValue("{$column}{$row}", _feeds_multinode_processor_get_value($term->name));
                }
              }
              $break_at_end = FALSE;
            }else{
              foreach($allowed_values as $term){
                $row++;
                $objPHPExcel->getSheet(1)->setCellValue("{$column}{$row}", _feeds_multinode_processor_get_value($term));
              }
              $break_at_end = TRUE;
            }
            $objValidation->setPrompt(t("Only values from column '!column' in the 'PermittedValues' worksheet are allowed.", array(
              '!columnn' => $column
            )) . "\n\n" . ($field_instance['required'] ? t('This cell may not be left blank.') : t('This cell may be left blank.')));
            $objValidation->setFormula1('PermittedValues!$' . $column . '$1:$' . $column . '$' . ($row));
            if($break_at_end){
              break;
            }
          }
        }
      }
    }else{
      // Here we should set the title to be required, and also other random
      // fields that aren't standard "fields".
      // Set title to required.
      if($header['target'] == 'title' && strtolower(substr(get_class($feeds_importer->processor), 5, strlen(get_class($feeds_importer->processor)) - 14)) == 'node'){
        $objValidation = $sheet->getDataValidation($column . '2:' . $column . '65536');
        $objValidation->setAllowBlank(FALSE);
        $objValidation->setPromptTitle(t('Required'));
        $objValidation->setPrompt(t('This cell may not be left blank'));
        $objValidation->setShowInputMessage(TRUE);
      }
    }
  }
  // setAutoSize for the row
  $sheet->getRowDimension(1)->setRowHeight(30);
  // Set the font for the header row
  $sheet->getStyle("A1:$cell")->getFont()->applyFromArray(array(
    'name' => 'Arial',
    'bold' => TRUE,
    'italic' => FALSE,
    'color' => array(
      'rgb' => 'ffffff'
    ),
    'size' => 14
  ));
  // Set font colour for required cells
  foreach($required_cells as $required_cell){
    $sheet->getStyle($required_cell . '1')->getFont()->applyFromArray(array(
      'name' => 'Arial',
      'bold' => TRUE,
      'italic' => FALSE,
      'color' => array(
        'rgb' => 'ff0000'
      ),
      'size' => 14
    ));
  }
  // Set the background colour for the header row.
  $sheet->getStyle("A1:$cell")->getFill()->applyFromArray(array(
    'type' => PHPExcel_Style_fill::FILL_SOLID,
    'color' => array(
      'rgb' => '000000'
    )
  ));
  // Size the cells accordingly.
  $sheet->getStyle("A1:$cell")->getAlignment()->applyFromArray(array(
    'horizontal' => PHPExcel_Style_Alignment::HORIZONTAL_CENTER,
    'vertical' => PHPExcel_Style_Alignment::VERTICAL_CENTER,
    'rotation' => 0,
    'wrap' => FALSE,
    'shrinkToFit' => FALSE
  ));
  // Rename sheet
  $sheet->setTitle('Template');
  // Redirect output to a client’s web browser (Excel5)
  header('Content-Type: application/vnd.ms-excel');
  $filename = str_replace(' ', '_', preg_replace('/[^a-z\ ]/', '', strtolower($feeds_importer->config['name'])));
  header('Content-Disposition: attachment;filename="TEMPLATE-' . $filename . '.xls"');
  header('Cache-Control: max-age=0');
  $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel5');
  $objWriter->save($output);
}

function feeds_multinode_processor_download_template_helper($feeds_importer, $output = 'php://output'){
  _feeds_multinode_processor_initialise_phpexcel();
  // Create new PHPExcel object
  $objPHPExcel = new PHPExcel();
  feeds_multinode_processor_set_headers($feeds_importer, $objPHPExcel, $output);
}

function _trimPrefix($string, $position){
  $parts = explode(':', $string, 2);
  return $parts[$position];
}