$(document).ready(function(){
	$('.college_state').change(function(){
		var webroot = 'http://localhost/spokentutorial.org/workshops/'
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
				if(output){
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
		//$(this)append().after().html('<div class="form-item">Selected State</div>');
		//document.getElementById('lbl_selected_states').innerHTML = selected_states2;
		//if(selected_states == ""){
		//	document.getElementById('lbl_caption').innerHTML = "";
		//}else{
		//	document.getElementById('lbl_caption').innerHTML = "Selected States:";
		//}
	});
	$('.organiser_academic_code').change(function(){
		var webroot = 'http://localhost/spokentutorial.org/workshops/'
		this_data = $(this);
		$.ajax({
			type : 'POST',
			url : webroot + "get_academic_details",
			data : {
				'academic_code' : $('.organiser_academic_code').val() 
			},
			success : function(data){
				output = JSON.parse(data);
				if (output){
					html_data = "<table><tr><td>Institution Name</td><td>" + output.institution_name + "</td></tr>" + "<tr><td>State Code</td><td>" + output.state_code + "</td></tr>" + "<tr><td>City</td><td>" + output.city + "</td></tr></table>";
					$('#academic-details').html(html_data);
					$('.department').val(output.department);
				}else{
					html_data = "<p>Academic code not exists, Please check</p>";
					$('#academic-details').html(html_data);
				}	
				console.log(output);
			}
		});
	});

	$('.detail_workshop_code').change(function(){
		var webroot = 'http://localhost/spokentutorial.org/workshops/'
		this_data = $(this);
		$.ajax({
			type : 'POST',
			url : webroot + "get_workshop_details",
			data : {
				'workshop_code' : $('.detail_workshop_code').val() 
			},
			success : function(data){
				output = JSON.parse(data);
				if (output){
					html_data = "<h3>Entered Academic Details</h3><table><tr><td>FOSS Category</td><td>" + output.foss_category + "</td></tr>" + "<tr><td>Date</td><td>" + output.cfm_wkshop_date + "</td></tr>" + "<tr><td>Time</td><td>" + output.cfm_wkshop_time + "</td></tr></table>";
					$('#workshop-details').html(html_data);
				}else{
					html_data = "<p>Entered Workshop code not exists, Please check</p>";
					$('#workshop-details').html(html_data);
				}	
				console.log(output);
			}
		});
	});

});
