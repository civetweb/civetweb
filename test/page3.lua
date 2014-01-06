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
    file = mg.get_var(mg.request_info.query_string, "file");
    if not file then
        mg.write("HTTP/1.0 400 Bad Request\r\n")
        mg.write("Connection: close\r\n")
        mg.write("Content-Type: text/html; charset=utf-8\r\n")
        mg.write("\r\n")
        mg.write("<html>\r\n<head><title>Civetweb Lua script test page 3</title></head>\r\n")
        mg.write("<body>\r\nQuery string does not contain a 'file' variable.<br>\r\n")
        mg.write("Try <a href=\"?file=page3.lua&somevar=something\">?file=page3.lua&somevar=something</a>\r\n")
        mg.write("</body>\r\n</html>\r\n")
    else
        filename = mg.document_root .. "/" .. file
        mg.send_file(filename)
    end
end
