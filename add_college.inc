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
	drupal_goto($path = 'workshops/college', $query = NULL, $fragment = NULL, $http_response_code = 302);
	// echo $query;
	// exit;
}
function workshops_add_college_form_validate($form, &$form_state){
	if (!preg_match('/^[0-9\ \+]{0,6}$/', $form_state['values']['pincode'])){
		form_set_error('pincode', t('Invalid Pincode number'));
	}
  return;
}

function workshops_list_college_details(){
	global $user;
	$query = "SELECT * FROM {academic_center} WHERE resource_person_id='". $user->name."' ORDER BY state_code, academic_code ASC";
	$header = array('', 'State Code', 'Academic Code', 'Institution Name', 'Department', 'Action');
	$tablesort = tablesort_sql($header);
	db_set_active('workshop_info');
	$result = pager_query($query . $tablesort, 30, 0, NULL);
	db_set_active('default');
	$row = array();
	$sno = 1;
	
	while ($item = db_fetch_object($result)) {
		$row[] = array($sno++, $item-> state_code, $item->academic_code, $item->institution_name, $item->department, l('Edit', 'workshops/college/edit_college/'. $item->id));
	}
	if (count($row) == 0) {
		$output .= '<div style="color:red;text-align:center;">No video tutorial available for selected FOSS category.</div>';
	} else {
		drupal_set_message($message);
		$output = theme('table', $header, $row);
		$output .= theme('pager');
	}
	return $output;
}

function workshops_edit_college_form($form_state, $id) {
	//$path = drupal_get_path('module', 'workshops');
	//drupal_add_js($path . '/workshops.js', 'module');
	global $user;
	
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
	$query = "select * from {academic_center} WHERE id='". $id."'";
	db_set_active('workshop_info');
	$result = db_query($query);
	db_set_active('default');
	
	$item = db_fetch_object($result);
	$form['#attributes'] = array('enctype' => "multipart/form-data");

	$form['state'] = array(
		'#type' => 'textfield',
		'#title' => t('State'),
		'#required' => TRUE,
		'#size' => 30,
		'#attributes' => array('readonly' => 'readonly', 'class' => 'college_state'),
		'#value' => $item->state_code,
	);

	$form['academic_code'] = array(
		'#type' => 'textfield',
		'#title' => t('Academic Code'),
		'#size' => 30,
		'#required' => TRUE,
		'#attributes' => array('readonly' => 'readonly', 'class' => 'college_academic_code'),
		'#value' => $item->academic_code,
	);

	$form['college_name'] = array(
		'#type' => 'textfield',
		'#title' => t('College Name'),
		'#size' => 30,
		'#required' => TRUE,
		'#attributes' => array('onChange' => 'javascript:this.value=this.value.toUpperCase();'),
		'#value' => $item->institution_name,
	);

	$form['department'] = array(
		'#type' => 'textfield',
		'#title' => t('Department'),
		'#size' => 30,
		'#required' => TRUE,
		'#attributes' => array('onChange' => 'javascript:this.value=this.value.toUpperCase();'),
		'#value' => $item->department,
	);

	$form['street'] = array(
		'#type' => 'textfield',
		'#title' => t('Street'),
		'#size' => 30,
		'#required' => TRUE,
		'#attributes' => array('onChange' => 'javascript:this.value=this.value.toUpperCase();'),
		'#value' => $item->street,
	);

	$form['city'] = array(
		'#type' => 'textfield',
		'#title' => t('City'),
		'#size' => 30,
		'#required' => TRUE,
		'#attributes' => array('onChange' => 'javascript:this.value=this.value.toUpperCase();'),
		'#value' => $item->city,
	);

	$form['pincode'] = array(
		'#type' => 'textfield',
		'#title' => t('Pin Code'),
		'#size' => 30,
		'#required' => TRUE,
		'#value' => $item->pincode,
	);
	
	$form['resource_person'] = array(
		'#type' => 'textfield',
		'#title' => t('Resource Person'),
		'#size' => 30,
		'#maxlength' => 50,
		'#required' => TRUE,
		'#value' => $item->resource_person_id,
		'#attributes' => array('readonly' => 'readonly')
	);
	$form['id'] = array(
		'#type' => 'hidden',
		'#value' => $item->id,
	);
	
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);
	
	return $form;
}
function workshops_edit_college_form_submit($form, &$form_state){
	echo $_POST['state'];
	$query = "update {academic_center} set state_code='".$_POST['state']."', institution_name='".$_POST['college_name']."', institution_name='".$_POST['college_name']."', department='".$_POST['department'];
	$query .= "', street='".$_POST['street']."', city='".$_POST['city']."',pincode='".$_POST['pincode']."', resource_person_id='".$_POST['resource_person']."' where id=".$_POST['id'];

	db_set_active('workshop_info');
	if(db_query($query)){
		drupal_set_message('College Details Added Successfully');
		drupal_goto($path = 'workshops/college', $query = NULL, $fragment = NULL, $http_response_code = 302);
	}else {
		drupal_set_message('Error Adding College Details');
	}
	db_set_active('default');

	// echo $query;
	// exit;
}

function workshops_view_college_details($id = NULL){
	if($id){
	 	
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

 		$query = "select * from {academic_center} where id=".$id;
 		
 		db_set_active('workshop_info');
 		$result = db_query($query);
 		db_set_active('default');

 		if($result){
 			$item = db_fetch_object($result);
 			$output = '<table width="100%" cellpadding="6">';
 			$output .= '<tr><td><b>State Code</b></td><td><b>'.$item->state_code.'</b></td></tr>';
 			$output .= '<tr><td><b>Academic Code</b></td><td><b>'.$item->academic_code.'</b></td></tr>';
 			$output .= '<tr><td><b>Institution Name</b></td><td><b>'.$item->institution_name.'</b></td></tr>';
 			$output .= '<tr><td><b>Department</b></td><td><b>'.$item->department.'</b></td></tr>';
 			$output .= '<tr><td><b>Street</b></td><td><b>'.$item->street.'</b></td></tr>';
 			$output .= '<tr><td><b>City</b></td><td><b>'.$item->city.'</b></td></tr>';
 			$output .= '<tr><td><b>State</b></td><td><b>'.$states_array[$item->state_code].'</b></td></tr>';
 			$output .= '<tr><td><b>Pin Code</b></td><td><b>'.$item->pincode.'</b></td></tr>';
 			$output .= '<tr><td colspan="2"><hr /></td></tr>';
 			$output .= '</table>';

 			return $output;
 		}else {
 			drupal_set_message('Error accessing database workshop_info');
 		}
 	}else{
 		return "<p>Sorry, no data found.</p>";
 	}
}
?>