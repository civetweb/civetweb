<?
-- This *.lp file simply runs the *.lua file in the same directory.
n = string.match(mg.request_info.uri, "^(.*)%.lp$")
if mg.system:find("Windows") then
    n = string.gsub(n, [[/]], [[\]])
end
n = mg.document_root .. n .. ".lua"
dofile(n)
?>