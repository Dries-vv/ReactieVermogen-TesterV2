<?php
session_start();
$hostname = "localhost";
$username = "root";
$password = "";
$database = "sensor_db";

$conn = mysqli_connect($hostname, $username, $password, $database);

$response = array();

$sql = "SELECT `temperature` FROM `dht11`";
$statement = $conn->prepare($sql);
$statement->execute();
$results = $statement->get_result();
$row = $results->fetch_assoc();
$response = $row;
// var_dump($_SESSION);
$_SESSION ['temperature'] = $row['temperature'];

// var_dump($response);
echo json_encode($response);
?>