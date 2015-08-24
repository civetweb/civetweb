<? mg.write("HTTP/1.0 200 OK") ?>
<? mg.write("Content-Type: text/html") ?>

<html><body>

<p>This is another example of a Lua server page, served by
<a href="https://github.com/civetweb/civetweb">CivetWeb web server</a>.
</p><p>
The following features are available:
<ul>
<?
  -- functions defubed in one Lua tag should still be available in the next one
  function print_if_available(tab, name)
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
?>
<?
  -- Print Lua version and available libraries
  mg.write("<li>" .. _VERSION .. " with the following standard libraries</li>\n")
  mg.write("<ul>")
  libs = {"string", "math", "table", "io", "os", "bit32", "utf8", "package", "coroutine", "debug"};
  for _,n in ipairs(libs) do
    print_if_available(_G[n], n);
  end
  mg.write("</ul>\n")
  print_if_available(sqlite3, "sqlite3 binding")
  print_if_available(lfs,"lua file system")

  -- Print mg library
  libname = "mg"
  print_if_available(_G[libname], libname .. " library")
  recurse(_G[libname])

  -- Print connect function
  print_if_available(connect, "connect function")

?>
</ul></p>
<p> Today is <? mg.write(os.date("%A")) ?>

<p>
<?
  -- for k,v in pairs(_G) do mg.write(k, '\n') end

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
?>
</p>
</body></html>
