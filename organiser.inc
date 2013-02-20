<?php
	
	function workshops_add_organiser(){		
		$output = '<div class="form-item" id="academic-details"></div>';
		$output .= drupal_get_form('workshops_add_organiser_form');
		return $output;
	}
	function workshops_add_organiser_form(){
	
		$path = drupal_get_path('module', 'workshops');
		drupal_add_js($path . '/workshops.js', 'module');
		$form['#prefix'] = '<div class="workshop-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data");
		
		$form['academic_code'] = array(
			'#type' => 'textfield',
			'#title' => t('Academic Center Code'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'organiser_academic_code'),
		);
		$form['organiser_name'] = array(
			'#type' => 'textfield',
			'#title' => t('Organiser Name'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'organiser_name')
		);
		$form['permanemt_address'] = array(
			'#type' => 'textarea',
			'#title' => t('Permanent Address'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'permanemt_address')
		);
		$form['city'] = array(
			'#type' => 'textfield',
			'#title' => t('City'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'city')
		);
		$form['phone'] = array(
			'#type' => 'textfield',
			'#title' => t('Phone'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'phone')
		);
		$form['department'] = array(
			'#type' => 'textfield',
			'#title' => t('Department / School'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'department')
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);
		
		return $form;
	}
	function workshops_add_organiser_form_validate($form, &$form_state){
		if (!preg_match('/^[0-9\ \+]{0,15}$/', $form_state['values']['phone'])){
			form_set_error('phone number', t('Invalid Phone number'));
		}
		 if ($form_state['values']['academic_code'] != ''){
			$query = "select id from academic_center where academic_code='".$form_state['values']['academic_code']."'";
			db_set_active('workshop_info');
			$result = db_query($query);
			db_set_active('default');
			if(!$row = db_fetch_object($result)){
				form_set_error('academic_code', t('Invalid Academic Center Code'));			
			}		
                }

		
  		return;

	}
	function workshops_add_organiser_form_submit($form, &$form_state){
		global $user;
		
		$query = "select * from organiser where organiser_id='".$user->uid."'";
		db_set_active('workshop_info');
		$result = db_query($query);
		db_set_active('default');
		if($row = db_fetch_object($result)){
			db_set_active('default');
			if($row->status == 0){
				drupal_set_message('You have already sent the organiser request, your request is waiting for resource person\'s approval');
			}else{
				drupal_set_message('You already have an organiser permissions');
			}
		}else{
			//print_r($form_state['values']);
			$query = "insert into organiser (academic_code,organiser_name,address,city,phone,department,organiser_id,status) values('".$form_state['values']['academic_code']."','".$form_state['values']['organiser_name']."', '".$form_state['values']['permanemt_address']."', '".$form_state['values']['city']."', '".$form_state['values']['phone']."', '".$form_state['values']['department']."', '".$user->uid."', 0)";
			db_set_active('workshop_info');
			if(db_query($query)){
				drupal_set_message('Organiser Details Added Successfully');
			}else {
				drupal_set_message('Error Adding Organiser Details');
			}
			db_set_active('default');
		}
	}
	
	function accept_organiser($id = NULL){
		if($id){
			//echo "id: ".$id; die;
			$query = "select o.*, ac.institution_name from organiser o, academic_center ac where o.id=".$id." and ac.academic_code=o.academic_code";
			//echo $query; die;
			db_set_active('workshop_info');
			$result = db_query($query);
			db_set_active('default');
			$row = db_fetch_object($result);
			$query = "SELECT rid FROM `role` WHERE name = 'organiser'";
			$result = db_query($query);
			$role_id = db_fetch_object($result);
			// insert into role_user
			$query = "INSERT INTO users_roles (uid, rid) VALUES ('".$row->organiser_id."', '".$role_id->rid."')";	
			db_query($query);
			$query = "select mail from users where uid=".$row->organiser_id;
			$result = db_query($query);
			$organiser_email = db_fetch_object($result);
			// change organiser status to active
			$query = "update organiser set status=1 where id=".$id;
			//echo $query; die;
			db_set_active('workshop_info');
			if(db_query($query)){
				db_set_active('default');
				// mail to Organiser
				//$mailfrom = variable_get('site_mail');
				$mailto = $organiser_email->mail;
				$subject = 'Account notification';
				$message = "<p>Dear ".$row->organiser_name.",</p><p>Thank you for registering at <a href='http://spoken-tutorial.org'>spoken-tutorial.org</a>. You are added as an organiser for '".$row->institution_name."'.";

				//echo $message; die;
				//echo $mailto."<br><br>".$subject."<br><br>".$message; die;
				workshops_notify($mailto, $subject, $message);
				global $base_url;
				drupal_set_message('Organiser is added successfully.');
				drupal_goto($path = $base_url.'/workshops/college/list_pending_organisers', $query = NULL, $fragment = NULL, $http_response_code = 302);
			}
			db_set_active('default');
		}else{
			drupal_set_message('Invalid request!');
			drupal_goto($path = $base_url.'/workshops/college/list_pending_organisers', $query = NULL, $fragment = NULL, $http_response_code = 302);
		}
	}
	
	function reject_organiser($id = NULL){
		if($id){
			$query = "delete from organiser where id=".$id;
			db_set_active('workshop_info');
			if(db_query($query)){
				db_set_active('default');
				drupal_set_message("Organiser request has been removed from list.");
				drupal_goto($path = $base_url.'/workshops/college/list_pending_organisers', $query = NULL, $fragment = NULL, $http_response_code = 302);
			}
		}else{
			db_set_active('default');
			drupal_set_message('Invalid request!');
			drupal_goto($path = $base_url.'/workshops/college/list_pending_organisers', $query = NULL, $fragment = NULL, $http_response_code = 302);
		}
	}
?>
