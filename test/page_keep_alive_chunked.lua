-- Set keep_alive. The return value specifies if this is possible at all.
canKeepAlive = mg.keep_alive(true)
now = os.date("!%a, %d %b %Y %H:%M:%S")

-- First send the http headers
mg.write("HTTP/1.1 200 OK\r\n")
mg.write("Content-Type: text/html\r\n")
mg.write("Date: " .. now .. " GMT\r\n")
mg.write("Cache-Control: no-cache\r\n")
mg.write("Last-Modified: " .. now .. " GMT\r\n")
if not canKeepAlive then
    mg.write("Connection: close\r\n")
    mg.write("\r\n")
    mg.write("<html><body>Keep alive not possible!</body></html>")
    return
end
if mg.request_info.http_version ~= "1.1" then
    -- wget will use HTTP/1.0 and Connection: keep-alive, so chunked transfer is not possible
    mg.write("Connection: close\r\n")
    mg.write("\r\n")
    mg.write("<html><body>Chunked transfer is only possible for HTTP/1.1 requests!</body></html>")
    mg.keep_alive(false)
    return
end

-- use chunked encoding (http://www.jmarshall.com/easy/http/#http1.1c2)
mg.write("Cache-Control: max-age=0, must-revalidate\r\n")
--mg.write("Cache-Control: no-cache\r\n")
--mg.write("Cache-Control: no-store\r\n")
mg.write("Connection: keep-alive\r\n")
mg.write("Transfer-Encoding: chunked\r\n")
mg.write("\r\n")

-- function to send a chunk
function send(str)
    local len = string.len(str)
    mg.write(string.format("%x\r\n", len))
    mg.write(str.."\r\n")
end

-- send the chunks
send("<html>")
send("<head><title>Civetweb Lua script chunked transfer test page</title></head>")
send("<body>\n")

fileCnt = 0
if lfs then
    send("Files in " .. lfs.currentdir())
    send('\n<table border="1">\n')
    send('<tr><th>name</th><th>type</th><th>size</th></tr>\n')
    for f in lfs.dir(".") do
        local at = lfs.attributes(f);
        if at then
          send('<tr><td>' .. f .. '</td><td>' .. at.mode .. '</td><td>' .. at.size .. '</td></tr>\n')
        end
        fileCnt = fileCnt + 1
    end
    send("</table>\n")
end

send(fileCnt .. " entries (" .. now .. " GMT)\n")
send("</body>")
send("</html>")

-- end
send("")
