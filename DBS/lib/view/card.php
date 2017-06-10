<form id='newCard' action='card' class='center' method='post'>
<span class='area'>
	<i class='icon fa fa-user'></i>
	<input type='text' name='name' id='cardname' placeholder='name' required="required"/>
</span>
<span class='area'>
	<i class='icon fa fa-university'></i>
	<input type='text' name='unit' id='cardunit' placeholder='unit' required="required"/>
</span>
<span class='area'>
	<i class='icon fa fa-id-badge'></i>
	<select name='class' required="required">
	<option value='teacher'>teacher</option>
	<option value='student'>student</option>
</select>
</span>
<input type='submit' value='Add' id='cardsubmit'/>
</form>
<table id='cardTable'/>
<?php if(isset($data['error'])):?>
<p>Error occurs: <?=$data['error']?></p>
<?php else:?>
		<tr>
	<?php foreach($data['cardHead'] as $head) :?>
		<th><?=$head?></th>
	<?php endforeach; ?>
		<th>delete</th></tr>
	<?php foreach($data['card'] as $card) :?>
		<tr>
			<?php foreach($card as $attr => $value) :?>
			<td class='<?=$attr?>'><?=$value?></td>
			<?php endforeach; ?>
		<td class='delete'><button class='deleteCard' cid='<?=$card['id']?>'>
		Delete</button>
		</tr>
	<?php endforeach; ?>
<?php endif;?>
</table>
