-- http://www.html5rocks.com/static/images/cors_server_flowchart.png

if not mg.request_info.http_headers.Origin then
  mg.write("HTTP/1.0 200 OK\r\n")
  mg.write("Connection: close\r\n")
  mg.write("Content-Type: text/html; charset=utf-8\r\n")
  mg.write("\r\n")
  mg.write("This test page should not be used directly. Open cors.html instead.")
  return
end

if mg.request_info.request_method == "OPTIONS" then

  local acrm = mg.request_info.http_headers['Access-Control-Request-Method'];
  if (acrm) then
    local acrh = nil -- mg.request_info.http_headers['Access-Control-Request-Header'];
    if (acrm~='PUT') then
      -- invalid request
      mg.write("HTTP/1.0 403 Forbidden\r\n")
      mg.write("Connection: close\r\n")
      mg.write("\r\n")
      return
    else
      -- preflight request
      mg.write("HTTP/1.0 200 OK\r\n")
      mg.write("Access-Control-Allow-Methods: PUT\r\n")
      if (acrh) then
        mg.write("Access-Control-Allow-Headers: " .. acrh .. "\r\n")
      end
      mg.write("Access-Control-Allow-Origin: *\r\n")
      mg.write("Connection: close\r\n")
      mg.write("Content-Type: text/html; charset=utf-8\r\n")
      mg.write("\r\n")
      return
    end
  end
end

-- actual request
if mg.request_info.request_method == "GET" then
  mg.write("HTTP/1.0 200 OK\r\n")
  mg.write("Access-Control-Allow-Origin: *\r\n")
  mg.write("Connection: close\r\n")
  mg.write("Content-Type: text/html; charset=utf-8\r\n")
  mg.write("\r\n")
  mg.write([[<!DOCTYPE html>
  <html>
  <head><title>CORS dynamic GET test reply - test OK</title></head>
  <body>This should never be shown</body>
  </html>
  ]])
  return
end


if mg.request_info.request_method == "PUT" then
  mg.write("HTTP/1.0 200 OK\r\n")
  mg.write("Access-Control-Allow-Origin: *\r\n")
  mg.write("Connection: close\r\n")
  mg.write("Content-Type: text/html; charset=utf-8\r\n")
  mg.write("\r\n")
  mg.write([[<!DOCTYPE html>
  <html>
  <head><title>CORS dynamic PUT test reply - test OK</title></head>
  <body>This should never be shown</body>
  </html>
  ]])
  return
end

mg.write("HTTP/1.0 403 Forbidden\r\n")
mg.write("Connection: close\r\n")
mg.write("\r\n")
