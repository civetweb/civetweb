<?php
set_time_limit(20);
header('Content-Type: text/plain; charset=utf-8');
echo "CivetWeb Auto Flush Test\n";
for($i = 0; $i <10; $i++) {
	echo "Auto flush $i\n";
	@flush();
	@ob_flush();
	sleep(1);
}
echo "\nCurrently this test fails, only the first FLUSH is processed by the server.\n";
?>