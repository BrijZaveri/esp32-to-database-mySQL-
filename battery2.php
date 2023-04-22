<?php
class gt_can_voltage{
 public $link='';
 function __construct($voltage){
  $this->connect();
  $this->storeInDB($voltage);
 }
 
 function connect(){
  $this->link = mysqli_connect('localhost','root','joyebike@1234') or die('Cannot connect to the DB');
  mysqli_select_db($this->link,'battery') or die('Cannot select the DB');
 }
 
 function storeInDB($voltage){
  $query = "INSERT into gt_can_voltage SET voltage='".$voltage."'";
  $result = mysqli_query($this->link,$query) or die('Errant query:  '.$query);
 }
 
}

if(isset($_GET['voltage']) && !empty($_GET['voltage'])){
 $gt_can_voltage=new gt_can_voltage($_GET['voltage']);
}
?>
