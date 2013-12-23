-- This test checks if a query string has been given.
-- It sends the file identified by the query string.
-- Do not use it in a real server in this way!

if not mg.request_info.query_string then
    mg.write("HTTP/1.0 200 OK\r\n")
    mg.write("Connection: close\r\n")
    mg.write("Content-Type: text/html; charset=utf-8\r\n")
    mg.write("\r\n")
    mg.write("<html><head><title>Civetweb Lua script test page 3</title></head>\r\n")
    mg.write("<body>No query string!</body></html>\r\n")
elseif mg.request_info.query_string:match("/") or mg.request_info.query_string:match("\\") then
    mg.write("HTTP/1.0 403 Forbidden\r\n")
    mg.write("Connection: close\r\n")
    mg.write("Content-Type: text/html; charset=utf-8\r\n")
    mg.write("\r\n")
    mg.write("<html><head><title>Civetweb Lua script test page 3</title></head>\r\n")
    mg.write("<body>No access!</body></html>\r\n")
else
    filename = mg.document_root .. "/" .. mg.request_info.query_string
    mg.send_file(filename)
end
