<?php

/**
 * Include hook files.
 * All of the processing runs through this file, so we include these files once
 * here.
 */
$inc = DRUPAL_ROOT . '/' . drupal_get_path('theme', 'base') . '/inc/';

require_once $inc . 'alter.inc';
require_once $inc . 'preprocess.inc';
require_once $inc . 'process.inc';
require_once $inc . 'theme.inc';
