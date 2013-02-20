<?php
	
	function workshops_workshop_request(){
  $path = drupal_get_path('module', 'workshops');
		drupal_add_js($path . '/datepickr.js');
		drupal_add_css($path . '/datepickr.css');
		drupal_add_js($path . '/workshops.js');
  drupal_add_js("$(document).ready(function(){ new datepickr('edit-pref-wkshop-date', { dateFormat: 'Y-m-d' }); });", 'inline');
		$output = '<div class="form-item" id="academic-details"></div>';
		$output .= drupal_get_form('workshops_workshop_request_form');
		return $output;
	}
	function workshops_workshop_request_form(){
			
		$hour = array('' => 'Hour');
		$minite = array('' => 'Minute');
		$foss_category = array('' => '-- Select --');
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

		$query = "select name from foss_categories";
		$foss_categories = db_query($query);

		while($row = db_fetch_object($foss_categories)) {
			$foss_category[$row->name] = $row->name;
		}

		$path = drupal_get_path('module', 'workshops');
		drupal_add_js($path . '/workshops.js', 'module');
		drupal_add_css($path . '/workshops.css', 'module');


		$form['#prefix'] = '<div class="workshop-form">';
		$form['#suffix'] = '</div>';		
		$form['#attributes'] = array('enctype' => "multipart/form-data");

		$form['academic_code'] = array(
			'#type' => 'textfield',
			'#title' => t('Academic Center Code'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'organiser_academic_code'),
		);
		$form['department'] = array(
			'#type' => 'textfield',
			'#title' => t('Department of Participants'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'department'),
		);
		$form['pref_wkshop_date'] = array(
			'#type' => 'textfield',
			'#title' => t('Preferred Workshop Date'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'pref_wkshop_date', 'readonly' => 'readonly')
		);
		$form['pref_wkshop_time'] = array(
			'#type' => 'select',
			'#required' => TRUE,
			'#options' => $hour,
			'#attributes' => array('class' => 'pref_wkshop_time'),
			'#prefix' => "<div class='date-and-time'><label for='edit-pref-wkshop-time'><b>Preferred Workshop Time: </b><span class='form-required' title='This field is required.'>*</span></label>",
		);
		$form['pref_wkshop_minute'] = array(
			'#type' => 'select',
			'#required' => TRUE,
			'#options' => $minite,
			'#attributes' => array('class' => 'pref_wkshop_minute'),
			'#suffix' => "</div>"
		);
		$form['foss_category'] = array(
			'#type' => 'select',
			'#title' => t('Foss Category'),
			'#options' => $foss_category,
			'#required' => TRUE,
			'#attributes' => array('class' => 'foss_category'),
			'#prefix' => '<div class="ws-foss-lang"><div class="ws-foss">',
			'#suffix' => '</div>'
		);
		$form['pref_language'] = array(
			'#type' => 'select',
			'#title' => t('Preferred Language'),
			'#options' => array('' => '-- Select --'),
			'#required' => TRUE,
			'#validated' => TRUE,
			'#attributes' => array('class' => 'pref_language'),
			'#prefix' => '<div class="ws-lang">',
			'#suffix' => '</div></div>'
		);
		$form['skype_request'] = array(
			'#type' => 'radios',
			'#title' => t('Skype Support'),
			'#options' => array(1 => 'Yes', 0 => 'No'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'skype_request'),
			'#prefix' => '<div class="skype-request">',
			'#suffix' => '</div>'
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);
		
		return $form;
	}
	function workshops_workshop_request_form_validate($form, &$form_state){
		if (!preg_match('/^[a-zA-Z ]+$/', $form_state['values']['department'])){
			form_set_error('department', t('Invalid Department'));
		}
		if($form_state['values']['pref_language'] == ""){
                        form_set_error('pref_language', t('Please select language'));
                }

	  return;
	}
	function workshops_workshop_request_form_submit($form, &$form_state){
		global $user;
		global $base_url;
		$pref_wkshop_date = $form_state['values']['pref_wkshop_date'];
		$pref_wkshop_time = $form_state['values']['pref_wkshop_time'].':'.$form_state['values']['pref_wkshop_minute'];
		$query = "insert into  workshop_requests(academic_code, organiser_id, department, pref_wkshop_date, pref_wkshop_time, foss_category, pref_language, skype_request) values('".$form_state['values']['academic_code']."', '".$user->uid."', '".$form_state['values']['department']."', '".$pref_wkshop_date."', '".$pref_wkshop_time."', '".$form_state['values']['foss_category']."', '".$form_state['values']['pref_language']."','".$form_state['values']['skype_request']."'".")";
		db_set_active('workshop_info');
		if(db_query($query)){
			// mail to Organiser
			//$mailfrom = $custom_email;
			$mailto = $user->mail;
			$subject = 'Workshop Request Notification';
			$message = "<p>Dear ".$user->name.",</p><p>Thank you for registering at <a href='http://spoken-tutorial.org'>spoken-tutorial.org</a>.Your workshop request is waiting for approval. Once it has been approved, you will receive another e-mail containing information about workshop confirmation details.</p>";
			workshops_notify($mailto, $subject, $message);
			
			
			// send email to Resource person
			$tem = explode('-', $form_state['values']['academic_code']);
			$query = "select rp.rp_fname, u.mail from workshop_info.resource_person rp, CDEEP.users u where FIND_IN_SET('".$tem[0]."', rp.states) > 0 and rp.user_uid=u.uid";
			db_set_active('workshop_info');
			$result = db_query($query);
			$mailto = '';
			while($row = db_fetch_object($result)){
				$mailto .= $row->mail.',';
				
			}
			db_set_active('default');
			//$mailfrom = variable_get('site_mail');
			$subject = 'Workshop Request Notification';
			$message = "<p>Dear Resource-person,</p><p>".$user->name." made a new workshop request.</p><p><b>Workshop Details</b></p><p>Foss category : ".$form_state['values']['foss_category']."</p><p>Preferred Workshop Date : ".$form_state['values']['pref_wkshop_date']."</p><p>Preferred Workshop Time : ".$pref_wkshop_time."</p>";
			workshops_notify($mailto, $subject, $message);
			drupal_set_message('Workshop Request Details Added Successfully');
			drupal_goto($path = $base_url, $query = NULL, $fragment = NULL, $http_response_code = 302);
		}else {
			drupal_set_message('Error Adding Workshop Request Details');
		}
		db_set_active('default');
		drupal_goto($path = 'workshops/list_workshop_request', $query = NULL, $fragment = NULL, $http_response_code = 302);
	}

	function workshops_list_workshop_request(){
		global $user;
		$query = "select wr.*, o.organiser_name  from workshop_requests wr, organiser o where wr.academic_code in (SELECT ac.`academic_code` FROM `academic_center` ac, resource_person rp WHERE rp.user_uid=".$user->uid." and FIND_IN_SET(ac.state_code, rp.states) > 0) and wr.status=0 and wr.organiser_id=o.organiser_id";
		$count_query = "select count(*) from workshop_requests wr, organiser o where wr.academic_code in (SELECT ac.`academic_code` FROM `academic_center` ac, resource_person rp WHERE rp.user_uid=".$user->uid." and FIND_IN_SET(ac.state_code, rp.states) > 0) and wr.status=0 and wr.organiser_id=o.organiser_id";
		$header = array('Sno', 'FOSS', 'Academic Center Code', 'Department', 'Organiser', 'Action');
		db_set_active('workshop_info');
		$result = pager_query($query, 30, 0, $count_query);
		db_set_active('default');
		$row = array();
		$sno = 1;
		
		while ($item = db_fetch_object($result)) {
			$row_tmp = db_fetch_object($res1);
			$row[] = array($sno++, $item->foss_category, $item->academic_code, $item->department, $item->organiser_name, l('Confirm', 'workshops/workshop_confirm/'. $item->id));
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

	function workshops_list_workshop_approval(){
		global $user;
		$query = "select distinct wr.*, o.organiser_name  from workshop_requests wr, organiser o where wr.academic_code in (SELECT ac.`academic_code` FROM `academic_center` ac, resource_person rp WHERE rp.user_uid=".$user->uid." and FIND_IN_SET(ac.state_code, rp.states) > 0) and wr.status=1 and o.organiser_id=wr.organiser_id order by cfm_wkshop_date DESC";
		$count_query = "select count(*) from workshop_requests wr, organiser o where wr.academic_code in (SELECT ac.`academic_code` FROM `academic_center` ac, resource_person rp WHERE rp.user_uid=".$user->uid." and FIND_IN_SET(ac.state_code, rp.states) > 0) and wr.status=1 and o.organiser_id=wr.organiser_id order by cfm_wkshop_date DESC";
		$header = array('Sno', 'Workshop Code', 'FOSS', 'Academic Center Code', 'Date/Time', 'Department', 'Organiser', 'Action');
		db_set_active('workshop_info');
		$result = pager_query($query , 30, 0, $count_query);
		db_set_active('default');
		$row = array();
		$sno = 1;
		
		while ($item = db_fetch_object($result)) {
			$row[] = array($sno++, $item->workshop_code, $item->foss_category, $item->academic_code, $item->cfm_wkshop_date.' / '.$item->cfm_wkshop_time, $item->department, $item->organiser_name, l('Edit', 'workshops/workshop_edit/'. $item->id));
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

	function workshops_list_workshop_completed(){
		global $user;
		$query = "select wr.*, o.organiser_name  from workshop_requests wr, organiser o where wr.academic_code in (SELECT ac.`academic_code` FROM `academic_center` ac, resource_person rp WHERE rp.user_uid=".$user->uid." and FIND_IN_SET(ac.state_code, rp.states) > 0) and wr.status=2 and wr.organiser_id=o.organiser_id order by cfm_wkshop_date  DESC";
		$count_query = "select count(*) from workshop_requests wr, organiser o where wr.academic_code in (SELECT ac.`academic_code` FROM `academic_center` ac, resource_person rp WHERE rp.user_uid=".$user->uid." and FIND_IN_SET(ac.state_code, rp.states) > 0) and wr.status=2 and wr.organiser_id=o.organiser_id order by cfm_wkshop_date  DESC";
		$header = array('Sno','Workshop Code', 'FOSS', 'Language', 'Academic Center Code', 'Department', 'Date & Time', 'Action');
		db_set_active('workshop_info');
		$result = pager_query($query , 30, 0, $count_query);
		db_set_active('default');
		$row = array();
		$sno = 1;
		
		while ($item = db_fetch_object($result)) {
			$row[] = array($sno++, $item->workshop_code, $item->foss_category, $item->pref_language, $item->academic_code, $item->department, $item->cfm_wkshop_date." ".$item->cfm_wkshop_time, l('View', 'workshops/view_workshop/'. $item->id));
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

	function workshops_workshop_confirm($id = NULL){
		$path = drupal_get_path('module', 'workshops');
		drupal_add_js($path . '/datepickr.js');
		drupal_add_css($path . '/datepickr.css');
		drupal_add_js($path . '/workshops.js');
		drupal_add_css($path . '/workshops.css');
  drupal_add_js("$(document).ready(function(){ new datepickr('edit-cfm-wkshop-date', { dateFormat: 'Y-m-d' }); });", 'inline');
		if($id){
			$query = "select * from {workshop_requests} WHERE id='". $id."'";
			db_set_active('workshop_info');
			$result = db_query($query);
			$item = db_fetch_object($result);
			db_set_active('default');
			//$query = "select * from {academic_center} WHERE academic_code='". $item->academic_code."'";

			//$result = db_query($query);
			//$academic_item = db_fetch_object($result);
			$output = '<div class="form-item" id="academic-details"><table width="100%" cellpadding="6">';
			$output .= '<tr><td>Academic Center Code</td><td>: '.$item->academic_code.'</td></tr>';
			$output .= '<tr><td>Language</td><td>: '.$item->pref_language.'</td></tr>';
			$output .= '<tr><td>FOSS Category</td><td>: '.$item->foss_category.'</td></tr>';
			$output .= '<tr><td>Preferred Date & Time</td><td>: '.$item->pref_wkshop_date.' '.$item->pref_wkshop_time.'</td>
			</tr>';
			$output .= '<tr><td>Skype Request</td><td>: '.(($item->skype_request == 1) ? 'Yes': 'No').'</td></tr>';
			$output .= '</table></div>';
			$output .= drupal_get_form('workshops_workshop_confirm_form', $id, $item->pref_wkshop_date, $item->pref_wkshop_time, $item->organiser_id);
			return $output;
		}else{
			return "<p>Sorry, no data found.</p>";
		}
	}

	function workshops_workshop_confirm_form($form_state, $id, $date_val, $time_val, $oid){
	
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
			'#title' => t('Preferred Workshop Date'),
			'#value' => $id,
		);
		$form['oid'] = array(
			'#type' => 'hidden',
			'#title' => t('Preferred Workshop Date'),
			'#value' => $oid,
		);
		$form['cfm_wkshop_date'] = array(
			'#type' => 'textfield',
			'#title' => t('Confirm Workshop Date'),
			'#required' => TRUE,
			'#size' => 30,
			'#value' => $date_val,
			'#attributes' => array('class' => 'cfm_wkshop_date', 'readonly' => 'readonly'),
		);

		$form['cfm_wkshop_time'] = array(
			'#type' => 'select',
			'#required' => TRUE,
			'#options' => $hour,
			'#default_value' => $time_arr[0],
			'#attributes' => array('class' => 'pref_wkshop_time'),
			'#prefix' => "<div class='date-and-time'><label for='edit-pref-wkshop-time'><b>Confirm Workshop Time: </b><span class='form-required' title='This field is required.'>*</span></label>",
		);
		$form['cfm_wkshop_minute'] = array(
			'#type' => 'select',
			'#required' => TRUE,
			'#options' => $minite,
			'#default_value' => $time_arr[1],
			'#attributes' => array('class' => 'pref_wkshop_minute'),
			'#suffix' => "</div>"
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
		global $user;
		global $base_url;
		$cfm_wkshop_date = $_POST['cfm_wkshop_date'];
		$cfm_wkshop_time = $_POST['cfm_wkshop_time'].':'.$_POST['cfm_wkshop_minute'].':00';
		$workshop_code = 'WC-'.$form_state['values']['id'];
		$query = "update {workshop_requests} set workshop_code='".$workshop_code."', cfm_wkshop_date='".$cfm_wkshop_date."', cfm_wkshop_time='".$cfm_wkshop_time."', status=1 where id=".$form_state['values']['id'];
		db_set_active('workshop_info');
		if(db_query($query)){
			// mail to Resorce Person

			//$mailfrom = variable_get('site_mail');
			$mailto = $user->mail;
			$subject = 'Workshop confirmation Notification';
			$message = "<p>Dear ".$user->name.",</p><p>workshop ".$workshop_code." has been approved.</p>";
			workshops_notify($mailto, $subject, $message);
			
			
			// send email to Organiser
			$query = "select name, mail from users where uid='".$_POST['oid']."'";
			$result = db_query($query);
			$row = db_fetch_object($result);
			$mailto = $row->mail;
			//$mailfrom = variable_get('site_mail');
			$subject = 'Workshop confirmation Notification';
			$message = "<p>Dear ".$row->name.",</p><p> Your new workshop request has has been confirmed.</p><p><b>Workshop Details</b></p><p>Workshop code : ".$workshop_code."</p><p>Confirmed Workshop Date : ".$cfm_wkshop_date."</p><p>Confirmed Workshop Time : ".$cfm_wkshop_time."</p>";
			workshops_notify($mailto, $subject, $message);
			drupal_set_message('Workshop Confirmation Added Successfully');
			drupal_goto($path = 'workshops/list_workshop_approval', $query = NULL, $fragment = NULL, $http_response_code = 302);
		}else {
			drupal_set_message('Error Adding Workshop Confirmation Details');
		}
		db_set_active('default');
	}
	function workshops_workshop_edit($id = NULL){
		$path = drupal_get_path('module', 'workshops');
		drupal_add_js($path . '/datepickr.js');
		drupal_add_css($path . '/datepickr.css');
		drupal_add_js($path . '/workshops.js');
		drupal_add_css($path . '/workshops.css');
  drupal_add_js("$(document).ready(function(){ new datepickr('edit-cfm-wkshop-date', { dateFormat: 'Y-m-d' }); });", 'inline');	
		if($id){
			$query = "select * from {workshop_requests} WHERE id='". $id."'";
			db_set_active('workshop_info');
			$result = db_query($query);
			$item = db_fetch_object($result);
			db_set_active('default');
			//$query = "select * from {academic_center} WHERE academic_code='". $item->academic_code."'";

			//$result = db_query($query);
			//$academic_item = db_fetch_object($result);
			$output = '<div class="form-item" id="academic-details"><table width="100%" cellpadding="6">';
			$output .= '<tr><td>Academic Center Code</td><td>: '.$item->academic_code.'</td></tr>';
			$output .= '<tr><td>Language</td><td>: '.$item->pref_language.'</td></tr>';
			$output .= '<tr><td>FOSS Category</td><td>: '.$item->foss_category.'</td></tr>';
			$output .= '<tr><td>Preferred Date & Time</td><td>: '.$item->cfm_wkshop_date.' '.$item->cfm_wkshop_time.'</td></tr>';
			$output .= '</table></div>';
			$output .= drupal_get_form('workshops_workshop_edit_form', $id, $item->cfm_wkshop_date, $item->cfm_wkshop_time, $item->organiser_id);
			return $output;
		}else{
			return "<p>Sorry, no data found.</p>";
		}
	}

	function workshops_workshop_edit_form($form_state, $id, $date_val, $time_val, $oid){
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
			'#title' => t('Preferred Workshop Date'),
			'#value' => $id,
		);
		$form['oid'] = array(
			'#type' => 'hidden',
			'#title' => t('Preferred Workshop Date'),
			'#value' => $oid,
		);
		$form['cfm_wkshop_date'] = array(
			'#type' => 'textfield',
			'#title' => t('Confirm Workshop Date'),
			'#required' => TRUE,
			'#size' => 30,
			'#value' => $date_val,
			'#attributes' => array('class' => 'cfm_wkshop_date', 'readonly' => 'readonly')
		);
		$form['cfm_wkshop_time'] = array(
			'#type' => 'select',
			'#required' => TRUE,
			'#options' => $hour,
			'#default_value' => $time_arr[0],
			'#attributes' => array('class' => 'pref_wkshop_time'),
			'#prefix' => "<div class='date-and-time'><label for='edit-pref-wkshop-time'><b>Confirm Workshop Time: </b><span class='form-required' title='This field is required.'>*</span></label>",
		);
		$form['cfm_wkshop_minute'] = array(
			'#type' => 'select',
			'#required' => TRUE,
			'#options' => $minite,
			'#default_value' => $time_arr[1],
			'#attributes' => array('class' => 'pref_wkshop_minute'),
			'#suffix' => "</div>"
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);
		return $form;
	}
	
	function workshops_workshop_edit_form_submit($form, &$form_state){
		global $user;
		$cfm_wkshop_date = $_POST['cfm_wkshop_date'];
		$cfm_wkshop_time = $_POST['cfm_wkshop_time'].':'.$_POST['cfm_wkshop_minute'].':00';
		$query = "update {workshop_requests} set cfm_wkshop_date='".$cfm_wkshop_date."', cfm_wkshop_time='".$cfm_wkshop_time."', status=1 where id=".$form_state['values']['id'];
		db_set_active('workshop_info');
		if(db_query($query)){
		
		// mail to Resorce Person

			//$mailfrom = variable_get('site_mail');
			$mailto = $user->mail;
			$subject = 'Workshop confirmation Notification';
			$message = "<p>Dear ".$user->name.",</p><p>Approved workshop WC-".$_POST['id']." has been Updated.</p>";
			workshops_notify($mailto, $subject, $message);
			// send email to Organiser
			$query = "select name, mail from users where uid='".$_POST['oid']."'";
			$result = db_query($query);
			$row = db_fetch_object($result);
			$mailto = $row->mail;
			//$mailfrom = variable_get('site_mail');
			$subject = 'Workshop confirmation Notification';
			$message = "<p>Dear ".$row->name.",</p><p> Your workshop WC-".$_POST['id']." has has been postponded.</p><p><b>Workshop Details</b></p><p>Confirmed Workshop Date : ".$cfm_wkshop_date."</p><p>Confirmed Workshop Time : ".$cfm_wkshop_time."</p>";
			workshops_notify($mailto, $subject, $message);
			drupal_set_message('Workshop Confirmation Added Successfully');

			drupal_goto($path = 'workshops/list_workshop_approval', $query = NULL, $fragment = NULL, $http_response_code = 302);
		}else {
			drupal_set_message('Error Adding Workshop Confirmation Details');
		}
		db_set_active('default');
	}
	function workshops_view_workshop($id = NULL){
	 	if($id){
	 		$query = "select * from {workshop_requests} where id=".$id;
	 		db_set_active('workshop_info');
	 		$result = db_query($query);
	 		db_set_active('default');
	 		if($result){
	 			$item = db_fetch_object($result);
	 			$output = '<table width="100%" cellpadding="6">';
	 			$output .= '<tr><td><b>Workshop Code</b></td><td><b>'.$item->workshop_code.'</b></td></tr>';
	 			$output .= '<tr><td><b>Foss Category</b></td><td><b>'.$item->foss_category.'</b></td></tr>';
	 			$output .= '<tr><td><b>Language</b></td><td><b>'.$item->pref_language.'</b></td></tr>';
	 			$output .= '<tr><td><b>Date & Time</b></td><td><b>'.$item->cfm_wkshop_date.' &nbsp;'.$item->cfm_wkshop_time.'</b></td></tr>';

	 			$query = "select * from  {workshop_details} where workshop_code='".$item->workshop_code."'";
	 			db_set_active('workshop_info');
	 			$result = db_query($query);
	 			db_set_active('default');
	 			$ws_detail = db_fetch_object($result);

	 			$output .= '<tr><td><b>Participants Count</b></td><td><b>'.$ws_detail->no_of_participants.'</b></td></tr>';
	 			$output .= '<tr><td><b>Invigilator</b></td><td><b>'.$ws_detail->name_of_invigilator.'</b></td></tr>';
	 			$output .= '<tr><td colspan="2"><hr /></td></tr>';

	 			$query = "select * from {academic_center} where academic_code='".$item->academic_code."'";
	 			db_set_active('workshop_info');
	 			$result = db_query($query);
	 			db_set_active('default');
	 			$coll_detail = db_fetch_object($result);

	 			$output .= '<tr><td><b>State Code</b></td><td><b>'.$coll_detail->state_code.'</b></td></tr>';
	 			$output .= '<tr><td><b>Academic Center Code</b></td><td><b>'.$coll_detail->academic_code.'</b></td></tr>';
	 			$output .= '<tr><td><b>Institution Name</b></td><td><b>'.$coll_detail->institution_name.'</b></td></tr>';
	 			$output .= '<tr><td><b>Department</b></td><td><b>'.$item->department.'</b></td></tr>';
	 			$output .= '<tr><td><b>Street</b></td><td><b>'.$coll_detail->street.'</b></td></tr>';
	 			$output .= '<tr><td><b>City</b></td><td><b>'.$coll_detail->city.'</b></td></tr>';
	 			$output .= '<tr><td><b>Pin Code</b></td><td><b>'.$coll_detail->pincode.'</b></td></tr>';
	 			$output .= '</table>';
	 			
	 			$output .= '<h2>Participants Details</h2>';
	 			//$query = "select id, user_id from {workshop_feedback} where workshop_code='".$item->workshop_code."'";
	 			$query = "select u.username as user_name, wf.user_id as uid from workshop_info.workshop_feedback wf, OTC.mdl_user u where wf.workshop_code='".$item->workshop_code."' and u.id=wf.user_id";
	 			db_set_active('workshop_info');
	 			$result = db_query($query);
	 			db_set_active('default');
	 			$output .= '<table width="100%" cellpadding="6"><tr><th>User name</th><th>Action</th></tr>';
	 			while($fd = db_fetch_object($result)){
	 				$output .= '<tr><td>'.$fd->user_name."</td><td>".l('View Feedback', 'workshops/feedback/'.$item->workshop_code.'/'.$fd->user_name.'/'.$fd->uid)."</td></tr>";
	 				
	 			}
	 			$output .= '</table>';
	 			//exit;

	 			
	 			return $output;
	 		}else {
	 			drupal_set_message('Error accessing database workshop_info');
	 		}
	 	}else{
	 		return "<p>Sorry, no data found.</p>";
	 	}
	}

	function workshops_admin_workshop_completed(){
		global $user;
		$query = "select * from {workshop_requests} where status=2 ORDER BY workshop_code ASC";
		// $query = "SELECT * FROM {workshop_requests} WHERE status=2 ORDER BY foss_category, pref_language ASC";
		$header = array('Sno','Workshop Code', 'FOSS', 'Language', 'Academic Center Code', 'Department', 'Date & Time', 'Action');
		$tablesort = tablesort_sql($header);
		db_set_active('workshop_info');
		$result = pager_query($query . $tablesort, 30, 0, NULL);
		db_set_active('default');
		$row = array();
		$sno = 1;
		
		while ($item = db_fetch_object($result)) {
			$row[] = array($sno++, $item->workshop_code, $item->foss_category, $item->pref_language, $item->academic_code, $item->department, $item->cfm_wkshop_date." ".$item->cfm_wkshop_time, l('View', 'workshops/view_workshop/'. $item->id));
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
?>
