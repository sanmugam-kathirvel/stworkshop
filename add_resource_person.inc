<?php

function workshops_add_resource_person_form($form_state) {
	$path = drupal_get_path('module', 'workshops');
	drupal_add_js($path . '/workshops.js', 'module');
	$form['#attributes'] = array('enctype' => "multipart/form-data");

	$form['rp_name'] = array(
		'#type' => 'textfield',
		'#title' => t('Name'),
		'#size' => 30,
		'#maxlength' => 50,
		'#required' => TRUE,
	);
 	
	$form['email'] = array(
		'#type' => 'textfield',
		'#title' => t('E-mail'),
		'#size' => 30,
		'#required' => TRUE,
	);
	
	$form['phone'] = array(
		'#type' => 'textfield',
		'#title' => t('Phone Number'),
		'#size' => 30,
		'#maxlength' => 15,
		'#required' => TRUE,
	);

	$form['username'] = array(
		'#type' => 'textfield',
		'#title' => t('User Name'),
		'#size' => 30,
		'#maxlength' => 15,
		'#required' => TRUE,
	);

	$form['password'] = array(
		'#type' => 'password',
		'#title' => t('Password'),
		'#size' => 30,
		'#required' => TRUE,
	);

	$form['states'] = array(
		'#type' => 'select',
		'#multiple' => TRUE,
		'#title' => t('States'),
		'#required' => TRUE,
		'#attributes' => array('class' => 'resource_person_state'),
		'#options' => array(
			'ANP' => 'Andhra Pradesh',
			'ANR' => 'Andaman/Nicobar',
			'ARP' => 'Arunachal Pradesh',
			'ASM' => 'Assam',
			'BHR' => 'Bihar',
			'CHG' => 'Chandigarh',
			'CTG' => 'Chattisgarh',
			'DDU' => 'Daman/Diu',
			'DEL' => 'Delhi',
			'DNG' => 'Dadar/Nag',
			'GOA' => 'Goa',
			'GUJ' => 'Gujarat',
			'HAR' => 'Hariyana',
			'HMP' => 'HimachalPradesh',
			'JNK' => 'JammuKashmir',
			'JHD' => 'Jharkhand',
			'KAR' => 'Karnataka',
			'KER' => 'Kerela',
			'LKD' => 'Lakshadweep',
			'MDP' => 'MadhyaPradesh',
			'MAH' => 'Maharastra',
			'MAN' => 'Manipur',
			'MEG' => 'Meghalaya',
			'MIZ' => 'Mizoram',
			'NAG' => 'Nagaland',
			'ODI' => 'Orissa',
			'PCY' => 'Pondicherry',
			'PJB' => 'Punjab',
			'RAJ' => 'Rajasthan',
			'SIK' => 'Sikkim',
			'TAM' => 'TamilNadu',
			'TRP' => 'Tripura',
			'UTK' => 'UttarKhand',
			'UTP' => 'UttarPradesh',
			'WBN' => 'WestBengal',
		),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);
	
	return $form;
}

function workshops_add_resource_person_form_validate($form, &$form_state){
	if (!preg_match('/^[0-9\ \+]{0,15}$/', $form_state['values']['phone'])){
		form_set_error('phone', t('Invalid phone number'));
	}
	if (!preg_match('/^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,3})$/', $form_state['values']['email'])){
		form_set_error('email', t('Invalid Email'));
	}
  return;
}

function workshops_add_resource_person_form_submit($form, &$form_state){
	// echo '<pre>';
	print_r($form_state['values']);
	$selected_states = '';
	foreach($form_state['values']['states'] as $sel_state){
		if($selected_states != '')
			$selected_states .= "-";
		$selected_states .= $sel_state;
	}
	$query = "insert into  resource_person values('', '".$form_state['values']['username']."', '".$form_state['values']['rp_name']."', '".$form_state['values']['email']."', '".$form_state['values']['phone']."', '".$selected_states."')";
	db_set_active('workshop_info');
	if(db_query($query)){
		drupal_set_message('Resource Person Details Added Successfully');
	}else {
		drupal_set_message('Error Adding Resource Person Details');
	}
	db_set_active('default');
	// echo $query;
	$query = "insert into  users values('', '".$form_state['values']['username']."', '".md5($form_state['values']['password'])."', '".$form_state['values']['email']."', '', '', '', '', '', '', UNIX_TIMESTAMP(), '', '', 1, '', '', '', '', '')";
	if(db_query($query)){
		drupal_set_message('Resource Person Login Details Added Successfully, User Name: '.$form_state['values']['username']);
	}else {
		drupal_set_message('Error Adding Resource Person Login Details');
	}
	// echo '<br />'.$query;
	// exit;
}
	