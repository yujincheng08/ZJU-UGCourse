<table ID='prompt'>
<tr><th>Function</th><th>Description</th></tr>
<tr>
	<td>Login</td>
	<td>
	<p>Log in the libaray with ID and password. All functions except searching require loging in first. After loging in the library, you can manipulate the library.</p>
	<p>If you provide the right ID and password, you can successfully log in the library and the website will be redirected to the homepage in 5 seconds. Otherwise, it will show you error message.</p>
	</td>
</tr>
<tr>
	<td>Borrow</td>
	<td>Borrow book with provided book ID to the provided card ID. If only provided card ID, which must be provided, books this card has borrowed will be shown on the website.</td>
</tr>
<tr>
	<td>Return</td>
	<td>Return books for the provided card ID. Firstly, books the provided card ID has borrowed will be shown. Then, libarian can click the return button to return book.</td>
</tr>
<tr>
	<td>Card Manage</td>
	<td><p>Manage cards. In this page, libarian can delete a card by providing card ID. If this card had returned all the books it borrowed, the card will be deleted. Otherwise, error will occur.</p>
	<p>Libarian can also insert a new card into the libaray with name, unit and identity all provided.</p></td>
</tr>
<tr>
	<td>Search</td>
	<td>Search books in the libaray with book title, book ID and so on. Leave the input box empty to ignore the attributes. If all boxes empty, all books in the library will be shown.</td>
</tr>
<tr>
	<td>Import</td>
	<td><p>Import book(s) into the library. All information of a book is required, which means no boxes can be left empty.</p>
	<p>Librarian can import only one book a time using the input boxes. Notably, book ID should be unique. If the provided book ID conflits with an existing book, error will occur.</p>
	<p>Librarian can also import book batch by uploading a text file with the format containing (book ID, category, title, publish, year, author, price, stock ) in each line.</p>
	<p>PS: price is fixed point number with 6 digital integer part and 2 digital fraction part which can be ignored and year contains four digital number. Stock much be an integer.</p></td>
</tr>
<tr>
<td colspan='2'>
IE 9 and earlier versions are not supported! Please use Chrome or Edge or Firefox for better experience!
</td>
</tr>
</table>
