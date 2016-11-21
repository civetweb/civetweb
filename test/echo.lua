
if mg.lua_type ~= "websocket" then
  mg.write("HTTP/1.0 403 Forbidden\r\n")
  mg.write("Connection: close\r\n")
  mg.write("\r\n")
  mg.write("forbidden")
  return
end


-- table of all active connection
allConnections = {}

-- function to get a client identification string
function who(tab)
  local ri = allConnections[tab.client].request_info
  return ri.remote_addr .. ":" .. ri.remote_port
end

-- Callback to accept or reject a connection
function open(tab)
  allConnections[tab.client] = tab
  return true -- return true to accept the connection
end

-- Callback for "Websocket ready"
function ready(tab)
  return true -- return true to keep the connection open
end

-- Callback for "Websocket received data"
function data(tab)
    mg.write(1, tab.data);
    return true -- return true to keep the connection open
end

-- Callback for "Websocket is closing"
function close(tab)
    allConnections[tab.client] = nil
end

