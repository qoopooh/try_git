<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>Hello World!</title>
  </head>
  <body>
    <?php
      $database   = "sd_app";
      $user       = "root";
      $password   = "sddba";
      $host       = "mysql";
      $connection = new PDO("mysql:host={$host};dbname={$database};charset=utf8", $user, $password);
      //$query      = $connection->query("SELECT TABLE_NAME FROM information_schema.TABLES WHERE TABLE_TYPE='BASE TABLE'");
      //$result      = $connection->query("SELECT * FROM w_user");
      $result      = $connection->query("SELECT * FROM w_user");

      if ($result)
        echo "<p>1</p>";
      else
        echo "<p>2</p>";
      //if ($result->num_rows < 1) {
        //echo "<p>There are no row in query \"{$database}\".</p>";
      //} else {
        //echo "<p>Database \"{$database}\" has the following row:</p>";
        //echo "<ul>";

        //while($row = $result->fetch_assoc()) {
          //echo "<li>id: " . $row["id"]. " - Name: </li>";
        //}
        //echo "</ul>";
      //}
    ?>
  </body>
</html>
