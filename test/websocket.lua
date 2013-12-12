
function iswebsocket()
  return pcall(function()
    if (string.upper(mg.request_info.http_headers.Upgrade)~="WEBSOCKET") then error("") end
  end)
end

if not iswebsocket() then
  mg.write("HTTP/1.0 403 Forbidden\r\n")
  mg.write("Connection: close\r\n")
  mg.write("\r\n")
  return
end


-- Callback for "Websocket ready"
function ready()
  mg.write("text", "Websocket ready")
end

-- Callback for "Websocket received data"
function data(bits, content)
end

-- Callback for "Websocket is closing"
function close()
end


coroutine.yield(true); -- first yield returns (true) or (false) to accept or reject the connection

ready()

repeat
    local cont, bits, content = coroutine.yield(true, 1.0)

    mg.write("text", os.date());

    if bits and content then
        data(bits, content)
    end
until not cont;

mg.write("text", "end")
close()
