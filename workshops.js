$(document).ready(function(){
 webroot = 'http://'+location.hostname+'/tester/workshops/';
 loading_image = "<img src='http://"+location.hostname+"/tester/ajax-loader.gif' />";
	$('.college_state').change(function(){
		if($('.college_state').val() != ''){
			$.ajax({
				type : 'POST',
				url : webroot + "college/get_college_details",
				data : {
					'state_code' : $('.college_state').val() 
				},
				success : function(data){
					//console.log(data);
					output = JSON.parse(data);
					console.log(output);
					var ac_code_str = '';
					if(output.ac_count){
						var aCode = parseInt(output.ac_count);
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
	$('#resource_person_state').click(function(){
		var selected_states = "";
		var selObj = document.getElementById('resource_person_state');
		var i;
		for (i = 0; i < selObj.options.length; i++) {
			if (selObj.options[i].selected) {
				if(selected_states != ""){
					selected_states += "-";
				}
				selected_states += selObj.options[i].text;
			}
		}
		console.log(selected_states);
		console.log($(this).val());
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
});
