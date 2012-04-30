<?php
/**
 * @file
 * Module's admin forms.
 */

/**
 * General configuration form.
 *
 * @return Assoc
 *   renderable form.
 */
function _easy_breadcrumb_general_settings_form() {

  // Fieldset for grouping general settings fields.
  $fieldset_general = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $fieldset_general[EasyBreadcrumbConstants::DB_VAR_DISABLE_DEFAULT_DRUPAL_BREADCRUMB] = array(
    '#type' => 'checkbox',
    '#title' => t("Disable the default Drupal's breadcrumb"),
    '#description' => t("Always disable the default Drupal's breadcrumb."),
    '#default_value' => variable_get(EasyBreadcrumbConstants::DB_VAR_DISABLE_DEFAULT_DRUPAL_BREADCRUMB, TRUE),
  );

  $fieldset_general[EasyBreadcrumbConstants::DB_VAR_INCLUDE_HOME_SEGMENT] = array(
    '#type' => 'checkbox',
    '#title' => t("Include the front page as a segment in the breadcrumb"),
    '#description' => t("Include the front page as the first segment in the breacrumb."),
    '#default_value' => variable_get(EasyBreadcrumbConstants::DB_VAR_INCLUDE_HOME_SEGMENT, TRUE),
  );

  $fieldset_general[EasyBreadcrumbConstants::DB_VAR_INCLUDE_TITLE_SEGMENT] = array(
    '#type' => 'checkbox',
    '#title' => t("Include the current page's title as a segment in the breadcrumb"),
    '#description' => t("Include the current page's title as the last segment in the breacrumb."),
    '#default_value' => variable_get(EasyBreadcrumbConstants::DB_VAR_INCLUDE_TITLE_SEGMENT, TRUE),
  );

  $fieldset_general[EasyBreadcrumbConstants::DB_VAR_TITLE_FROM_PAGE_WHEN_AVAILABLE] = array(
    '#type' => 'checkbox',
    '#title' => t("Use the page's title when available"),
    '#description' => t("Use the page's title when it is available instead of always trying to deduce it from the URL."),
    '#default_value' => variable_get(EasyBreadcrumbConstants::DB_VAR_TITLE_FROM_PAGE_WHEN_AVAILABLE, TRUE),
  );

  $fieldset_general[EasyBreadcrumbConstants::DB_VAR_TITLE_SEGMENT_AS_LINK] = array(
    '#type' => 'checkbox',
    '#title' => t("Make the page's title segment a link"),
    '#description' => t("Prints the page's title segment as a link."),
    '#default_value' => variable_get(EasyBreadcrumbConstants::DB_VAR_TITLE_SEGMENT_AS_LINK, FALSE),
  );

  $fieldset_general[EasyBreadcrumbConstants::DB_VAR_SEGMENTS_SEPARATOR] = array(
    '#type' => 'textfield',
    '#title' => t('Segments separator'),
    '#description' => t("Separator to be used between the breadcrumb's segments."),
    '#default_value' => variable_get(EasyBreadcrumbConstants::DB_VAR_SEGMENTS_SEPARATOR, '>>'),
  );

  $fieldset_general[EasyBreadcrumbConstants::DB_VAR_CAPITALIZATOR_MODE] = array(
    '#type' => 'select',
    '#title' => t("Segments title's transformation mode"),
    '#options' => array(
      'none' => t('None'),
      'ucwords' => t("Capitalize the first letter of each word in the segment"),
      'ucfirst' => t("Only capitalize the first letter of the whole segment"),
    ),
    '#description' => t("Choose the transformation mode you want to apply to the segments
      (E.g.: 'blog/once-a-time' -> 'Home >> Blog >> Once a Time')."),
    '#default_value' => variable_get(EasyBreadcrumbConstants::DB_VAR_CAPITALIZATOR_MODE, 'ucwords'),
  );

  // Formats the ignored-words array as space separated list of words
  // (word1 word2 wordN) before display them.
  $capitalizator_ignored_words_arr = variable_get(EasyBreadcrumbConstants::DB_VAR_CAPITALIZATOR_IGNORED_WORDS,
    EasyBreadcrumbConstants::defaultIgnoredWords());
  $capitalizator_ignored_words = @join(' ', $capitalizator_ignored_words_arr);

  $fieldset_general[EasyBreadcrumbConstants::DB_VAR_CAPITALIZATOR_IGNORED_WORDS] = array(
    '#type' => 'textarea',
    '#title' => t("Words to be ignored by the 'capitalizator'"),
    '#description' => t("Enter a space separated list of words to be ignored by the 'capitalizator'. E.g.: of and.!br
      Note: this will be applyed only to words not at the beginning of each segment.", array('!br' => '<br/>')),
    '#default_value' => $capitalizator_ignored_words,
  );

  $form = array();

  // Inserts the fieldset for grouping general settings fields.
  $form[EasyBreadcrumbConstants::MODULE_NAME] = $fieldset_general;

  // Adds buttons for processing the form.
  $form['buttons'] = array(
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save'),
    ),
  );

  // Specifies the callback function for processing the form submission.
  $form['#submit'] = array('_easy_breadcrumb_admin_submit');

  // Specifies the theme for the form.
  $form['#theme'] = 'system_settings_form';

  return $form;
}

/**
 * Process the submitting of the administration form of this module.
 *
 * @param Assoc $form
 *   renderable form.
 * @param Assoc $form_state
 *   form state.
 */
function _easy_breadcrumb_admin_submit($form, &$form_state) {

  // Pre-processes the list of ignored words for storing them as an array.
  // E.g.: array('of','and').
  $ignored_words = $form_state['values'][EasyBreadcrumbConstants::DB_VAR_CAPITALIZATOR_IGNORED_WORDS];
  $ignored_words_arr = array();
  $ignored_words_arr_aux = explode(' ', $ignored_words);
  foreach ($ignored_words_arr_aux as $word) {
    $word = trim($word);
    if ($word !== '') {
      $ignored_words_arr[$word] = $word;
    }
  }

  // Updates the $form_state before passing it to the Drupal system.
  $form_state['values'][EasyBreadcrumbConstants::DB_VAR_CAPITALIZATOR_IGNORED_WORDS] = $ignored_words_arr;

  // Delegates persistence to the existing Drupal system.
  system_settings_form_submit($form, $form_state);
}