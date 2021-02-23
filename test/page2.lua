mg.write("HTTP/1.0 200 OK\r\n")
mg.write("Content-Type: text/html\r\n")
mg.write("\r\n")
mg.write([[<html>
<head>
<title>CivetWeb Lua test page2</title>
<style>
table, th, td {
  border: 1px solid black;
}
</style>
</head>
<body>
<p>This is Lua script example 2, served by the
<a href="https://github.com/civetweb/civetweb">CivetWeb web server</a>,
version ]] .. mg.version .. [[.
</p><p>
The following features are available:
<ul>
]])

demo_data_for_xml = {}
demo_data_for_json = {}

function print_if_available(tab, name)
  if tab then
    mg.write("<li>" .. name .. " available</li>\n")
	if type(tab)=="table" then
	  demo_data_for_xml[name] = {}
	  demo_data_for_xml[name][0] = name
	  demo_data_for_json[name] = {}
	  for nname,nval in pairs(tab) do
	    demo_data_for_xml[name][nname] = type(nval)
		demo_data_for_json[name][nname] = type(nval)
	  end
	else
  	  demo_data_for_xml[name] = type(tab)
	  demo_data_for_json[name] = type(tab)
	end
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
print_if_available(sqlite3, "SQLite3 binding (sqlite3)")
print_if_available(lfs, "LuaFileSystem (lfs)")
print_if_available(json, "JSON binding (json)")
print_if_available(xml, "LuaXML (xml)")
print_if_available(shared, "Lua shared data (shared)")


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

-- Load path
mg.write("<li>Package search path: " .. package.path .. "</li>\n");

-- End of list
mg.write("</ul></p>\n");


-- Current date/time
mg.write("\n<hr/>\n")
mg.write("<p>Today is " .. os.date("%A") .. "</p>\n");

-- Request content
mg.write("\n<hr/>\n")
l = mg.request_info.content_length
if l then
  mg.write("<p>Content-Length = "..l..":<br>\n<pre>\n")
  mg.write(mg.read())
  mg.write("\n</pre>\n</p>\n")
else
  mg.write("<p>no request content available for " .. mg.request_info.request_method .. " method</p>\n")
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
mg.write("</p>\n")


function htmlEsc(txt)
  s = txt:gsub("%&", "&amp;")
  s = s:gsub("%<", "&lt;")
  s = s:gsub("%>", "&gt;")
  return (s)
end


function printTable(tab, indent)
  indent = indent or 0
  for k,v in pairs(tab) do
    if (type(v)=="table") then
      mg.write(string.rep("  ", indent) .. tostring(k) .. ":\n")
	  printTable(v, indent + 1)
	else
      mg.write(string.rep("  ", indent) .. tostring(k) .. "\t" .. tostring(v) .. "\n")
	end
  end
end


-- xml test
if (xml) then
mg.write("\n<hr/>\n")
mg.write("<p>xml2lua:<br>\n<pre>\n");
xmlstr = [[<obj attr="a"><sub1 attr="suba">sub1val</sub1><sub2 attr="suba2" /><sub3></sub3><sub4><subsub>subsubval</subsub></sub4></obj>]]
xmlev = xml.eval(xmlstr)
mg.write(htmlEsc(xmlstr))
mg.write("\n-->\n")
mg.write(type(xmlev) .. ":\n")
mg.write(printTable(xmlev, 1))
mg.write("</pre>\n</p>\n")

mg.write("<p>lua2xml:<br>\n<pre>\n");
mg.write(htmlEsc(xml.str(xmlev, 1, "xml")))
mg.write("</pre>\n</p>\n")

mg.write("<p>lua2xml:<br>\n<pre>\n");
mg.write(htmlEsc(xml.str(demo_data_for_xml, 1, "xml")))
mg.write("</pre>\n</p>\n")
end


-- json test
current_script_path = mg.script_name:match("(.*[%/%\\]).*%.lua")
script_search_path = current_script_path .. "?.lua"
package.path = script_search_path .. ";" .. package.path
json = require "json"
mg.write("\n<hr/>\n")
mg.write("<p>json2lua:<br>\n<pre>\n");
-- JSON example from https://en.wikipedia.org/wiki/JSON#Example
jsonstr = [[{
  "firstName": "John",
  "lastName": "Smith",
  "isAlive": true,
  "age": 27,
  "address": {
    "streetAddress": "21 2nd Street",
    "city": "New York",
    "state": "NY",
    "postalCode": "10021-3100"
  },
  "phoneNumbers": [
    {
      "type": "home",
      "number": "212 555-1234"
    },
    {
      "type": "office",
      "number": "646 555-4567"
    },
    {
      "type": "mobile",
      "number": "123 456-7890"
    }
  ],
  "children": [],
  "spouse": null
}]]
jsonev = json.decode(jsonstr)
mg.write(htmlEsc(jsonstr))
mg.write("\n-->\n")
mg.write(type(jsonev) .. ":\n")
mg.write(printTable(jsonev, 1))
mg.write("</pre>\n</p>\n")

mg.write("<p>lua2json:<br>\n<pre>\n");
mg.write(htmlEsc(json.encode(jsonev)))
mg.write("</pre>\n</p>\n")

mg.write("<p>lua2json:<br>\n<pre>\n");
mg.write(htmlEsc(json.encode(demo_data_for_json)))
mg.write("</pre>\n</p>\n")


if mg.request_info.query_string then
  qtab = mg.split_form_data(mg.request_info.query_string)
  mg.write("<p>\nSplit form test:<br/>\n");
  mg.write("<table>\n");
  mg.write("<tr>\n");
  mg.write("<th>#</th>\n");
  mg.write("<th>name</th>\n");
  mg.write("<th>value</th>\n");
  mg.write("</tr>\n");
  for n,t in pairs(qtab) do
    mg.write("<tr>\n");
    mg.write("<td>" .. n .. "</td>\n");
    mg.write("<td>" .. tostring(t.name) .. "</td>\n");
    mg.write("<td>" .. tostring(t.value) .. "</td>\n");
    mg.write("</tr>\n");
  end
  mg.write("</table>\n");
  mg.write("</p>\n");
end


-- Test timer from Lua background script
mg.write("\n<hr/>\n")

mg.write("<p>\nLua shared.timer ");
if (shared and shared.timer) then
  mg.write(tostring(shared.timer))
  mg.write([[ <button onClick="window.location.reload();">Refresh Page</button>]])
else
  mg.write("not available")
end
mg.write("\n</p>\n")

-- Next section ...
mg.write("\n<hr/>\n")

mg.write([[
</body></html>
]])
