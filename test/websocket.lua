function trace(text)
    local f = io.open("R:\\websocket.trace", "a")
    f:write(os.date() .. " - " .. text .. "\n")
    f:close()
end

function iswebsocket()
  return pcall(function()
    if (string.upper(mg.request_info.http_headers.Upgrade)~="WEBSOCKET") then error("") end
  end)
end

if not iswebsocket() then
  trace("no websocket")
  mg.write("HTTP/1.0 403 Forbidden\r\n")
  mg.write("Connection: close\r\n")
  mg.write("\r\n")
  mg.write("forbidden")
  return
end


-- Callback to reject a connection
function open()
  trace("open")
  return true
end

-- Callback for "Websocket ready"
function ready()
  trace("ready")
  mg.write("text", "Websocket ready")
  senddata()
  return true
end

-- Callback for "Websocket received data"
function data(bits, content)
    trace("data(" .. bits .. "): " .. content)
    senddata()
    return true
end

-- Callback for "Websocket is closing"
function close()
    trace("close")
    mg.write("text", "end")
end

function senddata()
    trace("senddata")
    local date = os.date('*t');
    local hand = (date.hour%12)*60+date.min;

    mg.write("text", string.format("%u:%02u:%02u", date.hour, date.min, date.sec));

    if (hand ~= lasthand) then
        mg.write("text", string.format("-->h %u", hand*360/(12*60)));
        mg.write("text", string.format("-->m %u", date.min*360/60));
        lasthand = hand;
    end

    if bits and content then
        data(bits, content)
    end
end

trace("defined")
