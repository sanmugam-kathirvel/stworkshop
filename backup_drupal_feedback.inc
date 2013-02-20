<?php

	function feedback(){
		$output = '<div class="form-item" id="feedback-error"></div>';
		$output .= drupal_get_form('feedback_form');
		return $output;
	}
	function feedback_form(){
		$path = drupal_get_path('module', 'workshops');
		drupal_add_js($path . '/workshops.js', 'module');
		drupal_add_css($path . '/workshops.css', 'module');
		$form['#attributes'] = array('enctype' => "multipart/form-data");

		$form['user_name'] = array(
			'#type' => 'textfield',
			'#title' => t('Username of the Participant'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'user_name'),
		);
		
		$form['workshop_code'] = array(
			'#type' => 'textfield',
			'#title' => t('Workshop Code'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'feedback_workshop_code'),
		);

		$form['participant_name'] = array(
			'#type' => 'textfield',
			'#title' => t('Name'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'participant_name', 'readonly' => 'readonly'),
		);
		$form['institution_name'] = array(
			'#type' => 'textfield',
			'#title' => t('Name of the Institution/College/University'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'institution_name', 'readonly' => 'readonly'),
		);
		$form['email'] = array(
			'#type' => 'textfield',
			'#title' => t('Email'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'feedback_email', 'readonly' => 'readonly'),
		);
		$form['range_of_topics_covered'] = array(
			'#type' => 'radios',
			'#title' => t('Range of topics covered in tutorial'),
			'#default_value' => 'Very relevant',
			'#options' => array('Very relevant' => 'Very relevant', 'Relevant' => 'Relevant', 'Somewhat relevant' => 'Somewhat relevant', 'Not relevant' => 'Not relevant'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'range_of_topics_covered'),
		);
		$form['depth_of_coverage'] = array(
			'#type' => 'radios',
			'#title' => t('Depth of coverage'),
			'#default_value' => 'Too detailed',
			'#options' => array('Too detailed' => 'Too detailed', 'Detailed' => 'Detailed', 'Not enough details' => 'Not enough details', 'Poor details' => 'Poor details'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'depth_of_coverage'),
		);
		$form['Pace_of_tutorial'] = array(
			'#type' => 'radios',
			'#title' => t('Pace of the tutorial'),
			'#default_value' => 'Just right',
			'#options' => array('Veryfast' => 'Veryfast', 'Fast' => 'Fast', 'Just right' => 'Just right', 'Slow' => 'Slow'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'Pace_of_tutorial'),
		);
		$form['workshop_venue'] = array(
			'#type' => 'radios',
			'#title' => t('The workshop venue was "Comfortable"'),
			'#default_value' => 'Excellent',
			'#options' => array('Poor' => 'Poor', 'Fair' => 'Fair', 'Average' => 'Average', 'Good' => 'Good', 'Excellent' => 'Excellent', 'Not Sure' => 'Not Sure'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'workshop_venue'),
		);
		$form['arrangements'] = array(
			'#type' => 'radios',
			'#title' => t('The arrangements for the workshop were good'),
			'#default_value' => 'Excellent',
			'#options' => array('Poor' => 'Poor', 'Fair' => 'Fair', 'Average' => 'Average', 'Good' => 'Good', 'Excellent' => 'Excellent', 'Not Sure' => 'Not Sure'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'arrangements'),
		);
		$form['content_relevent'] = array(
			'#type' => 'radios',
			'#title' => t('The workshop content was "Relevant"'),
			'#default_value' => 'Excellent',
			'#options' => array('Poor' => 'Poor', 'Fair' => 'Fair', 'Average' => 'Average', 'Good' => 'Good', 'Excellent' => 'Excellent', 'Not Sure' => 'Not Sure'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'content_relevent'),
		);
		$form['content_comprehensive'] = array(
			'#type' => 'radios',
			'#title' => t('The workshop content was "Comprehensive"'),
			'#default_value' => 'Excellent',
			'#options' => array('Poor' => 'Poor', 'Fair' => 'Fair', 'Average' => 'Average', 'Good' => 'Good', 'Excellent' => 'Excellent', 'Not Sure' => 'Not Sure'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'content_comprehensive'),
		);
		$form['content_easy'] = array(
			'#type' => 'radios',
			'#title' => t('The workshop content was Easy to Understand'),
			'#default_value' => 'Excellent',
			'#options' => array('Poor' => 'Poor', 'Fair' => 'Fair', 'Average' => 'Average', 'Good' => 'Good', 'Excellent' => 'Excellent', 'Not Sure' => 'Not Sure'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'content_easy'),
		);
		$form['break_sufficient'] = array(
			'#type' => 'radios',
			'#title' => t('Breaks were sufficient during the workshop'),
			'#default_value' => 'Excellent',
			'#options' => array('Poor' => 'Poor', 'Fair' => 'Fair', 'Average' => 'Average', 'Good' => 'Good', 'Excellent' => 'Excellent', 'Not Sure' => 'Not Sure'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'break_sufficient'),
		);
		$form['good_mix'] = array(
			'#type' => 'radios',
			'#title' => t('A good mix between listening and activities during the workshop'),
			'#default_value' => 'Excellent',
			'#options' => array('Poor' => 'Poor', 'Fair' => 'Fair', 'Average' => 'Average', 'Good' => 'Good', 'Excellent' => 'Excellent', 'Not Sure' => 'Not Sure'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'good_mix'),
		);
		$form['learning_experience'] = array(
			'#type' => 'radios',
			'#title' => t('The workshop was a useful learning experience'),
			'#default_value' => 'Excellent',
			'#options' => array('Poor' => 'Poor', 'Fair' => 'Fair', 'Average' => 'Average', 'Good' => 'Good', 'Excellent' => 'Excellent', 'Not Sure' => 'Not Sure'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'learning_experience'),
		);
		$form['resource_person_knowledgeable'] = array(
			'#type' => 'radios',
			'#title' => t('The resource persons/domain experts were knowledgeable and well prepared'),
			'#default_value' => 'Excellent',
			'#options' => array('Poor' => 'Poor', 'Fair' => 'Fair', 'Average' => 'Average', 'Good' => 'Good', 'Excellent' => 'Excellent', 'Not Sure' => 'Not Sure'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'resource_person_knowledgeable'),
		);
		$form['resource_person_responsive'] = array(
			'#type' => 'radios',
			'#title' => t('The resource persons/domain experts were "Responsive to participants" questions'),
			'#default_value' => 'Excellent',
			'#options' => array('Poor' => 'Poor', 'Fair' => 'Fair', 'Average' => 'Average', 'Good' => 'Good', 'Excellent' => 'Excellent', 'Not Sure' => 'Not Sure'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'resource_person_responsive'),
		);
		$form['like_most'] = array(
			'#type' => 'textarea',
			'#title' => t('What did you like most about this workshop?'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'like_most'),
		);
		$form['like_least'] = array(
			'#type' => 'textarea',
			'#title' => t('What did you like least about this workshop?'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'like_least'),
		);
		$form['improved'] = array(
			'#type' => 'textarea',
			'#title' => t('How could this workshop be improved?'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'improved'),
		);
		$form['foss_recommend'] = array(
			'#type' => 'radios',
			'#title' => t('Would you recommend the FOSS, Spoken Tutorial Project and its activities to others?'),
			'#required' => TRUE,
			'#default_value' => 'Yes',
			'#options' => array('Yes' => 'Yes', 'No' => 'No'),
			'#attributes' => array('class' => 'foss_recommend'),
		);
		$form['why_or_whynot'] = array(
			'#type' => 'textarea',
			'#title' => t('Why/why not'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'why_or_whynot'),
		);	

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);
		
		return $form;
	}
	function feedback_form_validate($form, &$form_state){
		if (!preg_match('/^[0-9\ \+]{0,15}$/', $form_state['values']['no_of_participants'])){
			form_set_error('phone number', t('Invalid participants'));
		}
		if ($form_state['values']['user_name'] && $form_state['values']['workshop_code']){
			$query = "select user_name, workshop_code from workshop_feedback where workshop_code='".$form_state['values']['workshop_code']."' and user_name='".$form_state['values']['user_name']."'";
			db_set_active('workshop_info');
			$result = db_query($query);
			$item = db_fetch_object($result);
			if($item){
				form_set_error('workshop code', t('You have already gave feedback for this workshop'));
			}
			db_set_active('default');
			//form_set_error('phone number', t('Invalid participants'));
		}
  		return;

	}
	function feedback_form_submit($form, &$form_state){
		$query = "insert into  {workshop_feedback} values('', '".$form_state['values']['user_name']."', '".$form_state['values']['participant_name']."', '".$form_state['values']['institution_name']."', '".$form_state['values']['email']."', '".$form_state['values']['workshop_code']."', '".$form_state['values']['range_of_topics_covered']."', '".$form_state['values']['depth_of_coverage']."', '".$form_state['values']['Pace_of_tutorial']."', '".$form_state['values']['workshop_venue']."', '".$form_state['values']['arrangements']."', '".$form_state['values']['content_relevent']."', '".$form_state['values']['content_comprehensive']."', '".$form_state['values']['content_easy']."', '".$form_state['values']['break_sufficient']."', '".$form_state['values']['good_mix']."', '".$form_state['values']['learning_experience']."', '".$form_state['values']['resource_person_knowledgeable']."', '".$form_state['values']['resource_person_responsive']."', '".$form_state['values']['like_most']."', '".$form_state['values']['like_least']."', '".$form_state['values']['improved']."', '".$form_state['values']['foss_recommend']."', '".$form_state['values']['why_or_whynot']."')";
		db_set_active('workshop_info');
		if(db_query($query)){
			drupal_set_message('Workshop Details Added Successfully');
		}else {
			drupal_set_message('Error Adding Workshop Details');
		}
		db_set_active('default');
		
	}
	function view_feedback($wc = NULL, $uid = NULL){
		$path = drupal_get_path('module', 'workshops');
		drupal_add_css($path . '/workshops.css', 'module');
		if($wc != NULL && $uid != NULL){
			$query = "select * from {workshop_feedback} where user_name='".$uid."' and workshop_code='".$wc."'";
			db_set_active('workshop_info');

			$result = db_query($query);
			db_set_active('default');	
			//$output = '<p class="back-to-completed-ws">Back</p>';
			$output = '<table class="feedback-message">';
 			while($fd = db_fetch_object($result)){
				$output .= '<tr><td>User Name</td>'.'<td>: '.$fd->user_name.'</td></tr>';
				$output .= '<tr><td>Name</td>'.'<td>: '.$fd->name.'</td></tr>';
				$output .= '<tr><td>Institution Name</td>'.'<td>: '.$fd->institution_name.'</td></tr>';
				$output .= '<tr><td>Email</td>'.'<td>: '.$fd->email.'</td></tr>';
				$output .= '<tr><td>Workshop Code</td>'.'<td>: '.$fd->workshop_code.'</td></tr>';
				$output .= '<tr><td>Range of topics covered in tutorial</td>'.'<td>: '.$fd->range_of_topics_covered.'</td></tr>';
				$output .= '<tr><td>Depth of coverage</td>'.'<td>: '.$fd->depth_of_coverage.'</td></tr>';
				$output .= '<tr><td>Pace of the tutorial</td>'.'<td>: '.$fd->Pace_of_tutorial.'</td></tr></table>';
				$mark_result = '<td>'.(($fd->workshop_venue == "Poor") ? '&#10004;' : '')."</td>".'<td>'.(($fd->workshop_venue == "Fair") ? '&#10004;' : '')."</td>"."<td>".(($fd->workshop_venue == "Average") ? '&#10004;' : '')."</td>"."<td>".(($fd->workshop_venue == "Good") ? '&#10004;' : '')."</td>"."<td>".(($fd->workshop_venue == "Excellent") ? '&#10004;' : '')."</td>"."<td>".(($fd->workshop_venue == "Not Sure") ? '&#10004;' : '')."</td>";
				$output .= "<table class='feedback-tick-mark'><tr><th class='big-column'></th><th>Poor</th><th>Fair</th><th>Average</th><th>Good</th><th>Excellent</th><th>Not Sure</th></tr>";
				

				$output .= "<tr><td class='big-column'>The workshop venue was 'Comfortable'</td>".'<td>'.(($fd->workshop_venue == "Poor") ? '&#10004;' : '')."</td>".'<td>'.(($fd->workshop_venue == "Fair") ? '&#10004;' : '')."</td>"."<td>".(($fd->workshop_venue == "Average") ? '&#10004;' : '')."</td>"."<td>".(($fd->workshop_venue == "Good") ? '&#10004;' : '')."</td>"."<td>".(($fd->workshop_venue == "Excellent") ? '&#10004;' : '')."</td>"."<td>".(($fd->workshop_venue == "Not Sure") ? '&#10004;' : '')."</td>"."</tr>";
				

				$output .= '<tr><td class="big-column">The arrangements for the workshop were good</td>'.'<td>'.(($fd->arrangements == "Poor") ? '&#10004;' : '')."</td>".'<td>'.(($fd->arrangements == "Fair") ? '&#10004;' : '')."</td>"."<td>".(($fd->arrangements == "Average") ? '&#10004;' : '')."</td>"."<td>".(($fd->arrangements == "Good") ? '&#10004;' : '')."</td>"."<td>".(($fd->arrangements == "Excellent") ? '&#10004;' : '')."</td>"."<td>".(($fd->arrangements == "Not Sure") ? '&#10004;' : '')."</td>"."</tr>";
				$output .= '<tr><td class="big-column">The workshop content was "Relevant"</td>'.'<td>'.(($fd->content_relevent == "Poor") ? '&#10004;' : '')."</td>".'<td>'.(($fd->content_relevent == "Fair") ? '&#10004;' : '')."</td>"."<td>".(($fd->content_relevent == "Average") ? '&#10004;' : '')."</td>"."<td>".(($fd->content_relevent == "Good") ? '&#10004;' : '')."</td>"."<td>".(($fd->content_relevent == "Excellent") ? '&#10004;' : '')."</td>"."<td>".(($fd->content_relevent == "Not Sure") ? '&#10004;' : '')."</td>"."</tr>";
				$output .= '<tr><td class="big-column">The workshop content was "Comprehensive"</td>'.'<td>'.(($fd->content_comprehensive == "Poor") ? '&#10004;' : '')."</td>".'<td>'.(($fd->content_comprehensive == "Fair") ? '&#10004;' : '')."</td>"."<td>".(($fd->content_comprehensive == "Average") ? '&#10004;' : '')."</td>"."<td>".(($fd->content_comprehensive == "Good") ? '&#10004;' : '')."</td>"."<td>".(($fd->content_comprehensive == "Excellent") ? '&#10004;' : '')."</td>"."<td>".(($fd->content_comprehensive == "Not Sure") ? '&#10004;' : '')."</td>"."</tr>";
				$output .= '<tr><td class="big-column">The workshop content was "Easy to Understand"</td>'.'<td>'.(($fd->content_easy == "Poor") ? '&#10004;' : '')."</td>".'<td>'.(($fd->content_easy == "Fair") ? '&#10004;' : '')."</td>"."<td>".(($fd->content_easy == "Average") ? '&#10004;' : '')."</td>"."<td>".(($fd->content_easy == "Good") ? '&#10004;' : '')."</td>"."<td>".(($fd->content_easy == "Excellent") ? '&#10004;' : '')."</td>"."<td>".(($fd->content_easy == "Not Sure") ? '&#10004;' : '')."</td>"."</tr>";
				$output .= '<tr><td class="big-column">Breaks were sufficient during the workshop</td>'.'<td>'.(($fd->break_sufficient == "Poor") ? '&#10004;' : '')."</td>".'<td>'.(($fd->break_sufficient == "Fair") ? '&#10004;' : '')."</td>"."<td>".(($fd->break_sufficient == "Average") ? '&#10004;' : '')."</td>"."<td>".(($fd->break_sufficient == "Good") ? '&#10004;' : '')."</td>"."<td>".(($fd->break_sufficient == "Excellent") ? '&#10004;' : '')."</td>"."<td>".(($fd->break_sufficient == "Not Sure") ? '&#10004;' : '')."</td>"."</tr>";
				$output .= '<tr><td class="big-column">A good mix between listening and activities during the workshop</td>'.'<td>'.(($fd->good_mix == "Poor") ? '&#10004;' : '')."</td>".'<td>'.(($fd->good_mix == "Fair") ? '&#10004;' : '')."</td>"."<td>".(($fd->good_mix == "Average") ? '&#10004;' : '')."</td>"."<td>".(($fd->good_mix == "Good") ? '&#10004;' : '')."</td>"."<td>".(($fd->good_mix == "Excellent") ? '&#10004;' : '')."</td>"."<td>".(($fd->good_mix == "Not Sure") ? '&#10004;' : '')."</td>"."</tr>";
				$output .= '<tr><td class="big-column">The workshop was a useful learning experience</td>'.'<td>'.(($fd->learning_experience == "Poor") ? '&#10004;' : '')."</td>".'<td>'.(($fd->learning_experience == "Fair") ? '&#10004;' : '')."</td>"."<td>".(($fd->learning_experience == "Average") ? '&#10004;' : '')."</td>"."<td>".(($fd->learning_experience == "Good") ? '&#10004;' : '')."</td>"."<td>".(($fd->learning_experience == "Excellent") ? '&#10004;' : '')."</td>"."<td>".(($fd->learning_experience == "Not Sure") ? '&#10004;' : '')."</td>"."</tr>";
				$output .= '<tr><td class="big-column">The resource persons/domain experts were knowledgeable and well prepared</td>'.'<td>'.(($fd->resource_person_knowledgeable == "Poor") ? '&#10004;' : '')."</td>".'<td>'.(($fd->resource_person_knowledgeable == "Fair") ? '&#10004;' : '')."</td>"."<td>".(($fd->resource_person_knowledgeable == "Average") ? '&#10004;' : '')."</td>"."<td>".(($fd->resource_person_knowledgeable == "Good") ? '&#10004;' : '')."</td>"."<td>".(($fd->resource_person_knowledgeable == "Excellent") ? '&#10004;' : '')."</td>"."<td>".(($fd->resource_person_knowledgeable == "Not Sure") ? '&#10004;' : '')."</td>"."</tr>";
				$output .= '<tr><td class="big-column">The resource persons/domain experts were "Responsive to participants questions"</td>'.'<td>'.(($fd->resource_person_responsive == "Poor") ? '&#10004;' : '')."</td>".'<td>'.(($fd->resource_person_responsive == "Fair") ? '&#10004;' : '')."</td>"."<td>".(($fd->resource_person_responsive == "Average") ? '&#10004;' : '')."</td>"."<td>".(($fd->resource_person_responsive == "Good") ? '&#10004;' : '')."</td>"."<td>".(($fd->resource_person_responsive == "Excellent") ? '&#10004;' : '')."</td>"."<td>".(($fd->resource_person_responsive == "Not Sure") ? '&#10004;' : '')."</td>"."</tr></table>";

				$output .= '<div id="qustion-and-answer"><p class="question"> What did you like most about this workshop?</p><p class="answer">'.$fd->like_most.'<p></div>';
				$output .= '<div id="qustion-and-answer"><p class="question"> What did you like least about this workshop?</leastp><p class="answer">'.$fd->like_least.'<p></div>';
				$output .= '<div id="qustion-and-answer"><p class="question"> How could this workshop be improved ?</p><p class="answer">'.$fd->like_improved.'<p></div>';
				$output .= '<div id="qustion-and-answer"><p class="question">Would you recommend the FOSS, Spoken Tutorial Project and its activities to others?</p><p class="answer">'.$fd->foss_recommend.'<p></div>';
				$output .= '<div id="qustion-and-answer"><p class="question"> Why/why not </p><p class="answer">'.$fd->why_or_whynot.'<p></div>';
 				
 			}
 			//$output .= '</table>';
		}else{
			$output = "<p>There is no data</p>";
		}
		return $output;

	}


?>