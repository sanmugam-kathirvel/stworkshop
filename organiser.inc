<?php
	
	function workshops_add_organiser(){		
		$output = '<div class="form-item" id="academic-details"></div>';
		$output .= drupal_get_form('workshops_add_organiser_form');
		return $output;
	}
	function workshops_add_organiser_form(){
	
		$path = drupal_get_path('module', 'workshops');
		drupal_add_js($path . '/workshops.js', 'module');
		$form['#prefix'] = '<div class="workshop-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data");
		$form['uid'] = array(
	 		'#type' => 'hidden',
	 		''
	 	);
		$form['academic_code'] = array(
			'#type' => 'textfield',
			'#title' => t('Academic Center Code'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'organiser_academic_code'),
		);
		$form['email'] = array(
			'#type' => 'textfield',
			'#title' => t('Email'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'rp_user_email')
		);
		$form['user_name'] = array(
			'#type' => 'textfield',
			'#title' => t('User Name'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'rp_user_name','readonly'=>'readonly')
		);
		$form['organiser_name'] = array(
			'#type' => 'textfield',
			'#title' => t('Organiser Name'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'organiser_name')
		);
		$form['permanemt_address'] = array(
			'#type' => 'textarea',
			'#title' => t('Permanent Address'),
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
		$form['department'] = array(
			'#type' => 'textfield',
			'#title' => t('Department / School'),
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
		}else{
			$query = "select user_uid from organiser where organiser_id='".$form_state['values']['uid']."'";
			db_set_active('workshop_info');
			$result = db_query($query);
			db_set_active('default');
			$row = db_fetch_object($result);
			if($row){
				form_set_error('email', t('The e-mail address '.$form_state['values']['email'].' is already registered as Organiser'));
			}
		}if($form_state['values']['uid'] == ''){
			form_set_error('email', t('This email is not registered at spoken-tutorial.org, Please create your account with this email.'));
		}
  		return;

	}
	function workshops_add_organiser_form_submit($form, &$form_state){
		//print_r($form_state['values']);
		$query = "insert into organiser (academic_code,organiser_name,address,city,phone,department,organiser_id) values('".$form_state['values']['academic_code']."','".$form_state['values']['organiser_name']."', '".$form_state['values']['permanemt_address']."', '".$form_state['values']['city']."', '".$form_state['values']['phone']."', '".$form_state['values']['department']."', '".$form_state['values']['uid']."')";
		db_set_active('workshop_info');
		if(db_query($query)){
			drupal_set_message('Organiser Details Added Successfully');
		}else {
			drupal_set_message('Error Adding Organiser Details');
		}
		db_set_active('default');
		// get the role
		$query = "SELECT rid FROM  `role` WHERE name =  'organiser'";
		$result = db_query($query);
		$role_id = db_fetch_object($result);
		// insert into role_user
		$query = "INSERT INTO users_roles (uid, rid) VALUES ('".$form_state['values']['uid']."', '".$role_id->rid."')";
		
		if(db_query($query)){
		
			// mail to Organiser
			
			//$mailfrom = variable_get('site_mail');
			$mailto = $form_state['values']['email'];
			$subject = 'Account notification';
			$message = "<p>Dear ".$form_state['values']['organiser_name'].",</p><p>Thank you for registering at <a href='http://spoken-tutorial.org'>spoken-tutorial.org</a>. You are added as an organiser for this academic center ".$form_state['values']['academic_code'].".";
			workshops_notify($mailto, $subject, $message);
		
			// send email to administrator
		
			$mailto = variable_get('site_mail');
			//$mailfrom = $form_state['values']['email'];
			$subject = 'Request for Organiser in spoken-tutorial.org (pending admin approval)';
			$message = "<p>Dear Administrator,</p><p>".$form_state['values']['user_name']." registred as Organiser. Please 
assign role as Organiser</p>";
			//workshops_notify($mailto, $subject, $message);
			global $base_url;
			drupal_goto($path = $base_url, $query = NULL, $fragment = NULL, $http_response_code = 302);
		}else {
			drupal_set_message('Error Adding Organiser Login Details');
		}
		// echo '<br />'.$query;
		// exit;
	}
?>
