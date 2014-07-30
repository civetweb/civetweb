timerID = "timeout"
--timerID = "interval"

function trace(text)
    local f = io.open("websocket.trace", "a")
    f:write(os.date() .. " - " .. text .. "\n")
    f:close()
end

function iswebsocket()
  return mg.lua_type == "websocket"
  --return pcall(function()
  --  if (string.upper(mg.request_info.http_headers.Upgrade)~="WEBSOCKET") then error("") end
  --end)
end

trace("called with Lua type " .. tostring(mg.lua_type))

if not iswebsocket() then
  trace("no websocket")
  mg.write("HTTP/1.0 403 Forbidden\r\n")
  mg.write("Connection: close\r\n")
  mg.write("\r\n")
  mg.write("forbidden")
  return
end


-- Serialize table to string
function ser(val)
  local t
  if type(val) == "table" then
    for k,v in pairs(val) do
      if t then
        t = t .. ", " .. ser(k) .. "=" .. ser(v)
      else
        t = "{" .. ser(k) .. "=" .. ser(v)
      end
    end
    t = t .. "}"
  else
    t = tostring(val)
  end
  return t
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
  trace("open[" .. who(tab) .. "]: " .. ser(tab))
  return true -- return true to accept the connection
end

-- Callback for "Websocket ready"
function ready(tab)
  trace("ready[" .. who(tab) .. "]: " .. ser(tab))
  mg.write(tab.client, "text", "Websocket ready")
  mg.write(tab.client, 1, "-->h 180");
  mg.write(tab.client, "-->m 180");
  senddata()
  if timerID == "timeout" then
    mg.set_timeout("timer()", 1)
  elseif timerID == "interval" then
    mg.set_interval("timer()", 1)
  end
  return true -- return true to keep the connection open
end

-- Callback for "Websocket received data"
function data(tab)
    trace("data[" .. who(tab) .. "]: " .. ser(tab))
    senddata()
    return true -- return true to keep the connection open
end

-- Callback for "Websocket is closing"
function close(tab)
    trace("close[" .. who(tab) .. "]: " .. ser(tab))
    mg.write("text", "end")
    allConnections[tab.client] = nil
end

function senddata()
    local date = os.date('*t');
    local hand = (date.hour%12)*60+date.min;

    mg.write("text", string.format("%u:%02u:%02u", date.hour, date.min, date.sec));

    if (hand ~= lasthand) then
        mg.write(1, string.format("-->h %u", hand*360/(12*60)));
        mg.write(   string.format("-->m %u", date.min*360/60));
        lasthand = hand;
    end

    if bits and content then
        data(bits, content)
    end
end

function timer()
    trace("timer")
    senddata()
    if timerID == "timeout" then
        mg.set_timeout("timer()", 1)
    else
        return true -- return true to keep an interval timer running
    end
end

