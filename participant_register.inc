<?php
	
	function participant_register(){
		$output = '<div class="form-item" id="academic-details"></div>';
		$output .= drupal_get_form('participant_register_form');
		return $output;
	}
	function participant_register_form(){
		$path = drupal_get_path('module', 'workshops');
		drupal_add_js($path . '/workshops.js', 'module');
		$form['#attributes'] = array('enctype' => "multipart/form-data");

		$form['academic_code'] = array(
			'#type' => 'textfield',
			'#title' => t('Academic Code'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'organiser_academic_code'),
		);
		$form['name_of_participants'] = array(
			'#type' => 'textfield',
			'#title' => t('Name of Participants'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'name_of_participants'),
		);
		$form['permanent_address'] = array(
			'#type' => 'textarea',
			'#title' => t('Permanent Address'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'permanent_address'),
		);
		$form['phone'] = array(
			'#type' => 'textfield',
			'#title' => t('Phone'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'phone'),
		);
		$form['email'] = array(
			'#type' => 'textfield',
			'#title' => t('Email'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'email'),
		);		
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);
		
		return $form;
	}
	function participant_register_form_validate($form, &$form_state){
		if (!preg_match('/^[0-9\ \+]{0,15}$/', $form_state['values']['phone'])){
			form_set_error('phone number', t('Invalid phone number'));
		}
		if (!preg_match('/^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,3})$/', $form_state['values']['email'])){
			form_set_error('email', t('Invalid Email'));
		}
  		return;

	}
	function participant_register_form_submit($form, &$form_state){
		//echo '<pre>';
		//print_r($form_state);
		$username = $form_state['values']['name_of_participants'].date("his");
		$password = md5($username);
		$query = "insert into  {mdl_user} (confirmed, mnethostid, username, password, firstname, email, institution) values(1, 1,'".$username."', '".$password."', '".$form_state['values']['name_of_participants']."', '".$form_state['values']['email']."', '".$form_state['values']['academic_code']."')";
		db_set_active('moodle');
		if(db_query($query)){
			drupal_set_message('Please, check your email for login Details.');
		}else {
			drupal_set_message('Error Adding Workshop Details.');
		}
		$query = "insert into  {users} (name, pass, mail, created, status) values('".$username."', '".$password."', '".$form_state['values']['email']."', UNIX_TIMESTAMP(), 1)";
		db_set_active('default');

		if(db_query($query)){
			drupal_set_message('Please, check your email for login Details.');
		}else {
			drupal_set_message('Sorry, somthing went wrong, please try again.');
		}
	}
?>