<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Sensor Data Submission</title>
    <link rel="stylesheet" href="styles.css">
    <!-- <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/css/bootstrap.min.css" rel="stylesheet"> -->

    <!-- Latest compiled JavaScript -->
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/js/bootstrap.bundle.min.js"></script>

    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.7.1/jquery.min.js"></script>

</head>
<?php
session_start();

// var_dump($_SESSION);
// $_SESSION["tempature"]

?>

<body>
    <div class="container">
        <h1>Submit Sensor Data</h1>
        <form id="sensorForm" action="submit.php" method="POST">
            <label for="temperature">Temperature (°C):</label>
            <input type="number" id="temperature" name="temperature" required>

            <label for="humidity">Humidity (%):</label>
            <input type="number" id="humidity" name="humidity" required>

            <button type="submit">Submit Data</button>
        </form>
        <div id="response"></div>
    </div>

    <script>
        document.getElementById('sensorForm').addEventListener('submit', function(event) {
            event.preventDefault();
            const formData = new FormData(this);

            fetch('./includes/data-sent.php', {
                    method: 'POST',
                    body: formData
                })
                .then(response => response.text())
                .then(data => {
                    document.getElementById('response').innerHTML = data;
                })
                .catch(error => {
                    console.error('Error:', error);
                });
        });


        // A $( document ).ready() block.
        $(document).ready(function() {
            loadData()
        });

        function loadData() {
            $.ajax({
                url: './includes/recieve-data.php',
                method: 'POST',
                data: {},
                dataType: 'json',
                success: function(data) {
                    console.log('Rating updated successfully');
                    console.log(data);
                },
                error: function(data) {
                    console.log('Error updating rating');
                    console.log(data);
                    
                }
            })
        }
    </script>
</body>

</html>