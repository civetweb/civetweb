-- Open database
local db = sqlite3.open('requests.db')

if db then
  db:busy_timeout(200);
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
end



local function logDB(method)
  -- Add entry about this request
  local r;
  repeat
    r = db:exec([[INSERT INTO requests VALUES(NULL, datetime("now"), "]] .. method .. [[", "]] .. mg.request_info.uri .. [[", "]] .. mg.request_info.remote_port .. [[");]]);
  until r~=5;

  --[[
  -- alternative logging (to a file)
  local f = io.open("R:\\log.txt", "a");
  f:write(os.date() .. " - " .. method .. " - " .. mg.request_info.uri .. " - " .. mg.request_info.remote_port .. " <" .. r .. ">\n")
  f:close()
  --]]
end


-- Callback for "Websocket ready"
function ready()
  logDB("WEBSOCKET READY")
end

-- Callback for "Websocket received data"
function data(bits, content)
    logDB(string.format("WEBSOCKET DATA (%x)", bits))
    return true;
end

-- Callback for "Websocket is closing"
function close()
  logDB("WEBSOCKET CLOSE")
  -- Close database
  db:close()
end


logDB("WEBSOCKET PREPARE")
return true; -- could return false to reject the connection before the websocket handshake
