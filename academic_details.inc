<?php
function academic_details(){
	$output = drupal_get_form('academic_details_form');
	$params = substr($_REQUEST['q'], strripos($_REQUEST['q'], '/'));
	$ac = explode('=', $params);
	if($ac[1]){

		db_set_active('workshop_info');
		$query = "select * from academic_center where state_code='".$ac[1]."' order by academic_code";
		$sql_count = "select count(*) from academic_center where state_code='".$ac[1]."' order by academic_code";

		$header = array('Academic Code', 'Institution Name', 'City', '');
		db_set_active('workshop_info');
		$result = pager_query($query, 30, 0, $sql_count);
		db_set_active('default');
		$row = array();
		$count = 0;
		while ($item = db_fetch_object($result)) {
			$row[] = array($item->academic_code, $item->institution_name, $item->city, l(t('View'), 'workshops/college/view_college/'. $item->id , array('attributes' => array('class' => 'view-record', 'title' => 'View'))));
			$count++;
		}
		if($count == 0) {
			$output .= "<div class='academic-data'>";
				$output .= "<p>There is no College for this State     </p>";
			$output .= "</div>";
		}else{
			$output .= theme('table', $header, $row);
			$output .= theme('pager');
		}
	}
	return $output;
}

function academic_details_form($form_state) {
	$path = drupal_get_path('module', 'workshops');
	drupal_add_js($path . '/workshops.js', 'module');
	drupal_add_css($path . '/workshops.css', 'module');
	$form['#prefix'] = '<div class="workshop-form">';
	$form['#suffix'] = '</div>';
	$form['#attributes'] = array('enctype' => "multipart/form-data");
	$form['states'] = array(
		'#type' => 'select',
		'#title' => t('States'),
		'#required' => TRUE,
		'#attributes' => array('id' => 'resource_person_state'),
		'#options' => array(
			'' => 'select state',
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
		),

	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);
	
	return $form;
}

function academic_details_form_validate($form, &$form_state){
  return;
}

function academic_details_form_submit($form, &$form_state){
	$redirect = 'workshops/academic_details/?ac='.$form_state['values']['states'];
	drupal_goto($path = $redirect, $query=NULL, $fragment = NULL, $http_response_code = 302);
}
	
