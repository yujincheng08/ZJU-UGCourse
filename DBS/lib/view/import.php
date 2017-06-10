<form id='addBook' action='import' class='center' method='post'>
<span class='area'>
	<i class='icon fa fa-book'></i>
	<input type='text' name='id' id='addid' placeholder='book ID' required/>
</span>
<span class='area'>
	<i class='icon fa fa-tags'></i>
	<input type='text' name='category' id='addCategory' placeholder='category' required/>
</span>
</br>
<span class='area'>
	<i class='icon fa fa-font'></i>
	<input type='text' name='title' id='addTitle' placeholder='title' required/>
</span>
<span class='area'>
	<i class='icon fa fa-university'></i>
	<input type='text' name='publish' id='addPublish' placeholder='publish' required/>
</span>
</br>
<span class='area'>
	<i class='icon fa-calendar'></i>
	<input type='text' name='year' id='addYear' placeholder='year' pattern='\d{4}' required/>
</span>
<span class='area'>
	<i class='icon fa-user-circle-o'></i>
	<input type='text' name='author' id='addAuthor' placeholder='author' required/>
</span>
</br>
<span class='area'>
	<i class='icon fa-usd'></i>
	<input type='text' name='price' id='addPrice' placeholder='price' pattern='\d{1,6}(\.\d{1,2})?' required/>
</span>
<span class='area'>
	<i class='icon fa-navicon'></i>
	<input type='text' name='stock' id='addStock' placeholder='stock' pattern='\d+' required/>
</span>
<button type='submit' id='addBookSubmit'>
	Import</button>
</form>

<form id='import' action='import' method='post' enctype='multipart/form-data' class='center'>
	<button class="upload">
	<input type='file' name='file' id='file' />
	Chose file
	</button>
	<input type='submit' value='submit' id='submit' />
</form>
