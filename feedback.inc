<?php

	function feedback(){
		$output = '<div class="form-item" id="workshop-details"></div>';
		$output .= drupal_get_form('feedback_form');
		return $output;
	}
	function feedback_form(){
		$path = drupal_get_path('module', 'workshops');
		drupal_add_js($path . '/workshops.js', 'module');
		$form['#attributes'] = array('enctype' => "multipart/form-data");

		$form['participant_name'] = array(
			'#type' => 'textfield',
			'#title' => t('Name of the Participant'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'participant_name'),
		);
		$form['workshop_code'] = array(
			'#type' => 'textfield',
			'#title' => t('Workshop Code'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'feedback_workshop_code'),
		);

		$form['institution_name'] = array(
			'#type' => 'textfield',
			'#title' => t('Name of the Institution/College/University'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'institution_name'),
		);
		$form['email'] = array(
			'#type' => 'textfield',
			'#title' => t('Email'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'feedback_email'),
		);
		$form['email'] = array(
			'#type' => 'textfield',
			'#title' => t('Email'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'feedback_email'),
		);
		
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);
		
		return $form;
	}
	function feedback_form_validate($form, &$form_state){
		if (!preg_match('/^[0-9\ \+]{0,15}$/', $form_state['values']['no_of_participants'])){
			form_set_error('phone number', t('Invalid participants'));
		}
  		return;

	}
	function feedback_form_submit($form, &$form_state){

		$query = "insert into  workshop_details values('', '".$form_state['values']['workshop_code']."', '".$form_state['values']['no_of_participants']."', '".$form_state['values']['name_of_invigilator']."')";
		db_set_active('workshop_info');

		if(db_query($query)){
			drupal_set_message('Workshop Details Added Successfully');
		}else {
			drupal_set_message('Error Adding Workshop Details');
		}

		$query = "update {workshop_requests} set status=2 where workshop_code='".$form_state['values']['workshop_code']."'";

		if(db_query($query)){
			drupal_set_message('Workshop has been completed');
		}else {
			drupal_set_message('Error update Workshop Details');
		}
		db_set_active('default');
		
	}


?>