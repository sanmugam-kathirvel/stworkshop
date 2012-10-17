<?php

function workshops_add_college_form($form_state) {
	$path = drupal_get_path('module', 'workshops');
	drupal_add_js($path . '/workshops.js', 'module');
	global $user;
	$states_array = array(
		'' => 'select state',
		'ANP' => 'Andhra Pradesh',
		'ARP' => 'Arunachal Pradesh',
		'ASM' => 'Assam',
		'BHR' => 'Bihar',
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
		'ANR' => 'Andaman/Nicobar',
		'CHG' => 'Chandigarh',
	);
# get current resource person name
	$query = "select rp_name, states from resource_person where user_name='" . $user->name."'";

	db_set_active('workshop_info');

	$result = db_query($query);

	db_set_active('default');

	$row = db_fetch_object($result);

	$state_codes = explode("-", $row->states);
	$states = array();

	foreach($state_codes as $st_code){
		$states[$st_code] = $states_array[$st_code];
	}

	$form['#attributes'] = array('enctype' => "multipart/form-data");

	$form['state'] = array(
		'#type' => 'select',
		'#title' => t('State'),
		'#required' => TRUE,
		'#options' => $states_array,
		'#attributes' => array('class' => 'college_state')
	);

	$form['academic_code'] = array(
		'#type' => 'textfield',
		'#title' => t('Academic Code'),
		'#size' => 30,
		'#required' => TRUE,
		'#attributes' => array('readonly' => 'readonly', 'class' => 'college_academic_code')
	);

	$form['college_name'] = array(
		'#type' => 'textfield',
		'#title' => t('College Name'),
		'#size' => 30,
		'#required' => TRUE,
		'#attributes' => array('onChange' => 'javascript:this.value=this.value.toUpperCase();')
	);

	$form['department'] = array(
		'#type' => 'textfield',
		'#title' => t('Department'),
		'#size' => 30,
		'#required' => TRUE,
		'#attributes' => array('onChange' => 'javascript:this.value=this.value.toUpperCase();')
	);

	$form['street'] = array(
		'#type' => 'textfield',
		'#title' => t('Street'),
		'#size' => 30,
		'#required' => TRUE,
		'#attributes' => array('onChange' => 'javascript:this.value=this.value.toUpperCase();')
	);

	$form['city'] = array(
		'#type' => 'textfield',
		'#title' => t('City'),
		'#size' => 30,
		'#required' => TRUE,
		'#attributes' => array('onChange' => 'javascript:this.value=this.value.toUpperCase();')
	);

	$form['pincode'] = array(
		'#type' => 'textfield',
		'#title' => t('Pin Code'),
		'#size' => 30,
		'#required' => TRUE,
	);
	
	$form['resource_person'] = array(
		'#type' => 'textfield',
		'#title' => t('Resource Person'),
		'#size' => 30,
		'#maxlength' => 50,
		'#required' => TRUE,
		'#value' => $row->rp_name,
		'#attributes' => array('readonly' => 'readonly')
	);
	
	$form['rp_name'] = array(
		'#type' => 'hidden',
		'#value' => $user->name,
	);
	
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);
	
	return $form;
}

function workshops_add_college_form_submit($form, &$form_state){

	$query = "insert into academic_center values('', '".$form_state['values']['state']."', '".$form_state['values']['academic_code']."', '".$form_state['values']['college_name']."', '".$form_state['values']['department'];
	$query .= "', '".$form_state['values']['street']."', '".$form_state['values']['city']."', '".$form_state['values']['pincode']."', '".$form_state['values']['rp_name']."')";
	db_set_active('workshop_info');
	if(db_query($query)){
		drupal_set_message('College Details Added Successfully');
	}else {
		drupal_set_message('Error Adding College Details');
	}
	db_set_active('default');
	// echo $query;
	// exit;
}
function workshops_add_college_form_validate($form, &$form_state){
	if (!preg_match('/^[0-9\ \+]{0,6}$/', $form_state['values']['pincode'])){
		form_set_error('pincode', t('Invalid Pincode number'));
	}
  return;
}
