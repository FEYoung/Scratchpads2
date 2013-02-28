<?php
/**
 * @file
 * darwincore.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function darwincore_field_group_info() {
  $export = array();

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_collection_display|node|specimen_observation|default';
  $field_group->group_name = 'group_collection_display';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'specimen_observation';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => '',
    'weight' => '2',
    'children' => array(
      0 => 'field_collector',
      1 => 'field_collector_number',
      2 => 'field_date_collected',
      3 => 'field_notes',
      4 => 'field_number',
    ),
    'format_type' => 'field_group_stripy_div',
    'format_settings' => array(
      'formatter' => 'open',
    ),
  );
  $export['group_collection_display|node|specimen_observation|default'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_collection|node|specimen_observation|form';
  $field_group->group_name = 'group_collection';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'specimen_observation';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'label' => 'Event',
    'weight' => '22',
    'children' => array(
      0 => 'field_notes',
      1 => 'field_number',
      2 => 'field_dwc_habitat',
      3 => 'field_event_remarks',
      4 => 'field_sampling_effort',
      5 => 'field_sampling_protocol',
      6 => 'field_verbatim_date',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Event',
      'instance_settings' => array(
        'display_mode' => '',
        'required_fields' => 0,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'closed',
    ),
  );
  $export['group_collection|node|specimen_observation|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_coordinates|node|location|form';
  $field_group->group_name = 'group_coordinates';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'location';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'label' => 'Coordinates',
    'weight' => '24',
    'children' => array(
      0 => 'field_coordinate_system',
      1 => 'field_coordinate_uncertainty',
      2 => 'field_geodetic_datum',
      3 => 'field_georeference_protocol',
      4 => 'field_georeference_remarks',
      5 => 'field_verbatim_latitude',
      6 => 'field_verbatim_longitude',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_coordinates|node|location|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_extra_fields|node|location|form';
  $field_group->group_name = 'group_extra_fields';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'location';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'label' => 'Geography',
    'weight' => '25',
    'children' => array(
      0 => 'field_continent_or_ocean',
      1 => 'field_country',
      2 => 'field_county',
      3 => 'field_island_group',
      4 => 'field_island',
      5 => 'field_locality',
      6 => 'field_state_province',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Geography',
      'instance_settings' => array(
        'display_mode' => '',
        'required_fields' => 0,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'closed',
    ),
  );
  $export['group_extra_fields|node|location|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_genetic_sample|node|specimen_observation|form';
  $field_group->group_name = 'group_genetic_sample';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'specimen_observation';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'label' => 'Sample (Tissue/DNA)',
    'weight' => '27',
    'children' => array(
      0 => 'field_concentration',
      1 => 'field_concentration_unit',
      2 => 'field_extraction_method',
      3 => 'field_extraction_date',
      4 => 'field_extracted_by',
      5 => 'field_person_provided_by',
      6 => 'field_instiruion_provided_by',
      7 => 'field_purification_method',
      8 => 'field_quality',
      9 => 'field_quality_check_date',
      10 => 'field_quality_check_method',
      11 => 'field_ratio_of_absorbance_260_23',
      12 => 'field_ratio_of_absorbance_260_28',
      13 => 'field_sample_type',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Sample (Tissue/DNA)',
      'instance_settings' => array(
        'display_mode' => '',
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'closed',
    ),
  );
  $export['group_genetic_sample|node|specimen_observation|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_height|node|location|form';
  $field_group->group_name = 'group_height';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'location';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'label' => 'Depth/Elevation',
    'weight' => '26',
    'children' => array(
      0 => 'field_max_depth',
      1 => 'field_max_elevation',
      2 => 'field_min_depth',
      3 => 'field_min_elevation',
      4 => 'field_verbatim_elevation',
      5 => 'field_verbatim_depth',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_height|node|location|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_legal|node|specimen_observation|form';
  $field_group->group_name = 'group_legal';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'specimen_observation';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'label' => 'Legal',
    'weight' => '29',
    'children' => array(
      0 => 'field_collecting_permit',
      1 => 'field_collecting_permit_url',
      2 => 'field_collecting_permit_file',
      3 => 'field_benefit_sharing',
      4 => 'field_benefit_sharing_document_u',
      5 => 'field_benefit_sharing_document',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_legal|node|specimen_observation|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_location_display|node|location|default';
  $field_group->group_name = 'group_location_display';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'location';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'BLANK',
    'weight' => '10',
    'children' => array(
      0 => 'field_continent_or_ocean',
      1 => 'field_coordinate_system',
      2 => 'field_coordinate_uncertainty',
      3 => 'field_country',
      4 => 'field_county',
      5 => 'field_geodetic_datum',
      6 => 'field_georeference_protocol',
      7 => 'field_georeference_remarks',
      8 => 'field_island_group',
      9 => 'field_island',
      10 => 'field_locality',
      12 => 'field_max_depth',
      13 => 'field_max_elevation',
      14 => 'field_min_depth',
      15 => 'field_min_elevation',
      16 => 'field_state_province',
    ),
    'format_type' => 'field_group_stripy_div',
    'format_settings' => array(
      'formatter' => 'open',
    ),
  );
  $export['group_location_display|node|location|default'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_location_display|node|location|linked_node';
  $field_group->group_name = 'group_location_display';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'location';
  $field_group->mode = 'linked_node';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'BLANK',
    'weight' => '10',
    'children' => array(
      0 => 'field_continent_or_ocean',
      1 => 'field_coordinate_system',
      2 => 'field_coordinate_uncertainty',
      3 => 'field_country',
      4 => 'field_county',
      5 => 'field_geodetic_datum',
      6 => 'field_georeference_protocol',
      7 => 'field_georeference_remarks',
      8 => 'field_island_group',
      9 => 'field_island',
      10 => 'field_locality',
      12 => 'field_max_depth',
      13 => 'field_max_elevation',
      14 => 'field_min_depth',
      15 => 'field_min_elevation',
      16 => 'field_state_province',
    ),
    'format_type' => 'field_group_stripy_div',
    'format_settings' => array(
      'formatter' => 'open',
    ),
  );
  $export['group_location_display|node|location|linked_node'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_location_other|node|location|form';
  $field_group->group_name = 'group_location_other';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'location';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'label' => 'Other',
    'weight' => '27',
    'children' => array(
      0 => 'field_georeference_sources',
      1 => 'field_verification_status',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_location_other|node|location|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_location|node|specimen_observation|form';
  $field_group->group_name = 'group_location';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'specimen_observation';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'label' => 'Location',
    'weight' => '24',
    'children' => array(
      0 => 'field_location',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Location',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'closed',
    ),
  );
  $export['group_location|node|specimen_observation|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_map|node|location|form';
  $field_group->group_name = 'group_map';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'location';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'label' => 'Map',
    'weight' => '23',
    'children' => array(
      0 => 'field_map',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Collection',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_map|node|location|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_measurements|node|specimen_observation|form';
  $field_group->group_name = 'group_measurements';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'specimen_observation';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'label' => 'Measurements',
    'weight' => '25',
    'children' => array(
      0 => 'field_measurements',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_measurements|node|specimen_observation|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_misc_display|node|specimen_observation|default';
  $field_group->group_name = 'group_misc_display';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'specimen_observation';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => '',
    'weight' => '3',
    'children' => array(
      0 => 'field_count',
      1 => 'field_genbank_number',
      2 => 'field_lifestage',
      3 => 'field_other_catalogue_numbers',
      4 => 'field_remarks',
      5 => 'field_sex',
      6 => 'field_media',
    ),
    'format_type' => 'field_group_stripy_div',
    'format_settings' => array(
      'formatter' => 'open',
    ),
  );
  $export['group_misc_display|node|specimen_observation|default'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_miscellaneous|node|specimen_observation|form';
  $field_group->group_name = 'group_miscellaneous';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'specimen_observation';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'label' => 'Miscellaneous',
    'weight' => '31',
    'children' => array(),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Miscellaneous',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'closed',
    ),
  );
  $export['group_miscellaneous|node|specimen_observation|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_occurrence|node|specimen_observation|form';
  $field_group->group_name = 'group_occurrence';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'specimen_observation';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'label' => 'Occurence',
    'weight' => '26',
    'children' => array(
      0 => 'field_media',
      1 => 'field_collector',
      2 => 'field_collector_number',
      3 => 'field_count',
      4 => 'field_date_collected',
      5 => 'field_genbank_number',
      6 => 'field_lifestage',
      7 => 'field_other_catalogue_numbers',
      8 => 'field_sex',
      9 => 'field_associated_taxa',
      10 => 'field_dwc_behaviour',
      11 => 'field_establishment_means',
      12 => 'field_occurence_remarks',
      13 => 'field_occurrence_status',
      14 => 'field_preparations',
      15 => 'field_previous_identifications',
      16 => 'field_references',
      17 => 'field_reproductive_condition',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_occurrence|node|specimen_observation|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_preparation|node|specimen_observation|form';
  $field_group->group_name = 'group_preparation';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'specimen_observation';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'label' => 'Preparation & Preservation',
    'weight' => '28',
    'children' => array(
      0 => 'field_preparation_process',
      1 => 'field_preparation_materials',
      2 => 'field_prepared_by',
      3 => 'field_preparation_date',
      4 => 'field_preservation_medium',
      5 => 'field_preservation_temperature_c',
      6 => 'field_preservation_date',
    ),
    'format_type' => 'tabs',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );
  $export['group_preparation|node|specimen_observation|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_related_materials|node|specimen_observation|form';
  $field_group->group_name = 'group_related_materials';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'specimen_observation';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'label' => 'Related Materials',
    'weight' => '30',
    'children' => array(),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_related_materials|node|specimen_observation|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_required_display|node|specimen_observation|default';
  $field_group->group_name = 'group_required_display';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'specimen_observation';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => '',
    'weight' => '0',
    'children' => array(
      0 => 'field_basis_of_record',
      1 => 'field_catalogue_number',
      2 => 'field_collection_code',
      3 => 'field_institution_code',
    ),
    'format_type' => 'field_group_stripy_div',
    'format_settings' => array(
      'formatter' => 'open',
    ),
  );
  $export['group_required_display|node|specimen_observation|default'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_required|node|specimen_observation|form';
  $field_group->group_name = 'group_required';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'specimen_observation';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'label' => 'Basics',
    'weight' => '21',
    'children' => array(
      0 => 'field_basis_of_record',
      1 => 'field_catalogue_number',
      2 => 'field_collection_code',
      3 => 'field_institution_code',
      4 => 'field_inherit_values_from',
      5 => 'title',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Basics',
      'instance_settings' => array(
        'display_mode' => '',
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_required|node|specimen_observation|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_tabs|node|location|form';
  $field_group->group_name = 'group_tabs';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'location';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Tabs',
    'weight' => '1',
    'children' => array(
      0 => 'group_extra_fields',
      1 => 'group_map',
      2 => 'group_coordinates',
      3 => 'group_height',
      4 => 'group_location_other',
    ),
    'format_type' => 'tabs',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );
  $export['group_tabs|node|location|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_tabs|node|specimen_observation|form';
  $field_group->group_name = 'group_tabs';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'specimen_observation';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Tabs',
    'weight' => '0',
    'children' => array(
      0 => 'group_genetic_sample',
      1 => 'group_collection',
      2 => 'group_location',
      3 => 'group_measurements',
      4 => 'group_miscellaneous',
      5 => 'group_occurrence',
      6 => 'group_related_materials',
      7 => 'group_required',
      8 => 'group_taxonomy',
      9 => 'group_legal',
      10 => 'group_preparation',
    ),
    'format_type' => 'tabs',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );
  $export['group_tabs|node|specimen_observation|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_taxonomy_display|node|specimen_observation|default';
  $field_group->group_name = 'group_taxonomy_display';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'specimen_observation';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => '',
    'weight' => '1',
    'children' => array(
      0 => 'field_date_identified',
      1 => 'field_identification_qualifier',
      2 => 'field_identified_by',
      3 => 'field_taxonomic_name',
      4 => 'field_type_status',
    ),
    'format_type' => 'field_group_stripy_div',
    'format_settings' => array(
      'formatter' => 'open',
    ),
  );
  $export['group_taxonomy_display|node|specimen_observation|default'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_taxonomy|node|specimen_observation|form';
  $field_group->group_name = 'group_taxonomy';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'specimen_observation';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'label' => 'Identification',
    'weight' => '23',
    'children' => array(
      0 => 'field_date_identified',
      1 => 'field_identification_qualifier',
      2 => 'field_identified_by',
      3 => 'field_remarks',
      4 => 'field_taxonomic_name',
      5 => 'field_type_status',
      6 => 'field_identification_references',
      7 => 'field_identification_verificatio',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Identification',
      'instance_settings' => array(
        'display_mode' => '',
        'required_fields' => 0,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'closed',
    ),
  );
  $export['group_taxonomy|node|specimen_observation|form'] = $field_group;

  return $export;
}
