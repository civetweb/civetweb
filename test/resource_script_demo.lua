-- This is a Lua script that handles sub-resources, e.g. resource_script_demo.lua/path/file.ext

scriptUri = "resource_script_demo.lua"
envVar = "resource_script_demo_storage"

resourcedir = os.getenv(envVar) or "R:\\RESOURCEDIR"
method = mg.request_info.request_method:upper()

if resourcedir then
  attr = lfs.attributes(resourcedir)
end

if (not mg.request_info.uri:find(scriptUri)) or (not resourcedir) or (not attr) or (attr.mode~="directory") then
    mg.write("HTTP/1.0 500 OK\r\n")
    mg.write("Connection: close\r\n")
    mg.write("Content-Type: text/html; charset=utf-8\r\n")
    mg.write("\r\n")
    mg.write("<html><head><title>Civetweb Lua script resource handling test</title></head>\r\n")
    mg.write("<body>\r\nServer error.<br>\r\n")
    mg.write("The server admin must make sure this script is available as URI " .. scriptUri .. "<br>\r\n")
    mg.write("The server admin must set the environment variable " .. envVar .. " to a directory.<br>\r\n")
    mg.write("</body>\r\n</html>\r\n")
    return
end
subresource = mg.request_info.uri:match(scriptUri .. "/(.*)")

if not subresource then
    if method=="GET" then
        mg.write("HTTP/1.0 200 OK\r\n")
        mg.write("Connection: close\r\n")
        mg.write("Content-Type: text/html; charset=utf-8\r\n")
        mg.write("\r\n")
        mg.write("<html><head><title>Civetweb Lua script resource handling test</title></head>\r\n")
        mg.write("<body>No resource specified.<br>resourcedir is " .. resourcedir .. "</body></html>\r\n")
    else
        mg.write("HTTP/1.0 405 Method Not Allowed\r\n")
        mg.write("Connection: close\r\n")
        mg.write("Content-Type: text/html; charset=utf-8\r\n")
        mg.write("\r\n")
        mg.write("<html><head><title>Civetweb Lua script resource handling test</title></head>\r\n")
        mg.write("<body>Method not allowed.</body></html>\r\n")
    end
    return
end


if method=="GET" then
    file = resourcedir .. "/" .. subresource
    if lfs.attributes(file) then
        mg.send_file(file)
    else
        mime = mg.get_mime_type(file)
        mg.write("HTTP/1.0 404 Not Found\r\n")
        mg.write("Connection: close\r\n")
        mg.write("Content-Type: text/html; charset=utf-8\r\n")
        mg.write("\r\n")
        mg.write("<html><head><title>Civetweb Lua script resource handling test</title></head>\r\n")
        mg.write("<body>Resource of type \"" .. mime .. "\" not found.</body></html>\r\n")
    end
    return
end

if method=="PUT" then
    file = resourcedir .. "/" .. subresource
    mime = mg.get_mime_type(file)
    if lfs.attributes(file) then
        mg.write("HTTP/1.0 405 Method Not Allowed\r\n")
        mg.write("Connection: close\r\n")
        mg.write("Content-Type: text/html; charset=utf-8\r\n")
        mg.write("\r\n")
        mg.write("<html><head><title>Civetweb Lua script resource handling test</title></head>\r\n")
        mg.write("<body>Resource of type \"" .. mime .. "\" already exists.</body></html>\r\n")
    else
        local f = io.open(file, "w")

        local data = {}
        repeat
            local l = mg.read();
            data[#data+1] = l;
        until ((l == "") or (l == nil));

        f:write(table.concat(data, ""))
        f:close()
        mg.write("HTTP/1.0 200 OK\r\n")
        mg.write("Connection: close\r\n")
        mg.write("Content-Type: text/html; charset=utf-8\r\n")
        mg.write("\r\n")
        mg.write("<html><head><title>Civetweb Lua script resource handling test</title></head>\r\n")
        mg.write("<body>Resource of type \"" .. mime .. "\" created.</body></html>\r\n")
    end
    return
end

if method=="DELETE" then
    file = resourcedir .. "/" .. subresource
    mime = mg.get_mime_type(file)
    if lfs.attributes(file) then
        os.remove(file)
        mg.write("HTTP/1.0 200 OK\r\n")
        mg.write("Connection: close\r\n")
        mg.write("Content-Type: text/html; charset=utf-8\r\n")
        mg.write("\r\n")
        mg.write("<html><head><title>Civetweb Lua script resource handling test</title></head>\r\n")
        mg.write("<body>Resource of type \"" .. mime .. "\" deleted.</body></html>\r\n")
    else
        mime = mg.get_mime_type(file)
        mg.write("HTTP/1.0 404 Not Found\r\n")
        mg.write("Connection: close\r\n")
        mg.write("Content-Type: text/html; charset=utf-8\r\n")
        mg.write("\r\n")
        mg.write("<html><head><title>Civetweb Lua script resource handling test</title></head>\r\n")
        mg.write("<body>Resource of type \"" .. mime .. "\" not found.</body></html>\r\n")
    end
    return
end

-- Any other method
mg.write("HTTP/1.0 405 Method Not Allowed\r\n")
mg.write("Connection: close\r\n")
mg.write("Content-Type: text/html; charset=utf-8\r\n")
mg.write("\r\n")
mg.write("<html><head><title>Civetweb Lua script resource handling test</title></head>\r\n")
mg.write("<body>Method not allowed.</body></html>\r\n")

