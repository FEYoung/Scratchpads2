<?php
/**
 * @file
 * import_into_specimens_and_locations.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function import_into_specimens_and_locations_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'import_into_specimens_and_locations';
  $feeds_importer->config = array(
    'name' => 'Import into: specimens and locations',
    'description' => 'Import into: specimens and locations',
    'fetcher' => array(
      'plugin_key' => 'FeedsFileFetcher',
      'config' => array(
        'allowed_extensions' => 'txt csv tsv xml opml xls xlsx',
        'direct' => 0,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsExcelParser',
      'config' => array(
        'all_worksheets' => 0,
        'max_rows' => 65535,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsMultiNodeProcessor',
      'config' => array(
        'expire' => '-1',
        'author' => '0',
        'mappings' => array(
          0 => array(
            'source' => 'Location GUID',
            'target' => 'location:guid',
            'unique' => 1,
          ),
          1 => array(
            'source' => 'Location Title',
            'target' => 'location:title',
            'unique' => 0,
          ),
          2 => array(
            'source' => 'Location Continent/Ocean',
            'target' => 'location:field_continent_or_ocean',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'Location Country (ISO alpha-2)',
            'target' => 'location:node:location:field_country:iso2',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'Location Island group',
            'target' => 'location:field_island_group',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'Location Island',
            'target' => 'location:field_island',
            'unique' => FALSE,
          ),
          6 => array(
            'source' => 'Location County',
            'target' => 'location:field_county',
            'unique' => FALSE,
          ),
          7 => array(
            'source' => 'Location State/Province',
            'target' => 'location:field_state_province',
            'unique' => FALSE,
          ),
          8 => array(
            'source' => 'Location Locality',
            'target' => 'location:field_locality',
            'unique' => FALSE,
          ),
          9 => array(
            'source' => 'Location Map',
            'target' => 'location:field_map',
            'unique' => FALSE,
          ),
          10 => array(
            'source' => 'Location Maximum depth',
            'target' => 'location:field_max_depth',
            'unique' => FALSE,
          ),
          11 => array(
            'source' => 'Location Minimum depth',
            'target' => 'location:field_min_depth',
            'unique' => FALSE,
          ),
          12 => array(
            'source' => 'Specimen GUID',
            'target' => 'specimen_observation:guid',
            'unique' => 1,
          ),
          13 => array(
            'source' => 'Specimen Basis of record',
            'target' => 'specimen_observation:field_basis_of_record',
            'unique' => FALSE,
          ),
          14 => array(
            'source' => 'Specimen Catalogue number',
            'target' => 'specimen_observation:field_catalogue_number',
            'unique' => FALSE,
          ),
          15 => array(
            'source' => 'Specimen Collection code',
            'target' => 'specimen_observation:field_collection_code',
            'unique' => FALSE,
          ),
          16 => array(
            'source' => 'Specimen Institution code',
            'target' => 'specimen_observation:field_institution_code',
            'unique' => FALSE,
          ),
          17 => array(
            'source' => 'Specimen Collector number',
            'target' => 'specimen_observation:field_collector_number',
            'unique' => FALSE,
          ),
          18 => array(
            'source' => 'Specimen Count',
            'target' => 'specimen_observation:field_count',
            'unique' => FALSE,
          ),
          19 => array(
            'source' => 'Specimen Date collected (Start)',
            'target' => 'specimen_observation:field_date_collected:start',
            'unique' => FALSE,
          ),
          20 => array(
            'source' => 'Specimen Lifestage',
            'target' => 'specimen_observation:field_lifestage',
            'unique' => FALSE,
          ),
          21 => array(
            'source' => 'Specimen Remarks',
            'target' => 'specimen_observation:field_remarks',
            'unique' => FALSE,
          ),
          22 => array(
            'source' => 'Specimen Taxonomic name (Name)',
            'target' => 'specimen_observation:field_taxonomic_name',
            'unique' => FALSE,
          ),
          23 => array(
            'source' => 'Specimen Taxonomic name (TID)',
            'target' => 'specimen_observation:field_taxonomic_name:tid',
            'unique' => FALSE,
          ),
          24 => array(
            'source' => 'Specimen Taxonomic name (GUID)',
            'target' => 'specimen_observation:field_taxonomic_name:guid',
            'unique' => FALSE,
          ),
          25 => array(
            'source' => 'Specimen Date collected (End)',
            'target' => 'specimen_observation:field_date_collected:end',
            'unique' => FALSE,
          ),
          26 => array(
            'source' => 'Specimen Date identified (Start)',
            'target' => 'specimen_observation:field_date_identified:start',
            'unique' => FALSE,
          ),
          27 => array(
            'source' => 'Specimen Date identified (End)',
            'target' => 'specimen_observation:field_date_identified:end',
            'unique' => FALSE,
          ),
          28 => array(
            'source' => 'Specimen GenBank number(s)',
            'target' => 'specimen_observation:field_genbank_number',
            'unique' => FALSE,
          ),
          29 => array(
            'source' => 'Specimen Identification qualifier',
            'target' => 'specimen_observation:field_identification_qualifier',
            'unique' => FALSE,
          ),
          30 => array(
            'source' => 'Specimen Field notes',
            'target' => 'specimen_observation:field_notes',
            'unique' => FALSE,
          ),
          31 => array(
            'source' => 'Specimen Field number',
            'target' => 'specimen_observation:field_number',
            'unique' => FALSE,
          ),
          32 => array(
            'source' => 'Specimen Sex',
            'target' => 'specimen_observation:field_sex',
            'unique' => FALSE,
          ),
          33 => array(
            'source' => 'Specimen Type status',
            'target' => 'specimen_observation:field_type_status',
            'unique' => FALSE,
          ),
          34 => array(
            'source' => 'Specimen Other catalogue numbers',
            'target' => 'specimen_observation:field_other_catalogue_numbers',
            'unique' => FALSE,
          ),
          35 => array(
            'source' => 'Location Maximum elevation',
            'target' => 'location:field_max_elevation',
            'unique' => FALSE,
          ),
          36 => array(
            'source' => 'Location Minimum elevation',
            'target' => 'location:field_min_elevation',
            'unique' => FALSE,
          ),
          37 => array(
            'source' => 'Location Verbatim Coordinate System',
            'target' => 'location:field_coordinate_system',
            'unique' => FALSE,
          ),
          38 => array(
            'source' => 'Location Coordinate uncertainty in meters',
            'target' => 'location:field_coordinate_uncertainty',
            'unique' => FALSE,
          ),
          39 => array(
            'source' => 'Location Geodetic datum',
            'target' => 'location:field_geodetic_datum',
            'unique' => FALSE,
          ),
          40 => array(
            'source' => 'Location Georeference protocol',
            'target' => 'location:field_georeference_protocol',
            'unique' => FALSE,
          ),
          41 => array(
            'source' => 'Location Georeference remarks',
            'target' => 'location:field_georeference_remarks',
            'unique' => FALSE,
          ),
          42 => array(
            'source' => 'Specimen Location (Title)',
            'target' => 'specimen_observation:field_location:title',
            'unique' => FALSE,
          ),
          43 => array(
            'source' => 'Specimen Location (NID)',
            'target' => 'specimen_observation:field_location:nid',
            'unique' => FALSE,
          ),
          44 => array(
            'source' => 'Specimen Location (GUID)',
            'target' => 'specimen_observation:field_location:guid',
            'unique' => FALSE,
          ),
          45 => array(
            'source' => 'Specimen Collector (UID)',
            'target' => 'specimen_observation:field_collector:uid',
            'unique' => FALSE,
          ),
          46 => array(
            'source' => 'Specimen Collector (Name on Site)',
            'target' => 'specimen_observation:field_collector:name',
            'unique' => FALSE,
          ),          
          47 => array(
            'source' => 'Specimen Identified by (UID)',
            'target' => 'specimen_observation:field_identified_by:uid',
            'unique' => FALSE,
          ),
          48 => array(
            'source' => 'Specimen Identified by (Name on site)',
            'target' => 'specimen_observation:field_identified_by:name',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '2',
        'input_format' => 'filtered_html',
        'content_types' => array(
          'location' => 'location',
          'specimen_observation' => 'specimen_observation',
        ),
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['import_into_specimens_and_locations'] = $feeds_importer;

  return $export;
}
