<?php
	
	function aworkshops_workshop_request(){
  $path = drupal_get_path('module', 'workshops');
		drupal_add_js($path . '/datepickr.js');
		drupal_add_css($path . '/datepickr.css');
		drupal_add_js($path . '/workshops.js');
  drupal_add_js("$(document).ready(function(){ new datepickr('edit-pref-wkshop-date', { dateFormat: 'Y-m-d' }); });", 'inline');
		$output = '<div class="form-item" id="academic-details"></div>';
		$output .= drupal_get_form('aworkshops_workshop_request_form');
		return $output;
	}
	function aworkshops_workshop_request_form(){
			
		$hour = array('' => 'Hour');
		$minite = array('' => 'Minute');
		$foss_category = array();
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
			'#multiple' => TRUE,
			'#title' => t('Foss Category'),
			'#options' => $foss_category,
			'#required' => TRUE,
			'#attributes' => array('class' => 'foss_categoryy'),
			'#prefix' => '<div class="ws-foss-lang"><div class="ws-foss">',
			'#suffix' => '</div>'
		);
		$form['pref_language'] = array(
			'#type' => 'textfield',
			'#title' => t('Preferred Language'),
			'#value' => 'English',
			'#required' => TRUE,
			'#prefix' => '<div class="ws-lang">',
			'#attributes' => array('size' => 30,'readonly' => 'readonly'),
			'#suffix' => '</div></div>'
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);
		
		return $form;
	}
	function aworkshops_workshop_request_form_validate($form, &$form_state){
		if (!preg_match('/^[a-zA-Z ]+$/', $form_state['values']['department'])){
			form_set_error('department', t('Invalid Department'));
		}
		if($form_state['values']['pref_language'] == ""){
                        form_set_error('pref_language', t('Please select language'));
                }

	  return;
	}
	function aworkshops_workshop_request_form_submit($form, &$form_state){
		global $user;
		global $base_url;
		$foss = '';
		foreach($form_state['values']['foss_category'] as $val ){
			if($foss != ''){
				$foss .= ',';
			}
			$foss .= $val;
		}
		$pref_wkshop_date = $form_state['values']['pref_wkshop_date'];
		$pref_wkshop_time = $form_state['values']['pref_wkshop_time'].':'.$form_state['values']['pref_wkshop_minute'];
		$query = "insert into  workshop_requests(academic_code, organiser_id, department, pref_wkshop_date, pref_wkshop_time, foss_category, pref_language, skype_request) values('".$form_state['values']['academic_code']."', '".$user->uid."', '".$form_state['values']['department']."', '".$pref_wkshop_date."', '".$pref_wkshop_time."', '".$foss."', '".$form_state['values']['pref_language']."',0)";
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

?>