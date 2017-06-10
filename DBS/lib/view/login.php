<?php if(!$data['user']):?>
<form id='login' action='login' method='post'>
<span class='area'>
	<i class='icon fa fa-user'></i>
	<input type='text' name='user' id='loginuser' placeholder='ID' required='required'/>
</span>
<span class='area'>
	<i class='icon fa fa-lock'></i>
	<input type='password' name='password' id='loginpasswd' placeholder='Passowrd' required='required'/>
</span>
	<input type='submit' value='login' id='loginsubmit' />
</form>
<?php endif;?>
