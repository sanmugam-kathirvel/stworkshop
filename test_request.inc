<?php
	
	function workshops_test_request(){
		$output = '<div class="form-item" id="academic-details"></div>';
		$output .= drupal_get_form('workshops_test_request_form');
		return $output;
	}
	function workshops_test_request_form(){
		$hour = array('' => 'Hour');
		$minite = array('' => 'Minute');
		$foss_category = array('' => 'Select FOSS Category');
		$language_array = array('' => 'Select Language');
		$i=0;
		while($i <= 23){
			if($i < 10){
				$hour['0'.$i] = '0'.$i;
			}else{
				$hour[$i] = $i;
			}
			$i++;
		}
		$i=0;
		while($i <= 59){
			if($i < 10){
				$minite['0'.$i] = '0'.$i;
			}else{
				$minite[$i] = $i;
			}
			$i++;
		}

		$query = "select DISTINCT FOSS_Category from stcontentlist";
		db_set_active('stcontent');
		$foss_categories = db_query($query);
		
		$query = "select DISTINCT Language from stcontentlist";
		$languages = db_query($query);

		db_set_active('default');


		$rows = db_fetch_object($foss_categories);

		while($row = db_fetch_object($foss_categories)) {
			$foss_category[$row->FOSS_Category] = $row->FOSS_Category;
		}
		while($row = db_fetch_object($languages)) {
			$language_array[$row->Language] = $row->Language;
		}

		$path = drupal_get_path('module', 'workshops');
		drupal_add_js($path . '/workshops.js', 'module');
		$form['#attributes'] = array('enctype' => "multipart/form-data");

		$form['academic_code'] = array(
			'#type' => 'textfield',
			'#title' => t('Academic Code'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'organiser_academic_code'),
		);
		$form['department'] = array(
			'#type' => 'textfield',
			'#title' => t('Department'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'department')
		);
		$form['pref_test_date'] = array(
			'#type' => 'textfield',
			'#title' => t('Preferred Test Date'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'pref_test_date')
		);
		$form['pref_test_time'] = array(
			'#type' => 'select',
			'#title' => t('Preferred Test Time'),
			'#required' => TRUE,
			'#options' => $hour,
			'#attributes' => array('class' => 'pref_test_time')
		);
		$form['pref_test_minute'] = array(
			'#type' => 'select',
			'#required' => TRUE,
			'#options' => $minite,
			'#attributes' => array('class' => 'pref_test_minute')
		);
		$form['foss_category'] = array(
			'#type' => 'select',
			'#title' => t('Fosss Category'),
			'#options' => $foss_category,
			'#required' => TRUE,
			'#attributes' => array('class' => 'foss_category')
		);
		$form['pref_language'] = array(
			'#type' => 'select',
			'#title' => t('Preferred Language'),
			'#options' => $language_array,
			'#required' => TRUE,
			'#attributes' => array('class' => 'pref_language')
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
	function workshops_test_request_form_validate($form, &$form_state){
		if (!preg_match('/^[0-9\ \+]{0,15}$/', $form_state['values']['phone'])){
			form_set_error('phone number', t('Invalid Phone number'));
		}
		if (!preg_match('/^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,3})$/', $form_state['values']['email'])){
			form_set_error('email', t('Invalid Email'));
		}
  		return;

	}
	function workshops_test_request_form_submit($form, &$form_state){
		$pref_test_date = $form_state['values']['pref_test_date'];
		$pref_test_time = $form_state['values']['pref_test_time'].':'.$form_state['values']['pref_test_minute'];
		$query = "insert into  test_requests values('','', '".$form_state['values']['academic_code']."', '".$form_state['values']['department']."', '".$pref_test_date."','', '".$pref_test_time."', '', '".$form_state['values']['foss_category']."', '".$form_state['values']['pref_language']."',''".")";
		db_set_active('workshop_info');
		if(db_query($query)){
			drupal_set_message('Test Request Details Added Successfully');
		}else {
			drupal_set_message('Error Adding Test Request Details');
		}
		db_set_active('default');
		drupal_goto($path = 'workshops/list_test_request', $query = NULL, $fragment = NULL, $http_response_code = 302);
	}

	function workshops_list_test_request(){
		global $user;
		$query = "select * from {test_requests} where academic_code in (select academic_code from {academic_center} where resource_person_id='".$user->name."') and status=0";
		 $query = "SELECT * FROM {test_requests} WHERE status=0 ORDER BY foss_category, pref_language ASC";
		$header = array('', 'FOSS Category', 'Language', 'Institution Name', 'Department', 'Preferred date & time', 'Action');
		$tablesort = tablesort_sql($header);
		db_set_active('workshop_info');
		$result = pager_query($query . $tablesort, 30, 0, NULL);
		db_set_active('default');
		$row = array();
		$sno = 1;
		
		while ($item = db_fetch_object($result)) {
			$row[] = array($sno++, $item->foss_category, $item->pref_language, $item->academic_code, $item->department, $item->pref_test_date." ".$item->pref_test_time, l('Confirm', 'workshops/test_confirm/'. $item->id));
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

	function workshops_list_test_approval(){
		global $user;
		$query = "select * from {test_requests} where academic_code in (select academic_code from {academic_center} where resource_person_id='".$user->name."') and status=1";
		 $query = "SELECT * FROM {test_requests} WHERE status=1 ORDER BY foss_category, pref_language ASC";
		$header = array('', 'FOSS Category', 'Language', 'Institution Name', 'Department', 'Confirmed date & time', 'Action');
		$tablesort = tablesort_sql($header);
		db_set_active('workshop_info');
		$result = pager_query($query . $tablesort, 30, 0, NULL);
		db_set_active('default');
		$row = array();
		$sno = 1;
		
		while ($item = db_fetch_object($result)) {
			$row[] = array($sno++, $item->foss_category, $item->pref_language, $item->academic_code, $item->department, $item->cfm_test_date." ".$item->cfm_test_time, l('View', 'workshops/test_view/'. $item->id));
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

	function workshops_test_confirm($id = NULL){
		if($id){
			$query = "select * from {test_requests} WHERE id='". $id."'";
			db_set_active('workshop_info');
			$result = db_query($query);
			$item = db_fetch_object($result);

			$query = "select * from {academic_center} WHERE academic_code='". $item->academic_code."'";

			$result = db_query($query);
			$academic_item = db_fetch_object($result);
			db_set_active('default');
			$output = '<div class="form-item" id="academic-details"><table>';
			$output .= '<tr><td>Academic Code</td><td>: '.$item->academic_code.'</td></tr>';
			$output .= '<tr><td>Language</td><td>: '.$item->pref_language.'</td></tr>';
			$output .= '<tr><td>FOSS Category</td><td>: '.$item->foss_category.'</td></tr>';
			$output .= '<tr><td>Preferred Date & Time</td><td>: '.$item->pref_test_date.' '.$item->pref_test_time.'</td></tr>';
			$output .= '</table></div>';
			$output .= drupal_get_form('workshops_test_confirm_form', $id, $item->pref_test_date, $item->pref_test_time);
			return $output;
		}else{
			return "<p>Sorry, no data found.</p>";
		}
	}

	function workshops_test_confirm_form($form_state, $id, $date_val, $time_val){
		$time_arr = explode(':', $time_val);
		$hour = array();
		$minite = array();
		$i=0;
		while($i <= 23){
			if($i < 10){
				$hour['0'.$i] = '0'.$i;
			}else{
				$hour[$i] = $i;
			}
			$i++;
		}
		$i=0;
		while($i <= 59){
			if($i < 10){
				$minite['0'.$i] = '0'.$i;
			}else{
				$minite[$i] = $i;
			}
			$i++;
		}
		$form['#attributes'] = array('enctype' => "multipart/form-data");
		$form['id'] = array(
			'#type' => 'hidden',
			'#title' => t('Preferred Test Date'),
			'#value' => $id,
		);
		$form['cfm_test_date'] = array(
			'#type' => 'textfield',
			'#title' => t('Confirm Test Date'),
			'#required' => TRUE,
			'#value' => $date_val,
			'#attributes' => array('class' => 'cfm_test_date')
		);
		$form['cfm_test_time'] = array(
			'#type' => 'select',
			'#title' => t('Confirm Test Time'),
			'#required' => TRUE,
			'#options' => $hour,
			'#default_value' => $time_arr[0],
			'#attributes' => array('class' => 'cfm_test_time')
		);
		$form['cfm_test_minute'] = array(
			'#type' => 'select',
			'#required' => TRUE,
			'#options' => $minite,
			'#default_value' => $time_arr[1],
			'#attributes' => array('class' => 'cfm_test_minute')
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);
		return $form;
	}
	function workshops_test_confirm_form_validate($form, &$form_state){
		 // if (!preg_match('/^[0-9\ \+]{0,15}$/', $form_state['values']['phone'])){
		 // 	form_set_error('phone number', t('Invalid Phone number'));
		 // }
		 // if (!preg_match('/^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,3})$/', $form_state['values']['email'])){
		 // 	form_set_error('email', t('Invalid Email'));
		 // }
  		return;

	}
	function workshops_test_confirm_form_submit($form, &$form_state){
		print_r($form_state['values']);
		$cfm_test_date = $form_state['values']['cfm_test_date'];
		$cfm_test_time = $form_state['values']['cfm_test_time'].':'.$form_state['values']['cfm_test_minute'].':00';
		$test_code = 'TC-'.$form_state['values']['id'];

		$query = "update {test_requests} set test_code='".$test_code."', cfm_test_date='".$cfm_test_date."', cfm_test_time='".$cfm_test_time."', status=1 where id=".$form_state['values']['id'];
		db_set_active('workshop_info');
		if(db_query($query)){
			drupal_set_message('Test Confirm Details Added Successfully');
				drupal_goto($path = 'workshops/list_test_approval', $query = NULL, $fragment = NULL, $http_response_code = 302);
		}else {
			drupal_set_message('Error Adding Test Confirm Details');
		}
		db_set_active('default');
	}
	function workshops_test_view($id = NULL){
	 	if($id){
	 		$query = "select * from {test_requests} where id=".$id;
	 		// echo $query;
	 		// exit;
	 		db_set_active('workshop_info');
	 		$result = db_query($query);
	 		db_set_active('default');
	 		if($result){
	 			$item = db_fetch_object($result);
	 			$output = '<table width="100%" cellpadding="6">';
	 			$output .= '<tr><td><b>Test Code</b></td><td><b>'.$item->test_code.'</b></td></tr>';
	 			$output .= '<tr><td><b>Foss Category</b></td><td><b>'.$item->foss_category.'</b></td></tr>';
	 			$output .= '<tr><td><b>Language</b></td><td><b>'.$item->pref_language.'</b></td></tr>';
	 			$output .= '<tr><td><b>Date & Time</b></td><td><b>'.$item->cfm_test_date.' &nbsp;'.$item->cfm_test_time.'</b></td></tr>';
	 			$output .= '<tr><td colspan="2"><hr /></td></tr>';

	 			$query = "select * from {academic_center} where academic_code='".$item->academic_code."'";
	 			db_set_active('workshop_info');
	 			$result1 = db_query($query);
	 			db_set_active('default');
	 			$coll_detail = db_fetch_object($result1);

	 			$output .= '<tr><td><b>State Code</b></td><td><b>'.$coll_detail->state_code.'</b></td></tr>';
	 			$output .= '<tr><td><b>Academic Code</b></td><td><b>'.$coll_detail->academic_code.'</b></td></tr>';
	 			$output .= '<tr><td><b>Institution Name</b></td><td><b>'.$coll_detail->institution_name.'</b></td></tr>';
	 			$output .= '<tr><td><b>Department</b></td><td><b>'.$item->department.'</b></td></tr>';
	 			$output .= '<tr><td><b>Street</b></td><td><b>'.$coll_detail->street.'</b></td></tr>';
	 			$output .= '<tr><td><b>City</b></td><td><b>'.$coll_detail->city.'</b></td></tr>';
	 			$output .= '<tr><td><b>Pin Code</b></td><td><b>'.$coll_detail->pincode.'</b></td></tr>';
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
