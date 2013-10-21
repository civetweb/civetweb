<?
function print(txt) mg.write(txt .. "\r\n") end
mg.write("HTTP/1.1 200 OK\r\n")
n = string.match(mg.request_info.uri, "^(.*)%.lp$")
n = string.gsub(n, [[/]], [[\]])
n = mg.document_root .. n .. ".cgi"
dofile(n)
?>