<?php
// Server test options:
//  ./civetweb -document_root test -cgi_interpreter /usr/bin/php-cgi -allow_sendfile_call no -num_threads 2 -cgi_buffering no &

set_time_limit(20);
header('Content-Type: text/plain; charset=utf-8');

echo "CivetWeb Flush Test:\nPrint one line every second.\n\n";
for($i = 1; $i <= 10; $i++) {
	@printf("Line: %2d / 10\n", $i);
	@flush();
	@ob_flush();
	sleep(1);
}
echo "\nEnd of test.\n";
?>