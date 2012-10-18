<?php
	
	function workshops_add_organiser(){
		$output = '<div class="form-item" id="academic-details"></div>';
		$output .= drupal_get_form('workshops_add_organiser_form');
		return $output;
	}
	function workshops_add_organiser_form(){
		$path = drupal_get_path('module', 'workshops');
		drupal_add_js($path . '/workshops.js', 'module');
		$form['#attributes'] = array('enctype' => "multipart/form-data");

		$form['academic_code'] = array(
			'#type' => 'textfield',
			'#title' => t('Academic Code'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'organiser_academic_code'),
		);
		$form['user_name'] = array(
			'#type' => 'textfield',
			'#title' => t('User Name'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'user_name')
		);
		$form['password'] = array(
			'#type' => 'textfield',
			'#title' => t('Password'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'password')
		);
		$form['organiser_name'] = array(
			'#type' => 'textfield',
			'#title' => t('Organiser Name'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'organiser_name')
		);
		$form['permanemt_address'] = array(
			'#type' => 'textarea',
			'#title' => t('Permanemt Address'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'permanemt_address')
		);
		$form['city'] = array(
			'#type' => 'textfield',
			'#title' => t('City'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'city')
		);
		$form['phone'] = array(
			'#type' => 'textfield',
			'#title' => t('Phone'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'phone')
		);
		$form['email'] = array(
			'#type' => 'textfield',
			'#title' => t('Email'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'email')
		);
		$form['department'] = array(
			'#type' => 'textfield',
			'#title' => t('Department'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'department')
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);
		
		return $form;
	}
	function workshops_add_organiser_form_validate($form, &$form_state){
		if (!preg_match('/^[0-9\ \+]{0,15}$/', $form_state['values']['phone'])){
			form_set_error('phone number', t('Invalid Phone number'));
		}
		if (!preg_match('/^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,3})$/', $form_state['values']['email'])){
			form_set_error('email', t('Invalid Email'));
		}
  		return;

	}
	function workshops_add_organiser_form_submit($form, &$form_state){
		print_r($form_state['values']);
		$query = "insert into  organiser values('', '".$form_state['values']['academic_code']."', '".$form_state['values']['organiser_name']."', '".$form_state['values']['permanemt_address']."', '".$form_state['values']['city']."', '".$form_state['values']['phone']."', '".$form_state['values']['email']."', '".$form_state['values']['department']."', '".$form_state['values']['user_name']."')";
		db_set_active('workshop_info');
		if(db_query($query)){
			drupal_set_message('Resource Person Details Added Successfully');
		}else {
			drupal_set_message('Error Adding Resource Person Details');
		}
		db_set_active('default');
		// echo $query;
		$query = "insert into  users values('', '".$form_state['values']['user_name']."', '".md5($form_state['values']['password'])."', '".$form_state['values']['email']."', '', '', '', '', '', '', UNIX_TIMESTAMP(), '', '', 1, '', '', '', '', '')";
		if(db_query($query)){
			drupal_set_message('Resource Person Login Details Added Successfully, User Name: '.$form_state['values']['user_name']);
		}else {
			drupal_set_message('Error Adding Resource Person Login Details');
		}
		// echo '<br />'.$query;
		// exit;
	}
?>