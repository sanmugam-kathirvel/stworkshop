<?php
	
	function workshops_workshop_request(){
		$output = '<div class="form-item" id="academic-details"></div>';
		$output .= drupal_get_form('workshops_workshop_request_form');
		return $output;
	}
	function workshops_workshop_request_form(){
		$hour = array('' => '');
		$minite = array('' => '');
		$foss_category = array('' => '');
		$language_array = array('' => '');
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


		//$rows = db_fetch_object($foss_categories);

		while($row = db_fetch_object($foss_categories)) {
			$foss_category[$row->FOSS_Category] = $row->FOSS_Category;
		}
		while($row = db_fetch_object($languages)) {
			$language_array[$row->Language] = $row->Language;
		}

		//print_r($language_array);
		//exit;

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
		$form['pref_wkshop_date'] = array(
			'#type' => 'date',
			'#title' => t('Preferred Workshop Date'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'pref_wkshop_date')
		);
		$form['pref_wkshop_time'] = array(
			'#type' => 'select',
			'#title' => t('Preferred Workshop Time'),
			'#required' => TRUE,
			'#options' => $hour,
			'#attributes' => array('class' => 'pref_wkshop_time')
		);
		$form['pref_wkshop_minute'] = array(
			'#type' => 'select',
			'#required' => TRUE,
			'#options' => $minite,
			'#attributes' => array('class' => 'pref_wkshop_minute')
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
	function workshops_workshop_request_form_validate($form, &$form_state){
		if (!preg_match('/^[0-9\ \+]{0,15}$/', $form_state['values']['phone'])){
			form_set_error('phone number', t('Invalid Phone number'));
		}
		if (!preg_match('/^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,3})$/', $form_state['values']['email'])){
			form_set_error('email', t('Invalid Email'));
		}
  		return;

	}
	function workshops_workshop_request_form_submit($form, &$form_state){
		print_r($form_state['values']);
		$pref_wkshop_date = $form_state['values']['pref_wkshop_date']['year'].'-'.$form_state['values']['pref_wkshop_date']['month'].'-'.$form_state['values']['pref_wkshop_date']['day'];
		$pref_wkshop_time = $form_state['values']['pref_wkshop_time'].':'.$form_state['values']['pref_wkshop_minute'];
		$query = "insert into  workshop_requests values('','', '".$form_state['values']['academic_code']."', '".$form_state['values']['department']."', '".$pref_wkshop_date."','', '".$pref_wkshop_time."', '', '".$form_state['values']['foss_category']."', '".$form_state['values']['pref_language']."',''".")";
		db_set_active('workshop_info');
		if(db_query($query)){
			drupal_set_message('Workshop Request Details Added Successfully');
		}else {
			drupal_set_message('Error Adding Workshop Request Details');
		}
		db_set_active('default');
	}

	function workshops_list_workshop_request(){
		$query = "SELECT * FROM {workshop_requests} WHERE status!=1 ORDER BY foss_category, pref_language ASC";
		$header = array('', 'FOSS Category', 'Language', 'Institution Name', 'Department', 'Preferred date & time', 'Action');
		$tablesort = tablesort_sql($header);
		db_set_active('workshop_info');
		$result = pager_query($query . $tablesort, 30, 0, NULL);
		db_set_active('default');
		$row = array();
		$sno = 1;
		
		while ($item = db_fetch_object($result)) {
			$row[] = array($sno++, $item->foss_category, $item->pref_language, $item->academic_code, $item->department, $item->pref_wkshop_date.":".$item->pref_wkshop_time, l('Confirm', 'workshops/workshop_confirm/'. $item->id));
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

	function workshops_workshop_confirm($id = NULL){
		if($id){
			$query = "select * from {workshop_requests} WHERE id='". $id."'";
			db_set_active('workshop_info');
			$result = db_query($query);
			$item = db_fetch_object($result);

			//$query = "select * from {academic_center} WHERE academic_code='". $item->academic_code."'";

			//$result = db_query($query);
			//$academic_item = db_fetch_object($result);
			db_set_active('default');
			$output = '<div class="form-item" id="academic-details"><table>';
			$output .= '<tr><td>Academic Code</td><td>: '.$item->academic_code.'</td></tr>';
			$output .= '<tr><td>Language</td><td>: '.$item->pref_language.'</td></tr>';
			$output .= '<tr><td>FOSS Category</td><td>: '.$item->foss_category.'</td></tr>';
			$output .= '<tr><td>Preferred Date & Time</td><td>: '.$item->pref_wkshop_date.' '.$item->pref_wkshop_time.'</td></tr>';
			$output .= '</table></div>';
			$output .= drupal_get_form('workshops_workshop_confirm_form');
			return $output;
		}else{
			return "<p>Sorry, no data found.</p>";
		}
	}

	function workshops_workshop_confirm_form(){
		$hour = array('' => '');
		$minite = array('' => '');
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
			'#title' => t('Preferred Workshop Date'),
			'#value' => $item->id,
		);
		$form['cfm_wkshop_date'] = array(
			'#type' => 'date',
			'#title' => t('Confirm Workshop Date'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'cfm_wkshop_date')
		);
		$form['cfm_wkshop_time'] = array(
			'#type' => 'select',
			'#title' => t('Confirm Workshop Time'),
			'#required' => TRUE,
			'#options' => $hour,
			'#attributes' => array('class' => 'cfm_wkshop_time')
		);
		$form['cfm_wkshop_minute'] = array(
			'#type' => 'select',
			'#required' => TRUE,
			'#options' => $minite,
			'#attributes' => array('class' => 'cfm_wkshop_minute')
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);
		return $form;
	}
	function workshops_workshop_confirm_form_validate($form, &$form_state){
		// if (!preg_match('/^[0-9\ \+]{0,15}$/', $form_state['values']['phone'])){
		// 	form_set_error('phone number', t('Invalid Phone number'));
		// }
		// if (!preg_match('/^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,3})$/', $form_state['values']['email'])){
		// 	form_set_error('email', t('Invalid Email'));
		// }
  		return;

	}
	function workshops_workshop_confirm_form_submit($form, &$form_state){
		print_r($form_state['values']);
		$cfm_wkshop_date = $form_state['values']['cfm_wkshop_date']['year'].'-'.$form_state['values']['cfm_wkshop_date']['month'].'-'.$form_state['values']['cfm_wkshop_date']['day'];
		$cfm_wkshop_time = $form_state['values']['cfm_wkshop_time'].':'.$form_state['values']['cfm_wkshop_minute'];

		$transition = array(
				'id' => $form_state['values']['id'],
				'cfm_wkshop_date' => $cfm_wkshop_date,
				'cfm_wkshop_time' => $cfm_wkshop_time
			);
		//$query = "insert into  workshop_requests values('','', '".$form_state['values']['academic_code']."', '".$form_state['values']['department']."', '".$pref_wkshop_date."','', '".$pref_wkshop_time."', '', '".$form_state['values']['foss_category']."', '".$form_state['values']['pref_language']."',''".")";
		db_set_active('workshop_info');
		//if(db_query($query)){
		if(drupal_write_record('workshop_requests', $transition, 'id')){
			drupal_set_message('Workshop Request Details Added Successfully');
		}else {
			drupal_set_message('Error Adding Workshop Request Details');
		}
		db_set_active('default');
	}
?>