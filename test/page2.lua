mg.write("HTTP/1.0 200 OK\r\n")
mg.write("Content-Type: text/html\r\n")
mg.write("\r\n")
mg.write([[<html><body>

<p>This is another example of a Lua server page, served by
<a href="https://github.com/civetweb/civetweb">CivetWeb web server</a>.
</p><p>
The following features are available:
<ul>
]])

function print_if_available(tab, name)
  if tab then
    mg.write("<li>" .. name .. "</li>\n")
  end
end

function recurse(tab, excl)
  excl = excl or {}
  mg.write("<ul>\n")
  for k,v in pairs(tab) do
    if type(v) == "table" then
      mg.write("<li>" .. tostring(k) .. ":</li>\n")
      if excl[v] then
        -- cyclic
      else
        excl[v] = true
        recurse(v, excl)
        excl[v] = false
      end
    else
      mg.write("<li>" .. tostring(k) .. " = " .. tostring(v) .. "</li>\n")
    end
  end
  mg.write("</ul>\n")
end

-- Print Lua version and available libraries
mg.write("<li>" .. _VERSION .. " with the following standard libraries</li>\n")
mg.write("<ul>\n")
libs = {"string", "math", "table", "io", "os", "bit32", "utf8", "package", "coroutine", "debug"};
for _,n in ipairs(libs) do
  print_if_available(_G[n], n);
end
mg.write("</ul>\n")
print_if_available(sqlite3, "sqlite3 binding")
print_if_available(lfs, "lua file system")

--recurse(_G)

-- Print mg library
libname = "mg"
print_if_available(_G[libname], libname .. " library")
recurse(_G[libname])

-- Print connect function
print_if_available(connect, "connect function")

mg.write("</ul></p>\n");
mg.write("<p> Today is " .. os.date("%A") .. "</p>\n");

l = mg.request_info.content_length
if l then
  mg.write("<p>Content-Length = "..l..":<br>\n<pre>\n")
  mg.write(mg.read())
  mg.write("\n</pre>\n</p>\n")
end

mg.write("<p>\n");

 if lfs then
  mg.write("Files in " .. lfs.currentdir())
  mg.write("\n<ul>\n")
  for f in lfs.dir(".") do
    local mime = mg.get_mime_type(f)
    mg.write("<li>" .. f .. " (" .. mime .. ")</li>\n")
    local at = lfs.attributes(f);
    recurse(at)
  end
  mg.write("</ul>\n")
end

mg.write([[
</p>
</body></html>
]])
