mg.write("HTTP/1.0 200 OK\r\n")
mg.write("Content-Type: text/plain\r\n")
mg.write("\r\n")
mg.write(mg.request_info.request_method .. " " .. mg.request_info.request_uri .. "  HTTP/" .. mg.request_info.http_version .. "\r\n")
for k,v in pairs(mg.request_info.http_headers) do
  mg.write(k .. ": " .. v .. "\r\n")
end
mg.write("\r\n")

repeat
  local r = mg.read()
  if (r) then
    mg.write(r)
  end
until not r

