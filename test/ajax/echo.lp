<?
n = string.match(mg.request_info.uri, "^(.*)%.lp$")
n = string.gsub(n, [[/]], [[\]])
n = mg.document_root .. n .. ".lua"
dofile(n)
?>