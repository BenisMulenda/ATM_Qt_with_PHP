<?php 

	
	/*
	* codes (010 - withdraw) (011 - deposit) (100 - receipt)
	* client (Holder Name, Holder Surname, Account No., PIN, Balance, Type)
	* client is a JSON file with the ID of the client
	* Functions: Withdraw (amount, Account No.) Deposit (Account No., amount) GenerateSummary(Account No. )
	* receive session token 
	* create a temp session cookie with the token 
	* and that token will be used to communicate with the client
	* future you can use encryption 
	*/
	header('Content-Type: application/json;charset=utf-8');
	$jsonData = ["id"=>"123", "title"=>"Cybersec"];
	
	echo json_encode($jsonData, JSON_PRETTY_PRINT);
	
	function 
?>
