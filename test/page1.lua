loc = string.gsub(mg.request_info.request_uri, "page1.lua", "page.lua")

mg.write("HTTP/1.0 301 Moved Permanently\r\n")
mg.write("Location: " .. loc .. "\r\n")
mg.write("Content-Length: 0\r\n")
mg.write("Connection: close\r\n")
mg.write("\r\n")

