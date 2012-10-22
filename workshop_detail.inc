<?php
	
	function workshop_detail(){
		$output = '<div class="form-item" id="workshop-details"></div>';
		$output .= drupal_get_form('workshop_detail_form');
		return $output;
	}
	function workshop_detail_form(){
		$path = drupal_get_path('module', 'workshops');
		drupal_add_js($path . '/workshops.js', 'module');
		$form['#attributes'] = array('enctype' => "multipart/form-data");

		$form['workshop_code'] = array(
			'#type' => 'textfield',
			'#title' => t('Workshop Code'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'detail_workshop_code'),
		);
		$form['no_of_participants'] = array(
			'#type' => 'textfield',
			'#title' => t('No of Participants'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'no_of_participants'),
		);
		$form['name_of_invigilator'] = array(
			'#type' => 'textfield',
			'#title' => t('Name of invigilator'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'name_of_invigilator'),
		);
		
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);
		
		return $form;
	}
	function workshop_detail_form_validate($form, &$form_state){
		if (!preg_match('/^[0-9\ \+]{0,15}$/', $form_state['values']['no_of_participants'])){
			form_set_error('phone number', t('Invalid participants'));
		}
  		return;

	}
	function workshop_detail_form_submit($form, &$form_state){
		echo '<pre>';
		print_r($form_state);
		exit;
		$query = "insert into  workshop_details values('', '".$form_state['values']['workshop_code']."', '".$form_state['values']['no_of_participants']."', '".$form_state['values']['name_of_invigilator']."')";
		db_set_active('workshop_info');
		if(db_query($query)){
			drupal_set_message('Resource Workshop Details Added Successfully');
		}else {
			drupal_set_message('Error Adding Workshop Details');
		}
		db_set_active('default');
	}
?>