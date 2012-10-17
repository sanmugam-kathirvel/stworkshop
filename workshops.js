$(document).ready(function(){
	$('.college_state').change(function(){
		var webroot = 'http://localhost/spokentutorial.org/workshops/'
		$.ajax({
			type : 'POST',
			url : webroot + "get_college_details",
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
		//document.getElementById('lbl_selected_states').innerHTML = selected_states2;
		//if(selected_states == ""){
		//	document.getElementById('lbl_caption').innerHTML = "";
		//}else{
		//	document.getElementById('lbl_caption').innerHTML = "Selected States:";
		//}
	});

});
