[Updated 31 Jan 2013]

This module provides 2 jquery plugins; The Powerzoomer and Featuredzoomer.

Powerzoomer demo can be seen here:
http://www.dynamicdrive.com/dynamicindex4/powerzoomer.htm

Featuredzoomer demo can be seen here:
http://www.dynamicdrive.com/dynamicindex4/featuredzoomer.htm

====================================================

INSTALLATION STEPS:

1. Download and place this module to your D7 as normal
(sites/all/modules/imagezoomer)

2. Download the plugins

2.1 For Powerzoomer
http://www.dynamicdrive.com/dynamicindex4/ddpowerzoomer.js

2.2 For Featuredzoomer
http://www.dynamicdrive.com/dynamicindex4/multizoom.js
http://www.dynamicdrive.com/dynamicindex4/multizoom.css
http://www.dynamicdrive.com/dynamicindex4/spinningred.gif

3. Upload all the above to libraries folder so that they look like:

sites/all/libraries/imagezoomer/ddpowerzoomer.js

sites/all/libraries/imagezoomer/multizoom.js
sites/all/libraries/imagezoomer/multizoom.css
sites/all/libraries/imagezoomer/spinningred.gif

4. Enable the module from your Drupal site: /admin/modules

5. Visit this page to configure all the plugins options: /admin/config/user-interface/imagezoomer

====================================================

HOW TO USE:

I. FOR NORMAL USERS

1. In your content type, add an image field

2. Go to "Manage display" then change display format from "Image" to "Power Zoomer" or "Featured Zoomer"

3. Click on the cog wheel and select an image style for displaying and another for zooming. For zooming, normally, select "Original" for the best resolution. You could also select other image style if the original image is too large.

4. Save your setting. Create a new node (of that content type), upload an image then save it.

5. In the node view, you should be able to zoom your image using a magnifying glass now.

II. FOR DEVELOPERS

The module provides a theme function so that developers could use it in coding.

Please take a look at this example below:

1. For Power Zoomer

<?php
print theme('imagezoomer_power_image', $variables);
?>

With $variables can be declared as such:

<?php
$variables = array(
  'file' => $file,
  'image_style_display' => 'style1',
  'image_style_zooming' => 'style2',
  'imagezoomer_power_options' => $options,
);
?>

$file: Drupal image file object. It should include a path to the image file.

'style1': Image style name for displaying. For example: 'product_thumb'.

'style2': Image style name for zooming. It should be larger style 1. For example: 'product_large'.

$options: Are the powerzoomer plug-in options. For example:
$options = array(
  'imagezoomer_power_default_power' => 2,
  'imagezoomer_power_power_range_low' => 2,
  'imagezoomer_power_power_range_high' => 7,
  'imagezoomer_power_magnifier_size_width' => 75,
  'imagezoomer_power_magnifier_size_height' => 75,
);

If you don't set the specific options as above (leave it empty, for example: 'imagezoomer_power_options' => array()) 
the theme will take default settings from the settings page.

2. For Featured Zoomer

<?php
print theme('imagezoomer_featured_image', $variables);
?>

With $variables can be declared as such:

<?php
$variables = array(
  'file' => $file,
  'image_style_display' => 'style1',
  'image_style_zooming' => 'style2',
  'imagezoomer_featured_options' => $options,
);
?>

$file: Drupal image file object. It should include a path to the image file.

'style1': Image style name for displaying. For example: 'product_thumb'.

'style2': Image style name for zooming. It should be larger style 1. For example: 'product_large'.

$options: Are the featuredzoomer plug-in options. For example:
$options = array(
  'imagezoomer_featured_power_range_low' => 2,
  'imagezoomer_featured_power_range_high' => 7,
  'imagezoomer_featured_magnifier_size_width' => 200,
  'imagezoomer_featured_magnifier_size_height' => 200,
  'imagezoomer_featured_magnifier_position' => 'right',
);

====================================================

[End of file]