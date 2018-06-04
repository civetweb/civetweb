-- This test checks if a query string has been given.
-- It sends the file identified by the query string.
-- Do not use it in a real server in this way!

file = mg.get_var(mg.request_info.query_string, "file");


if not file then
    mg.write("HTTP/1.0 200 OK\r\n")
    mg.write("Connection: close\r\n")
    mg.write("Content-Type: text/html; charset=utf-8\r\n")
    mg.write("\r\n")
    mg.write("<html><head><title>CivetWeb Lua script test page 3</title></head>\r\n")
    mg.write("<body>No file parameter in query string!</body></html>\r\n")
    return
end

if file:match("/") or file:match("\\") then
    mg.write("HTTP/1.0 403 Forbidden\r\n")
    mg.write("Connection: close\r\n")
    mg.write("Content-Type: text/html; charset=utf-8\r\n")
    mg.write("\r\n")
    mg.write("<html><head><title>CivetWeb Lua script test page 3</title></head>\r\n")
    mg.write("<body>No access!</body></html>\r\n")
    return
end
    
filename = mg.document_root .. "/" .. file
mg.send_file(filename)

