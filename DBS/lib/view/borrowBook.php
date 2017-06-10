<form id='borrow' action='borrowBook' class='center' method='post'>
<span class='area'>
	<i class='icon fa fa-id-card'></i>
	<input type='text' name='cid' id='borrowCard' placeholder='card ID' required="required"/>
</span>
<span class='area'>
	<i class='icon fa fa-book'></i>
	<input type='text' name='bid' id='borrowBook' placeholder='book ID'/>
</span>
<input type='submit' id='borrowSubmit' value='Borrow'/>
</form>
<table id='returnTable' ></table>
