HTTP/1.0 200 OK
Content-Type: text/html

<html><body>


<p>This is another example of a Lua server page, served by
<a href="http://sourceforge.net/projects/civetweb/">Civetweb web server</a>.
</p><p>
The following features are available:
<ul>
<?
  mg.write("<li>" .. _VERSION .. " server pages</li>")
  if sqlite3 then
    mg.write("<li>sqlite3 binding</li>")
  end
  if lfs then
    mg.write("<li>lua file system</li>")
  end
?>
</ul></p>
<p> Today is <? mg.write(os.date("%A")) ?></p>
<p> URI is <? mg.write(mg.request_info.uri) ?></p>
<p> URI is <?=mg.request_info.uri?></p>

<p>Database example:
<pre>
<?
  -- Open database
  local db = sqlite3.open('requests.db')
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
      addr
    );
  ]])

  -- Add entry about this request
  local stmt = db:prepare(
    'INSERT INTO requests VALUES(NULL, datetime("now"), ?, ?, ?);');
  stmt:bind_values(mg.request_info.request_method,
                   mg.request_info.uri,
                   mg.request_info.remote_port)
  stmt:step()
  stmt:finalize()

  -- Show all previous records
  mg.write('Previous requests:\n')
  stmt = db:prepare('SELECT * FROM requests ORDER BY id DESC;')
  while stmt:step() == sqlite3.ROW do
    local v = stmt:get_values()
    mg.write(v[1] .. ' ' .. v[2] .. ' ' .. v[3] .. ' '
          .. v[4] .. ' ' .. v[5] .. '\n')
  end

  -- Close database
  db:close()
?>
</pre></p>

</body></html>
