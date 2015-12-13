-- Set keep_alive. The return value specifies if this is possible at all.
canKeepAlive = mg.keep_alive(true)

if canKeepAlive then
    -- Create the entire response in a string variable first. Content-Length will be set to the length of this string.
    reply = [[
        <html><body>
        <p>This is a Lua script supporting html keep-alive with the <a href="http://sourceforge.net/projects/civetweb/">Civetweb web server</a>.</p>
        <p>It works by setting the Content-Length header field properly.
        </body></html>
    ]]
else
    reply = "<html><body>Keep alive not possible!</body></html>"
end

-- First send the http headers
mg.write("HTTP/1.1 200 OK\r\n")
mg.write("Content-Type: text/html\r\n")
mg.write("Date: " .. os.date("!%a, %d %b %Y %H:%M:%S") .. " GMT\r\n")
mg.write("Cache-Control: no-cache\r\n")

if canKeepAlive then
    mg.write("Content-Length: " .. tostring(string.len(reply)) .. "\r\n")
    mg.write("Connection: keep-alive\r\n")
else
    mg.write("Connection: close\r\n")
end
mg.write("\r\n")

-- Finally send the content
mg.write(reply)

