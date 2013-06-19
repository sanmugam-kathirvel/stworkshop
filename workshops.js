$(document).ready(function(){
	webroot = 'http://'+location.hostname+'/workshops/';
	loading_image = "<img src='http://"+location.hostname+"/ajax-loader.gif' />";
	tmp_disp = $('.resource_center').val();
	$('#edit-resource-center-1').change(function(){
		var disp_val = $(this).val();
		if(disp_val == 1){
			$('.resource_center_details').css({'display':'block'});
		}else {
			$('.resource_center_details').css({'display':'none'});
		}
	});
	$('#edit-resource-center-0').change(function(){
		var disp_val = $(this).val();
		if(disp_val == 1){
			$('.resource_center_details').css({'display':'block'});
		}else {
			$('.resource_center_details').css({'display':'none'});
		}
	});
	$('.college_state').change(function(){
		if($('.college_state').val() != ''){
			$.ajax({
				type : 'POST',
				url : webroot + "college/get_college_details",
				data : {
					'state_code' : $('.college_state').val() 
				},
				beforeSend: function() {
					field_data = $('.coll_academic_code_div').html();
					$('.coll_academic_code_div').html(loading_image);
				},
				success : function(data){
					//console.log(data);
					$('.coll_academic_code_div').html(field_data);
					output = JSON.parse(data);
					var ac_code_str = '';
					if((output.length == 2) && output[1]){
						var tmp = output[1].replace(/^0+(?!\.|$)/, '');
						var aCode = parseInt(tmp);
						aCode += 1;
						if (aCode < 10)
							ac_code_str = '0000' + aCode;
						else if (aCode < 99)
							ac_code_str = '000' + aCode;
						else if (aCode < 999)
							ac_code_str = '00' + aCode;
						else if (aCode < 9999)
							ac_code_str = '0' + aCode;
						ac_code_str = $('.college_state').val() + "-" + ac_code_str;
						console.log(ac_code_str);
						$('.college_academic_code').val(ac_code_str);
					}else{
						alert('Error fetching academic code, please refresh the page and try again');
					}
				}
			});
		}else{
			$('.college_academic_code').val('');
		}
	});
// resurce preson
	$('#resource_person_state').change(function(){
		// var selected_states = "";
		var selected_states_html = "";
		var selObj = document.getElementById('resource_person_state');
		var i;
		for (i = 0; i < selObj.options.length; i++) {
			if (selObj.options[i].selected) {
				if(selected_states_html != ""){
					// selected_states += "-";
					selected_states_html += "<br />";
				}
				// selected_states += selObj.options[i].text;
				selected_states_html += selObj.options[i].text;
			}
		}
		// console.log(selected_states_html);
		// console.log($(this).val());
		$('#rp-sel-states').html("<label><h1 class='title'>Selected States:</h1></label><div style='float: left;'>"+selected_states_html+"</div>");
	});
	$('.organiser_academic_code').change(function(){
		
		this_data = $(this);
		if(this_data != ''){
			$.ajax({
				type : 'POST',
				url : webroot + "get_academic_details",
				data : {
					'academic_code' : $('.organiser_academic_code').val() 
				},
				success : function(data){
					output = JSON.parse(data);
					if (output.academic){
						html_data = "<table><tr><td>Institution Name</td><td>" + output.academic.institution_name + "</td></tr>" + "<tr><td>State Code</td><td>" + output.academic.state_code + "</td></tr>" + "<tr><td>City</td><td>" + output.academic.city + "</td></tr></table>";
						$('#academic-details').html(html_data);
					}else{
						html_data = "<p>Academic code not exists, Please check</p>";
						$('#academic-details').html(html_data);
						$('.organiser_academic_code').val('');
						$(this_data).focus();
					}	
					console.log(output);
				}
			});
		}
	});

	$('.detail_workshop_code').change(function(){
		
		this_data = $(this);
		if(this_data != ''){
			$.ajax({
				type : 'POST',
				url : webroot + "get_workshop_details",
				data : {
					'workshop_code' : $('.detail_workshop_code').val() 
				},
				success : function(data){
					output = JSON.parse(data);
					if (output){
						html_data = "<h3>Academic Details</h3><table><tr><td>FOSS Category</td><td>" + output.foss_category + "</td></tr>" + "<tr><td>Date</td><td>" + output.cfm_wkshop_date + "</td></tr>" + "<tr><td>Time</td><td>" + output.cfm_wkshop_time + "</td></tr></table>";
						$('#workshop-details').html(html_data);
					}else{
						html_data = "<p>Entered Workshop code not exists, Please check</p>";
						$('#workshop-details').html(html_data);
					}
					console.log(output);
				}
			});
		}
	});

	$('.user_name').change(function(){
		
		this_data = $(this);
		if(this_data != ''){
			$.ajax({
				type : 'POST',
				url : webroot + "get_feedback_detail",
				data : {
					'user_name' : $('.user_name').val()
				},
				success : function(data){
					output = JSON.parse(data);
					if (output){
						$('.participant_name').val(output.firstname);
						$('.feedback_email').val(output.email);
					}else{
						$('.participant_name').val('');
						$('.feedback_email').val('');
						$('#feedback-error').html('<p style="color:red">Please, enter currect User Name</p>');
					}	
				}
			});
		}
	});
	$('.feedback_workshop_code').change(function(){
		
		this_data = $(this);
		$.ajax({
			type : 'POST',
			url : webroot + "get_feedback_detail",
			data : {
				'workshop_code' : $('.feedback_workshop_code').val() 
			},
			success : function(data){
				output = JSON.parse(data);
				console.log(data);
				if (output){
					$('.institution_name').val(output.institution_name);					
				}else{
					$('.institution_name').val('');
					$('#feedback-error').html('<p style="color:red">Please, enter currect workshop code</p>');
				}	
			}
		});
	});
	//$('.foss_recommend').change(function(){
		//console.log($(this).parent('div').parent().next());

	//});
	$('.foss_category').change(function(){
		foss = $(this).val();	
		$.ajax({
			type : 'POST',
			url : webroot + "get_lang",
			data : {
				'foss' : foss
			},
			beforeSend: function() {
			    field_data = $('.ws-lang').html();
			    $('.ws-lang').html(loading_image);
			},
			success : function(data){
				$('.ws-lang').html(field_data);
				var html_data = "<option val=''>-- Select --</option>";
				output = JSON.parse(data);
				html_data = "<option value=''>-- Select --</option>";
				if(output){
					for (var i=0; i < output.length; i++){
						html_data += "<option value='"+ output[i].language +"'>" + output[i].language + "</option>";	
					}
					$('.pref_language').html(html_data);
				}else{
					alert('Somthing wrong, Please refresh page');
				}
			}
		});
	});

	//check email
	$('.rp_user_email').blur(function(){
		email = $(this).val();
		$.ajax({
			type : 'POST',
			url : webroot + "check_email",
			data : {
				'email' : email
			},
			//beforeSend: function() {
			  //  field_data = $('.ws-lang').html();
			    //$('.ws-lang').html(loading_image);
			//},
			success : function(data){
				output = JSON.parse(data);
				if(output){
					if(output.name){
						$('.rp_user_name').val(output.name);
						$('.rp_user_pass_container').css({'display':'none'});
						$('#edit-uid').val(output.uid);
						$('.org-user-pass').css({'display' : 'none'});
					}else{
						$('.rp_user_name').val('');
						$('.rp_user_pass').val('');
						$('#edit-uid').val('');
						$('.rp_user_pass_container').css({'display':'block'});
						$('.org-user-pass').css({'display' : 'block'});
						//$('.rp_user_email').focus();
					}
				}else{
					alert('Somthing wrong, Please refresh page');
				}
			}
		});
	});
	
	$('.detail_test_code').change(function(){
		var test_code = $('.detail_test_code').val();
		$.ajax({
			type : 'POST',
			url : webroot + "get_batches",
			data : {
				'test_code' : test_code
			},
			success : function(data){
				output = JSON.parse(data);
				if(output != 0){
					var i = 1;
					var html_data = '<option value>Select Batch Number</option>';
					for(i = 1; i <= output; i++){
						html_data += '<option value="' + i + '">' + i + '</option>';
					}
					$('.batch_no_fill').html(html_data);
				}else{
					$('.detail_test_code').val('');
					alert('Invalid test code!');
				}
			}
		});
	});
	
	$('.add_reqs_state').change(function(){
		var state_code = $('.add_reqs_state').val();
		var html_data = '<option value>-- select --</option>';
		$('.reqs_department').html(html_data);
		if(state_code != '') {
			$.ajax({
				type : 'POST',
				url : webroot + "get_org_institutions",
				data : {
					'state_code' : state_code
				},
				beforeSend: function() {
					field_data = $('.reqs_academic_code_div').html();
					$('.reqs_academic_code_div').html(loading_image);
				},
				success : function(data){
					$('.reqs_academic_code_div').html(field_data);
					output = JSON.parse(data);
					if(output != 0){
						var i = 0;
						for(i = 0; i < output.length; i++){
							html_data += '<option value="' + output[i].academic_code + '">' + output[i].institution_name + ", " + output[i].city + ", " + output[i].pincode + '</option>';
						}
						//console.log(html_data);
						$('.add_reqs_academic_code').html(html_data);
					}else{
						$('.add_reqs_academic_code').html(html_data);
					}
					
					// departments
					$('.add_reqs_academic_code').change(function(){
						var academic_code = $('.add_reqs_academic_code').val();
						var html_data = '<option value>-- select --</option>';

						if(academic_code != '') {
							$.ajax({
								type : 'POST',
								url : webroot + "get_reqs_invigilator",
								data : {
									'academic_code' : academic_code
								},
								beforeSend: function() {
									field_data = $('.add_reqs_invigilator_div').html();
									$('.add_reqs_invigilator_div').html(loading_image);
								},
								success : function(data){
									$('.add_reqs_invigilator_div').html(field_data);
									output = JSON.parse(data);
									if(output != 0){
										var i = 0;
										for(i = 0; i < output.length; i++){
											html_data += '<option value="' + output[i].invigilator_id + '">' + output[i].invigilator_name + '</option>';
										}
										$('.add_invigilator_sel').html(html_data);
									}
								}
							});
						}
					});
				}
			});
		}else{
			$('.add_reqs_academic_code').html(html_data);
		}
	});
	// to select all 
	$('.select-all').attr('checked', false);
	$('.select-all').click(function (event) {
		var selected = this.checked;
		// Iterate each checkbox
		$(':checkbox').each(function () {    this.checked = selected; });

	});
});
