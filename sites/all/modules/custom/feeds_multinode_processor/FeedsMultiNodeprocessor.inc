<?php
/* @file
 * Class definition of FeedsMultiNodeProcessor.
 * http://drupalcode.org/sandbox/twistor/1135898.git/blob_plain/refs/heads/7.x-1.x:/FeedsMultiNodeProcessor.inc
 */
class FeedsMultiNodeProcessor extends FeedsNodeProcessor{

  /**
   * Override parent::newEntity().
   */
  protected function newEntity(FeedsSource $source){
    $node = parent::newEntity($source);
    $node->log = 'Created by FeedsMultiNodeProcessor';
    return $node;
  }

  /**
   * Override parent::entityLoad().
   */
  protected function entityLoad(FeedsSource $source, $nid){
    $node = parent::entityLoad($source, $nid);
    if($this->config['update_existing'] == FEEDS_UPDATE_EXISTING){
      $node->log = 'Updated by FeedsMultiNodeProcessor';
    }else{
      $node->log = 'Replaced by FeedsMultiNodeProcessor';
    }
    return $node;
  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults(){
    $defaults = parent::configDefaults();
    $types = $this->multiNodeContentTypes();
    $type = isset($types['article']) ? 'article' : key($types);
    $defaults['content_types'] = array(
      $type
    );
    return $defaults;
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state){
    $form = parent::configForm($form_state);
    unset($form['content_type']);
    $types = $this->multiNodeContentTypes();
    array_walk($types, 'check_plain');
    $form['content_types'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Content types'),
      '#description' => t('The content types of the nodes to be created.', array(
        '!feed_id' => $this->id
      )),
      '#options' => $types,
      '#disabled' => TRUE,
      '#default_value' => $this->config['content_types']
    );
    return $form;
  }

  /**
   * Override parent::getMappingTargets().
   */
  public function getMappingTargets(){
    $targets = array();
    $types = $this->multiNodeContentTypes();
    foreach($this->config['content_types'] as $type){
      $this->config['bundle'] = $type;
      $type_targets = parent::getMappingTargets();
      foreach($type_targets as $key => $value){
        if(isset($value['name'])){
          $value['name'] = $types[$type] . ': ' . $value['name'];
          $targets[$type . ':' . $key] = $value;
        }
      }
    }
    return $targets;
  }

  /**
   * Override parent::existingEntityId().
   *
   * Copy both FeedsNodeProcessor::existingEntityId() and
   * FeedsProcessor::existingEntityId() adding in the bundle as an argument.
   */
  protected function existingEntityId(FeedsSource $source, FeedsParserResult $result){
    $query = db_select('feeds_item')->fields('feeds_item', array(
      'entity_id'
    ))->condition('feed_nid', $source->feed_nid)->condition('entity_type', $this->entityType())->condition('bundle', $this->config['content_type'])->condition('id', $source->id);
    // Iterate through all unique targets and test whether they do already exist in the database.
    $unique_targets = $this->uniqueTargets($source, $result);
    foreach($unique_targets as $target => $value){
      $target_content = $this->trimPrefix($target, 0); //specimen_observation, location
      if($target_content == $this->config['content_type']){
        switch($this->trimPrefix($target, 1)){
          case 'url':
            $nid = $query->condition('url', $value)->execute()->fetchField();
            break;
          case 'guid':
            $nid = $query->condition('guid', $value)->execute()->fetchField();
            break;
          case 'nid':
            $nid = db_query("SELECT nid FROM {node} WHERE nid = :nid", array(
              ':nid' => $value
            ))->fetchField();
            break;
          case 'feeds_source':
            if($id = feeds_get_importer_id($this->config['content_type'])){
              $nid = db_query("SELECT fs.feed_nid FROM {node} n JOIN {feeds_source} fs ON n.nid = fs.feed_nid WHERE fs.id = :id AND fs.source = :source", array(
                ':id' => $id,
                ':source' => $value
              ))->fetchField();
            }
            break;
        }
        if(isset($nid)){return $nid;}
      }
    }
    return 0;
  }

  /**
   * Override parent::process().
   */
  public function process(FeedsSource $source, FeedsParserResult $parser_result){
    $state = $source->state(FEEDS_PROCESS);
    while($item = $parser_result->shiftItem()){
      foreach($this->config['content_types'] as $type){
        $this->config['content_type'] = $type;
        $hash = $this->hash($item);
        if(!($entity_id = $this->existingEntityId($source, $parser_result)) || ($this->config['update_existing'] != FEEDS_SKIP_EXISTING)){
          // Only proceed if item has actually changed.   
          if(!empty($entity_id) && $hash == $this->getHash($entity_id)){
            continue;
          }
          try{
            // Assemble node, map item to it, save.
            if(empty($entity_id)){
              $entity = $this->newEntity($source);
              $this->newItemInfo($entity, $source->feed_nid, $hash);
            }else{
              // already have an entity id
              $entity = $this->entityLoad($source, $entity_id);
              // The feeds_item table is always updated with the info for the most recently processed entity.
              // The only carryover is the entity_id.
              $this->newItemInfo($entity, $source->feed_nid, $hash);
              $entity->feeds_item->entity_id = $entity_id;
            }
            $this->map($source, $parser_result, $entity);
            $this->entityValidate($entity);
            // Allow modules to alter the entity before saving.
            module_invoke_all('feeds_presave', $source, $entity);
            if(module_exists('rules')){
              rules_invoke_event('feeds_import_' . $source->importer()->id, $entity);
            }
            // Enable modules to skip saving at all.
            if(empty($entity->feeds_item->skip)){
              $this->entitySave($entity);
              // Track progress.
              if(empty($entity_id)){
                $state->created++;
              }else{
                $state->updated++;
              }
            }
          }
          catch(Exception $e){
            $state->failed++;
            drupal_set_message($e->getMessage(), 'warning');
            $message = $e->getMessage();
            $message .= '<h3>Original item</h3>';
            $message .= '<pre>' . var_export($item, true) . '</pre>';
            $message .= '<h3>Entity</h3>';
            $message .= '<pre>' . var_export($entity, true) . '</pre>';
            $source->log('import', $message, array(), WATCHDOG_ERROR);
          }
        }
      }
    }
    // Set messages if we're done.
    if($source->progressImporting() != FEEDS_BATCH_COMPLETE){return;}
    $info = $this->entityInfo();
    $tokens = array(
      '@entity' => strtolower($info['label']),
      '@entities' => strtolower($info['label plural'])
    );
    $messages = array();
    if($state->created){
      $messages[] = array(
        'message' => format_plural($state->created, 'Created @number @entity', 'Created @number @entities', array(
          '@number' => $state->created
        ) + $tokens)
      );
    }
    if($state->updated){
      $messages[] = array(
        'message' => format_plural($state->updated, 'Updated @number @entity', 'Updated @number @entities', array(
          '@number' => $state->updated
        ) + $tokens)
      );
    }
    if($state->failed){
      $messages[] = array(
        'message' => format_plural($state->failed, 'Failed importing @number @entity', 'Failed importing @number @entities', array(
          '@number' => $state->failed
        ) + $tokens),
        'level' => WATCHDOG_ERROR
      );
    }
    if(empty($messages)){
      $messages[] = array(
        'message' => t('There are no new @entities.', array(
          '@entities' => strtolower($info['label plural'])
        ))
      );
    }
    foreach($messages as $message){
      drupal_set_message($message['message']);
      $source->log('import', $message, array(), isset($message['level']) ? $message['level'] : WATCHDOG_INFO);
    }
  }

  /**
   * Override parent::map().
   */
  protected function map(FeedsSource $source, FeedsParserResult $result, $target_item = NULL){
    // Static cache $targets as getMappingTargets() may be an expensive method.
    static $sources;
    if(!isset($sources[$this->id])){
      $sources[$this->id] = feeds_importer($this->id)->parser->getMappingSources();
    }
    static $targets;
    if(!isset($targets[$this->id])){
      $targets[$this->id] = $this->getMappingTargets();
    }
    $parser = feeds_importer($this->id)->parser;
    if(empty($target_item)){
      $target_item = array();
    }
    // Filter mappings to the current content type.
    $mappings = array();
    foreach($this->config['content_types'] as $type){
      foreach($this->config['mappings'] as $mapping){
        if(strpos($mapping['target'], $type . ':') === 0){
          $mappings[$type][] = $mapping;
        }
      }
    }
    // Many mappers add to existing fields rather than replacing them. Hence we
    // need to clear target elements of each item before mapping in case we are
    // mapping on a prepopulated item such as an existing node.
    foreach($this->config['content_types'] as $type){
      if(array_key_exists($type, $mappings)){
        foreach($mappings[$type] as $mapping){
          $target = $this->trimPrefix($mapping['target'], 1);
          $target = $this->trimSuffix($target);
          if(isset($targets[$mapping['target']]['real_target'])){
            unset($target_item->{$targets[$mapping['target']]['real_target']});
          }elseif(isset($target_item->{$target})){
            unset($target_item->{$target});
          }
        }
      }
    }
    /*
    This is where the actual mapping happens: For every mapping we envoke
    the parser's getSourceElement() method to retrieve the value of the source
    element and pass it to the processor's setTargetElement() to stick it
    on the right place of the target item.

    If the mapping specifies a callback method, use the callback instead of
    setTargetElement().
    */
    self::loadMappers();
    foreach($this->config['content_types'] as $type){
      if(array_key_exists($type, $mappings)){
        foreach($mappings[$type] as $mapping){
          // Retrieve source element's value from parser.
          if(isset($sources[$this->id][$mapping['source']]) && is_array($sources[$this->id][$mapping['source']]) && isset($sources[$this->id][$mapping['source']]['callback']) && function_exists($sources[$this->id][$mapping['source']]['callback'])){
            $callback = $sources[$this->id][$mapping['source']]['callback'];
            $value = $callback($source, $result, $mapping['source']);
          }else{
            $value = $parser->getSourceElement($source, $result, $mapping['source']);
          }
          if($target_item->type == $type){
            // Map the source element's value to the target.
            if(isset($targets[$this->id][$mapping['target']]) && is_array($targets[$this->id][$mapping['target']]) && isset($targets[$this->id][$mapping['target']]['callback']) && function_exists($targets[$this->id][$mapping['target']]['callback'])){
              $callback = $targets[$this->id][$mapping['target']]['callback'];
              $callback($source, $target_item, $this->trimPrefix($mapping['target'], 1), $value);
            }else{
              $this->setTargetElement($source, $target_item, $this->trimPrefix($mapping['target'], 1), $value);
            }
          }
        }
      }
    }
    return $target_item;
  }

  /**
   * Override parent::newItemInfo().
   */
  protected function newItemInfo($entity, $feed_nid, $hash = ''){
    parent::newItemInfo($entity, $feed_nid, $hash);
    $entity->feeds_item->bundle = $this->config['content_type'];
  }

  private function trimPrefix($string, $position){
    $parts = explode(':', $string, 2);
    return $parts[$position];
  }

  private function trimSuffix($string){
    $parts = explode(':', $string, 2);
    return $parts[0];
  }

  private function multiNodeContentTypes(){
    $types = array(
      'location' => 'Location',
      'specimen_observation' => 'Specimen/Observation'
    );
    return $types;
  }

  protected function hash($item){
    static $serialized_mappings = array();
    if(!isset($serialized_mappings[$this->config['content_type']])){
      $mappings = array();
      foreach($this->config['mappings'] as $mapping){
        if(strpos($mapping['target'], $this->config['content_type'] . ':') === 0){
          $mappings[] = $mapping;
        }
      }
      $serialized_mappings[$this->config['content_type']] = serialize($mappings);
    }
    return hash('md5', serialize($item) . $serialized_mappings[$this->config['content_type']]);
  }
}