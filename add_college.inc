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
		'HAR' => 'Haryana',
		'HMP' => 'Himachal Pradesh',
		'JNK' => 'Jammu Kashmir',
		'JHD' => 'Jharkhand',
		'KAR' => 'Karnataka',
		'KER' => 'Kerala',
		'LKD' => 'Lakshadweep',
		'MDP' => 'Madhya Pradesh',
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
		'TAM' => 'Tamil Nadu',
		'TRP' => 'Tripura',
		'UTK' => 'Uttarakhand',
		'UTP' => 'Uttar Pradesh',
		'WBN' => 'West Bengal',
		'ANR' => 'Andaman/Nicobar',
		'CHG' => 'Chandigarh',
	);
# get current resource person name
	$query = "select states from resource_person where user_uid='" . $user->uid."'";

	db_set_active('workshop_info');

	$result = db_query($query);

	db_set_active('default');

	$row = db_fetch_object($result);

	$state_codes = explode(",", $row->states);
	$states = array();
	$states[''] = "Select State";
	foreach($state_codes as $st_code){
		$states[$st_code] = $states_array[$st_code];
	}
	$form['#prefix'] = '<div class="workshop-form">';
	$form['#suffix'] = '</div>';
	$form['#attributes'] = array('enctype' => "multipart/form-data");

	$form['state'] = array(
		'#type' => 'select',
		'#title' => t('State'),
		'#required' => TRUE,
		'#options' => $states,
		'#attributes' => array('class' => 'college_state')
	);

	$form['academic_code'] = array(
		'#type' => 'textfield',
		'#title' => t('Academic Center Code'),
		'#size' => 30,
		'#required' => TRUE,
		'#attributes' => array('readonly' => 'readonly', 'class' => 'college_academic_code')
	);

	$form['college_name'] = array(
		'#type' => 'textfield',
		'#title' => t('College Name'),
		'#size' => 30,
		'#required' => TRUE,
		'#attributes' => array()
	);

	$form['street'] = array(
		'#type' => 'textarea',
		'#title' => t('Address'),
		'#size' => 30,
		'#required' => TRUE,
		'#attributes' => array( 'rows' => '4', 'cols' => '30')
	);

	$form['city'] = array(
		'#type' => 'textfield',
		'#title' => t('City'),
		'#size' => 30,
		'#required' => TRUE,
		'#attributes' => array()
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
		'#value' => $user->name,
		'#attributes' => array('readonly' => 'readonly')
	);
	
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);
	
	return $form;
}

function workshops_add_college_form_submit($form, &$form_state){
	global $user;
	$query = "insert into academic_center values('', '".$form_state['values']['state']."', '".$form_state['values']['academic_code']."', '".db_escape_string($form_state['values']['college_name']);
	$query .= "', '".db_escape_string($form_state['values']['street'])."', '".db_escape_string($form_state['values']['city'])."', '".$form_state['values']['pincode']."', ".$user->uid.")";
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
	//$query = "SELECT * FROM {academic_center} WHERE resource_person_id='". $user->name."' ORDER BY state_code, academic_code ASC";
	$query = "SELECT ac.*, rp.states FROM `academic_center` ac, resource_person rp WHERE rp.user_uid='".$user->uid."' and FIND_IN_SET(ac.state_code, rp.states) > 0";
	$count_query = "SELECT count(*) FROM `academic_center` ac, resource_person rp WHERE rp.user_uid='".$user->uid."' and FIND_IN_SET(ac.state_code, rp.states) > 0";
	$header = array('', 'State Code', 'Academic Center Code', 'Institution Name', 'City', 'Action');
	db_set_active('workshop_info');
	$result = pager_query($query , 30, 0, $count_query);
	db_set_active('default');
	$row = array();
	$sno = 1;
	
	while ($item = db_fetch_object($result)) {
		$row[] = array($sno++, $item-> state_code, $item->academic_code, $item->institution_name, $item->city, l('View', 'workshops/college/view_college/'. $item->id).", ".l('Edit', 'workshops/college/edit_college/'. $item->id));
	}
	if (count($row) == 0) {
		$output .= '<div style="color:red;text-align:center;">List is empty.</div>';
	} else {
		drupal_set_message($message);
		$output = theme('table', $header, $row);
		$output .= theme('pager');
	}
	return $output;
}

function workshops_list_organisers_detail(){
	global $user;
	global $base_url;
	//$query = "SELECT * FROM {organiser} WHERE academic_code IN (SELECT academic_code FROM academic_center WHERE resource_person_id='".$user->name."') ORDER BY academic_code ASC";
	$query = "SELECT * FROM organiser o WHERE o.academic_code IN (SELECT ac.`academic_code` FROM `academic_center` ac, resource_person rp WHERE rp.user_uid='".$user->uid."' and FIND_IN_SET(ac.state_code, rp.states) > 0) ORDER BY o.academic_code ASC";
	$count_query = "SELECT count(*) FROM organiser o WHERE o.academic_code IN (SELECT ac.`academic_code` FROM `academic_center` ac, resource_person rp WHERE rp.user_uid='".$user->uid."' and FIND_IN_SET(ac.state_code, rp.states) > 0) ORDER BY o.academic_code ASC";
	$header = array('', 'Academic Center Code', 'Name', 'Department', 'City', 'Phone', 'Action');
	db_set_active('workshop_info');
	$result = pager_query($query, 30, 0, $count_query);
	db_set_active('default');
	$row = array();
	$sno = 1;
	while ($item = db_fetch_object($result)) {
		$row[] = array($sno++, $item->academic_code, $item->organiser_name, $item->department, $item->city, $item->phone, l('Edit','workshops/edit_organiser/'.$item->id).",".l('View','workshops/view_organiser/'.$item->id));
	}
	if (count($row) == 0) {
		$output .= '<div style="color:red;text-align:center;">List is empty.</div>';
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
		'#title' => t('Academic Center Code'),
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
		'#attributes' => array(),
		'#value' => $item->institution_name,
	);

	$form['street'] = array(
		'#type' => 'textarea',
		'#title' => t('Address'),
		'#size' => 30,
		'#required' => TRUE,
		'#attributes' => array(),
		'#value' => $item->street,
	);

	$form['city'] = array(
		'#type' => 'textfield',
		'#title' => t('City'),
		'#size' => 30,
		'#required' => TRUE,
		'#attributes' => array(),
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
	$query = "update {academic_center} set state_code='".$_POST['state']."', institution_name='".db_escape_string($_POST['college_name']);
	$query .= "', street='".db_escape_string($_POST['street'])."', city='".db_escape_string($_POST['city'])."',pincode='".$_POST['pincode']."', resource_person_id='".$_POST['resource_person']."' where id=".$_POST['id'];

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
		'HAR' => 'Haryana',
		'HMP' => 'Himachal Pradesh',
		'JNK' => 'Jammu Kashmir',
		'JHD' => 'Jharkhand',
		'KAR' => 'Karnataka',
		'KER' => 'Kerala',
		'LKD' => 'Lakshadweep',
		'MDP' => 'Madhya Pradesh',
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
		'TAM' => 'Tamil Nadu',
		'TRP' => 'Tripura',
		'UTK' => 'Uttarakhand',
		'UTP' => 'Uttar Pradesh',
		'WBN' => 'West Bengal',
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
 			$output .= '<tr><td><b>Academic Center Code</b></td><td><b>'.$item->academic_code.'</b></td></tr>';
 			$output .= '<tr><td><b>Institution Name</b></td><td><b>'.$item->institution_name.'</b></td></tr>';
 			$output .= '<tr><td><b>Address</b></td><td><b>'.$item->street.'</b></td></tr>';
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

function edit_organiser($id=NULL){
	if($id){
		$output = drupal_get_form('edit_organiser_form', $id);
	}else{
		$output = "<p>Something went worong, Please try again. </p>";
	}
	return $output;
}
function edit_organiser_form($form_state, $id) {
	$query = "select organiser_name, address, city, phone, department from organiser where id='".$id."'";
	db_set_active('workshop_info');
	$result = db_query($query);
	db_set_active('default');
	$row = db_fetch_object($result);

	$form['#attributes'] = array('enctype' => "multipart/form-data");
	
	$form['organiser_name'] = array(
		'#type' => 'textfield',
		'#title' => t('Organiser Name'),
		'#required' => TRUE,
		'#attributes' => array('class' => 'organiser_name'),
		'#value' => $row->organiser_name,
	);
	$form['address'] = array(
		'#type' => 'textarea',
		'#title' => t('Address'),
		'#required' => TRUE,
		'#value' => $row->address,
	);
	$form['city'] = array(
		'#type' => 'textfield',
		'#title' => t('City'),
		'#required' => TRUE,
		'#value' => $row->city,
	);
	$form['phone'] = array(
		'#type' => 'textfield',
		'#title' => t('Phone Number'),
		'#required' => TRUE,
		'#value' => $row->phone,
	);
	$form['department'] = array(
		'#type' => 'textfield',
		'#title' => t('Department'),
		'#required' => TRUE,
		'#value' => $row->department,
	);

	$form['id'] = array(
		'#type' => 'hidden',
		'#value' => $id,
	);
	
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);
	
	return $form;
}
function edit_organiser_form_validate($form, &$form_state){
	if($_POST['phone'] == ''){
		form_set_error('phone', t('Please fill your phone number'));
		if(strlen($_POST['phone']) == 10){
			form_set_error('phone', t('phone number should be 10 digits'));
		}
	}
	if($_POST['address'] == ''){
		form_set_error('address', t('Please fill your Address'));
	}if($_POST['city'] == ''){
		form_set_error('city', t('Please fill your City'));
	}
	if($_POST['department'] == ''){
		form_set_error('department', t('Please fill your department'));
	}
	return;
}
function edit_organiser_form_submit($form, &$form_state){
	$query = "update organiser set organiser_name='".$_POST['organiser_name']."', address='".db_escape_string($_POST['address'])."', city='".$_POST['city']."', phone='".$_POST['phone']."', department='".$_POST['department']."' where id='".$_POST['id']."'";

	db_set_active('workshop_info');
	if(db_query($query)){
		db_set_active('default');
		drupal_set_message('organiser Details updated Successfully');
		drupal_goto($path = 'workshops/college/list_organisers', $query = NULL, $fragment = NULL, $http_response_code = 302);
	}else {
		drupal_set_message('Error updating organiser Details');
	}
	db_set_active('default');

	// echo $query;
	// exit;
}
	function view_organiser($id = NULL){
		if($id){
	 		$query = "select * from {organiser} where id=".$id;
	 		db_set_active('workshop_info');
	 		$result = db_query($query);
	 		db_set_active('default');

	 		if($result){
	 			$item = db_fetch_object($result);
	 			$output = '<table width="100%" cellpadding="6">';
	 			$output .= '<tr><td><b>Academic Center Code</b></td><td><b>'.$item->academic_code.'</b></td></tr>';
	 			$output .= '<tr><td><b>Organiser Name</b></td><td><b>'.$item->organiser_name.'</b></td></tr>';
	 			$output .= '<tr><td><b>Address</b></td><td><b>'.$item->address.'</b></td></tr>';
	 			$output .= '<tr><td><b>City</b></td><td><b>'.$item->city.'</b></td></tr>';
	 			$output .= '<tr><td><b>Phone</b></td><td><b>'.$item->phone.'</b></td></tr>';
	 			//$output .= '<tr><td><b>Email</b></td><td><b>'.$item->email.'</b></td></tr>';
	 			$output .= '<tr><td><b>Department</b></td><td><b>'.$item->department.'</b></td></tr>';
	 			
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
