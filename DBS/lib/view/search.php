<form id='search' action='search' class='center' method='post'>
<span class='area'>
	<i class='icon fa fa-book'></i>
	<input type='text' name='id' id='searchid' placeholder='book ID'/>
</span>
<span class='area'>
	<i class='icon fa fa-tags'></i>
	<input type='text' name='category' id='searchCategory' placeholder='category'/>
</span>
</br>
<span class='area'>
	<i class='icon fa fa-font'></i>
	<input type='text' name='title' id='searchTitle' placeholder='title'/>
</span>
<span class='area'>
	<i class='icon fa fa-university'></i>
	<input type='text' name='publish' id='searchPublish' placeholder='publish'/>
</span>
</br>
<span class='area'>
	<i class='icon fa-calendar-minus-o'></i>
	<input type='text' name='startYear' id='searchStartYear' placeholder='start year'/>
</span>
<span class='area'>
	<i class='icon fa-calendar-plus-o'></i>
	<input type='text' name='endYear' id='searchEndYear' placeholder='end year'/>
</span>
<span class='area'>
	<i class='icon fa-user-circle-o'></i>
	<input type='text' name='author' id='searchAuthor' placeholder='author'/>
</span>
</br>
<span class='area'>
	<i class='icon fa-minus-square-o'></i>
	<input type='text' name='startPrice' id='searchStartPrice' placeholder='start price'/>
</span>
<span class='area'>
	<i class='icon fa-plus-square-o'></i>
<input type='text' name='endPrice' id='searchStartPrice' placeholder='end price'/>
</span>
<button type='submit' id='searchSubmit'>
	Search <i class='fa fa-search'></i></button>
</form>
<table id='searchTable'></table>
