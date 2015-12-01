
msg=[[<html><body>
<p>Exit CivetWeb</p>
</body></html>
]]

mg.write("HTTP/1.0 200 OK\r\n")
mg.write("Connection: close\r\n")
mg.write("Content-Length: " .. #msg .. "\r\n")
mg.write("Content-Type: text/html\r\n")
mg.write("\r\n")
mg.write(msg)

os.exit(0)

