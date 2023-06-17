<?php

// Connect to MySQL database
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "battery";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Get the values from the HTTP GET request
$id = isset($_GET["id"]) ? $_GET["id"] : null;
$voltage = isset($_GET["voltage"]) ? $_GET["voltage"] : null;
$current = isset($_GET["current"]) ? $_GET["current"] : null;
$soc = isset($_GET["soc"]) ? $_GET["soc"] : null;
$remainingAh = isset($_GET["remainingAh"]) ? $_GET["remainingAh"] : null;
$temp1 = isset($_GET["temp1"]) ? $_GET["temp1"] : null;
$temp2 = isset($_GET["temp2"]) ? $_GET["temp2"] : null;
$temp3 = isset($_GET["temp3"]) ? $_GET["temp3"] : null;
$temp4 = isset($_GET["temp4"]) ? $_GET["temp4"] : null;
$temp5 = isset($_GET["temp5"]) ? $_GET["temp5"] : null;
$serialnumber = isset($_GET["serialnumber"]) ? $_GET["serialnumber"] : null;

// Insert the values into the database
$sql = "INSERT INTO gt_can_data (id, voltage, current, soc, remainingAh, temp1, temp2, temp3, temp4, temp5, serialnumber) 
        VALUES ('$id', '$voltage', '$current', '$soc', '$remainingAh', '$temp1', '$temp2', '$temp3', '$temp4', '$temp5', '$serialnumber')";

if ($conn->query($sql) === TRUE) {
    echo "Data inserted successfully";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}

// Close the database connection
$conn->close();

?>
<?php
