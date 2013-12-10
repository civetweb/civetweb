mg.write("HTTP/1.0 200 OK\r\n")
mg.write("Content-Type: text/html\r\n")
mg.write("\r\n")
mg.write([[<html><body>

<p>This is another example of a Lua server page, served by
<a href="http://code.google.com/p/civetweb">Civetweb web server</a>.
</p><p>
The following features are available:
<ul>
]])
  -- function in one Lua tag should still be available in the next one
  function test(tab, name)
    if tab then
      mg.write("<li>" .. name .. "</li>\n")
    end
  end
  function recurse(tab)
    mg.write("<ul>\n")
    for k,v in pairs(tab) do      
      if type(v) == "table" then
        mg.write("<li>" .. tostring(k) .. ":</li>\n")
        recurse(v)
      else
        mg.write("<li>" .. tostring(k) .. " = " .. tostring(v) .. "</li>\n")        
      end
    end
    mg.write("</ul>\n")
  end

  mg.write("<li>" .. _VERSION .. " with the following standard libraries</li>\n")
  mg.write("<ul>\n")
  libs = {"string", "math", "table", "io", "os", "bit32", "package", "coroutine", "debug"};
  for _,n in ipairs(libs) do
    test(_G[n], n);
  end
  mg.write("</ul>\n")
  test(sqlite3, "sqlite3 binding")
  test(lfs,"lua file system")
  
  libname = "mg"
  test(_G[libname], libname .. " library")
  recurse(_G[libname])

  mg.write("</ul></p>\n");
  mg.write("<p> Today is " .. os.date("%A") .. "</p>\n");

 mg.write("<p>\n");
 
 if lfs then    
    mg.write("Files in " .. lfs.currentdir())
    mg.write("\n<ul>\n")
    for f in lfs.dir(".") do
      mg.write("<li>" .. f .. "</li>\n")
      local at = lfs.attributes(f);
      recurse(at)
    end
    mg.write("</ul>\n")
  end

mg.write([[
</p>
</body></html>
]])