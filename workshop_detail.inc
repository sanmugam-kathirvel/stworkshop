<?php
	
	function workshop_detail(){
		$output = '';//<div class="form-item" id="workshop-details"></div>';
		$output .= drupal_get_form('workshop_detail_form');
		return $output;
	}
	function workshop_detail_form(){
		$path = drupal_get_path('module', 'workshops');
		drupal_add_js($path . '/workshops.js', 'module');
		$form['#prefix'] = '<div class="workshop-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data");

		$form['workshop_code'] = array(
			'#type' => 'textfield',
			'#title' => t('Workshop Code'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'detail_workshop'),
		);
		$form['no_of_participants'] = array(
			'#type' => 'textfield',
			'#title' => t('No of Participants'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'no_of_participants'),
		);
		
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);
		
		return $form;
	}
	function workshop_detail_form_validate($form, &$form_state){
		if (!preg_match('/^[0-9\ \+]{0,15}$/', $form_state['values']['no_of_participants'])){
			form_set_error('no_of_participants', t('Invalid participants count'));
		}
		if ($form_state['values']['workshop_code'] != ''){
			$query = "select * from workshop_requests where workshop_code='".$form_state['values']['workshop_code']."'";
			db_set_active('workshop_info');
			$result = db_query($query);
			db_set_active('default');
			if($row = db_fetch_object($result)){
				if($row->status == '2')
					form_set_error('workshop_code', t('You have already entered workshop detail'));
			}else{
				form_set_error('workshop_code', t('Invalid workshop code'));
			}
		}
  		return;

	}
	function workshop_detail_form_submit($form, &$form_state){

		$query = "update workshop_requests set status=2 where workshop_code='".$form_state['values']['workshop_code']."'";
		db_set_active('workshop_info');
		db_query($query);
		$query = "insert into  workshop_details values('', '".$form_state['values']['workshop_code']."', '".$form_state['values']['no_of_participants']."')";

		if(db_query($query)){
			// mail to Organiser
			/*$query = "select email, organiser_name from organiser left join workshop_requests on organiser.username = workshop_requests.organiser_id where workshop_requests.workshop_code='".$form_state['values']['workshop_code']."'";
			
			db_set_active('workshop_info');
			$result = db_query($query);
			$row = db_fetch_object($result);
			db_set_active('default');

			//$mailfrom = variable_get('site_mail');
			$mailto = $row->email;
			$subject = 'Workshop Detail';
			
			$message = "<p>Dear ".$row->organiser_name.",</p><p>Workshop ".$form_state['values']['workshop_code']." details has been updated. The details are </p><p>No of Participants : ".$form_state['values']['no_of_participants']."</p><p>Name of Invigilator : ".$form_state['values']['name_of_invigilator']."</p>";
			workshops_notify($mailto, $subject, $message); */
						
			drupal_set_message('Workshop Details Added Successfully');
			drupal_goto($path = 'workshops/workshop_detail', $query = NULL, $fragment = NULL, $http_response_code = 302);
		}else {
			drupal_set_message('Error Adding Workshop Details');
		}
		
	}
?>
