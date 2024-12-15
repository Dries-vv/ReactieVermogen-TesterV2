<?php

$hostname = "localhost";
$username = "root";
$password = "";
$database = "sensor_db";

$conn = mysqli_connect($hostname, $username, $password, $database);

if (!$conn) {
    die("Connection failed: " . mysqli_connect_error());
}

echo "Database connection is OK<br>";

if (isset($_POST["temperature"]) && isset($_POST["humidity"])) {
    
    // Sanitize input
    $t = filter_input(INPUT_POST, 'temperature', FILTER_SANITIZE_NUMBER_FLOAT, FILTER_FLAG_ALLOW_FRACTION);
    $h = filter_input(INPUT_POST, 'humidity', FILTER_SANITIZE_NUMBER_FLOAT, FILTER_FLAG_ALLOW_FRACTION);

    // Prepare the SQL statement
    $stmt = $conn->prepare("INSERT INTO dht11 (temperature, humidity) VALUES (?, ?)");
    if ($stmt === false) {
        die("Prepare failed: " . $conn->error);
    }

    // Bind parameters
    $stmt->bind_param("dd", $t, $h); // "dd" means two doubles

    // Execute the statement
    if ($stmt->execute()) {
        echo "New record created successfully";
    } else {
        echo "Error: " . $stmt->error;
    }

    // Close the statement
    $stmt->close();
}

// Close the connection
$conn->close();
?>
