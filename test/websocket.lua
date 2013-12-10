-- Open database
local db = sqlite3.open('r:\\websockLog.db')

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
end


-- Callback for "Websocket ready"
function ready()
  logDB("WEBSOCKET READY")
  mg.write("text", "Websocket ready")
end

-- Callback for "Websocket received data"
function data(bits, content)
    logDB(string.format("WEBSOCKET DATA (%x)", bits))
    mg.write("text", os.date())
    return true;
end

-- Callback for "Websocket is closing"
function close()
  logDB("WEBSOCKET CLOSE")
  -- Close database
  db:close()
end


-- Websocket with coroutines
logDB("WEBSOCKET PREPARE")

coroutine.yield(true); -- first yield returns (true) or (false) to accept or reject the connection
ready()
repeat
    local cont, bits, content = coroutine.yield(true, 1.0)
    if bits and content then
        data(bits, content)
    end
until not cont;

mg.write("text", "end")
close()
