mg.write("HTTP/1.0 200 OK\r\n")
mg.write("Content-Type: text/html\r\n")
mg.write("\r\n")
mg.write([[<html><body>
<p>This is Lua script example 2, served by the
<a href="https://github.com/civetweb/civetweb">CivetWeb web server</a>,
version ]] .. mg.version .. [[.
</p><p>
The following features are available:
<ul>
]])

function print_if_available(tab, name)
  if tab then
    mg.write("<li>" .. name .. " available</li>\n")
  else
    mg.write("<li>" .. name .. " not available</li>\n")
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

-- Get all server options
mg.write("<li>server options</li>\n")
recurse(mg.get_option())

-- Print some data from random generators
if mg.uuid then
  mg.write("<li>random data</li>\n")
  local t = {GUID = mg.uuid(), random = mg.random()}
  recurse(t)
end

-- Print loaded packages known to Lua
if package and (type(package.loaded)=="table") then
  if #package.loaded > 0 then
    mg.write("<li>loaded packages</li>\n")
    recurse(package.loaded)
  else
    mg.write("<li>loaded packages: none</li>\n")
  end
end

-- Print preloaded packages known to Lua
if xml then
  mg.write("<li>xml</li>\n")
  recurse(xml)
end

-- Current date/time
mg.write("</ul></p>\n");
mg.write("<p> Today is " .. os.date("%A") .. "</p>\n");

-- Request content
mg.write("\n<hr/>\n")
l = mg.request_info.content_length
if l then
  mg.write("<p>Content-Length = "..l..":<br>\n<pre>\n")
  mg.write(mg.read())
  mg.write("\n</pre>\n</p>\n")
else
  mg.write("<p>not request content available</p>\n")
end

-- Directory listing
mg.write("\n<hr/>\n")
mg.write("<p>\n");
if not lfs then
  mg.write("lfs not available\n")
else
  mg.write("Files in " .. lfs.currentdir())
  mg.write("\n<ul>\n")
  local cnt = 0
  for f in lfs.dir(".") do
    cnt = cnt + 1
    if (cnt < 6) then
      local mime = mg.get_mime_type(f)
      mg.write("<li>" .. f .. " (" .. mime .. ")</li>\n")
      local at = lfs.attributes(f);
      recurse(at)
    end
  end
  mg.write("</ul>\n")
  mg.write(string.format("<ul>%u files total</ul>\n", cnt))
end

mg.write([[
</p>
</body></html>
]])

