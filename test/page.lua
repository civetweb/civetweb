mg.write("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n")

mg.write([[<html>
<head>
<title>Lua SQLite database test</title>
<style>
table, th, td {
    border: 1px solid black;
    border-collapse: collapse;
    border-spacing: 5px;
}
th, td {
    padding: 5px;
    text-align: left;    
}
</style>
</head>
<body>
<p>This is Lua script example 1, served by the
<a href="https://github.com/civetweb/civetweb">CivetWeb web server</a>,
version ]] .. mg.version .. [[.
</p><p>
The following features are available:
<ul>
]])

  mg.write('<li><a href="http://www.lua.org/docs.html">' .. _VERSION .. "</a> server pages</li>")
  if sqlite3 then
    mg.write('<li><a href="http://lua.sqlite.org/index.cgi/doc/tip/doc/lsqlite3.wiki">sqlite3</a> binding</li>')
  end
  if lfs then
    mg.write('<li><a href="https://keplerproject.github.io/luafilesystem/manual.html">lua file system</a></li>')
  end

  
mg.write("</ul></p>\r\n")
mg.write("<p> Today is " .. os.date("%A") .. "</p>\r\n")
mg.write("<p> URI is " .. mg.request_info.uri .. "</p>\r\n")

mg.write("<p>\r\n<pre>\r\n")

-- Open database
local db, errcode, errmsg = sqlite3.open('requests.db')

if db then

  -- Note that the data base is located in the current working directory
  -- of the process if no other path is given here.

  -- Setup a trace callback, to show SQL statements we'll be executing.
  -- db:trace(function(data, sql) mg.write('Executing: ', sql: '\n') end, nil)

  -- Create a table if it is not created already
  db:exec([[
    CREATE TABLE IF NOT EXISTS requests (
      id INTEGER PRIMARY KEY AUTOINCREMENT,
      timestamp NOT NULL,
      method NOT NULL,
      uri NOT NULL,
      addr,
      civetwebversion,
      luaversion,
      aux
    );
  ]])

  -- Add columns to table created with older version
  db:exec("ALTER TABLE requests ADD COLUMN civetwebversion;")
  db:exec("ALTER TABLE requests ADD COLUMN luaversion;")
  db:exec("ALTER TABLE requests ADD COLUMN aux;")

  -- Add entry about this request
  local stmt = db:prepare(
    'INSERT INTO requests VALUES(NULL, datetime("now"), ?, ?, ?, ?, ?, ?);');
  stmt:bind_values(mg.request_info.request_method,
                   mg.request_info.uri,
                   mg.request_info.remote_port,
                   mg.version,
                   _VERSION,
                   ""
                   )
  stmt:step()
  stmt:finalize()

  -- Show all previous records
  mg.write('<table>\n')
  mg.write("<tr>\n")
  mg.write("<th>id</th>\n")
  mg.write("<th>timestamp</th>\n")
  mg.write("<th>method</th>\n")
  mg.write("<th>uri</th>\n")
  mg.write("<th>addr</th>\n")
  mg.write("<th>civetweb</th>\n")
  mg.write("<th>lua</th>\n")
  mg.write("<th>aux</th>\n")
  mg.write("</tr>\n")

  stmt = db:prepare('SELECT * FROM requests ORDER BY id DESC;')
  while stmt:step() == sqlite3.ROW do
    local v = stmt:get_values()
    mg.write("<tr>\n")
    local i = 1
    while (v[i]) do
      mg.write("<td>" .. v[i] .. "</td>\n")
      i = i+1
    end
    mg.write("</tr>\n")
  end

  mg.write("</table>\n")

  -- Close database
  db:close()

else

  mg.write("DB error:\n")
  mg.write("code = " .. tostring(errcode) .. "\n")
  mg.write("msg = " .. tostring(msg) .. "\n")

end

mg.write([[
</pre>
</p>
</body>
</html>
]])

