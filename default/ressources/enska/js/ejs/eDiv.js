function toggleDiv(div_name)
{
	var Obj = document.getElementById(div_name);

	if (Obj.style.display == "block") {
		hideDiv(div_name);
	}
	else {
		showDiv(div_name);
	}
	
	return; 
}

function hideDiv(div_name)
{
	var Obj = document.getElementById(div_name);
	Obj.style.display = "none";
	
	return; 
}

function showDiv(div_name)
{
	var Obj = document.getElementById(div_name);
	Obj.style.display = "block";
	
	return; 
}