mg.write("HTTP/1.0 200 OK\r\n")
mg.write("Content-Type: text/html\r\n")
mg.write("\r\n")
mg.write([[<html><body>
  <p>Lua error handler:</p>
  <p>Status code: ]])

mg.write(tostring(mg.request_info.status))

mg.write([[</p>
</body></html>
]])