function send_ok()
	mg.write("HTTP/1.0 200 OK\r\n")
	mg.write("Connection: close\r\n")
	mg.write("Date: " .. os.date("%a, %d %b %Y %H:%M:%S GMT") .. "\r\n")
end


function send_not_found()
	mg.write("HTTP/1.0 404 Not Found\r\n")
	mg.write("Connection: close\r\n")
	mg.write("Date: " .. os.date("%a, %d %b %Y %H:%M:%S GMT") .. "\r\n")
end


handler = "filehandler.lua"
sub_uri = mg.request_info.uri:sub(#handler+2)
filename = "D:\\civetweb\\civetweb" .. sub_uri
attr = lfs.attributes(filename)

--[[
if not attr then
	send_not_found()
	mg.write("\r\n")
	mg.write("File " .. sub_uri .. " not available")
	return
end
]]

if mg.request_info.request_method == "GET" then
	-- send_file will handle 404 internally
	mg.send_file(filename)
	return

elseif mg.request_info.request_method == "HEAD" then
	-- send_file can handle "GET" and "HEAD"
	mg.send_file(filename)
	return

elseif mg.request_info.request_method == "PUT" then
	local f = io.open(filename, "w")
	if (not f) then
		mg.write("HTTP/1.0 500 Internal Server Error\r\n")
		mg.write("Connection: close\r\n")
		mg.write("Date: " .. os.date("%a, %d %b %Y %H:%M:%S GMT") .. "\r\n")
		mg.write("\r\n")
		return
	end

	mg.write("HTTP/1.0 201 Created\r\n")
	mg.write("Connection: close\r\n")
	mg.write("Date: " .. os.date("%a, %d %b %Y %H:%M:%S GMT") .. "\r\n")
	mg.write("\r\n")
	repeat
		local buf = mg.read();
		if (buf) then
			f:write(buf)
		end
	until (not buf);
	f:close()

	mg.write("HTTP/1.0 201 Created\r\n")
	mg.write("Connection: close\r\n")
	mg.write("Date: " .. os.date("%a, %d %b %Y %H:%M:%S GMT") .. "\r\n")
	mg.write("\r\n")
	return

elseif mg.request_info.request_method == "DELETE" then
	if not attr then
		send_not_found()
		mg.write("\r\n")
		mg.write("File " .. sub_uri .. " not available")
		return
	end
	os.remove(filename)
	mg.write("HTTP/1.0 204 No Content\r\n")
	mg.write("Connection: close\r\n")
	mg.write("Date: " .. os.date("%a, %d %b %Y %H:%M:%S GMT") .. "\r\n")
	mg.write("\r\n")
	return

elseif mg.request_info.request_method == "OPTIONS" then
	send_ok()
	mg.write("Allow: GET, HEAD, PUT, DELETE, OPTIONS\r\n")
	mg.write("\r\n")
	return

else
	mg.write("HTTP/1.0 405 Method Not Allowed\r\n")
	mg.write("Connection: close\r\n")
	mg.write("Date: " .. os.date("%a, %d %b %Y %H:%M:%S GMT") .. "\r\n")
	mg.write("\r\n")
	return
end
