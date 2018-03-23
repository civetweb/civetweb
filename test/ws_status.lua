if mg.lua_type ~= "websocket" then
  mg.write("HTTP/1.0 200 OK\r\n")
  mg.write("Connection: close\r\n")
  mg.write("\r\n")
  mg.write("<!DOCTYPE HTML>\r\n")
  mg.write("<html xmlns=\"http://www.w3.org/1999/xhtml\">\r\n")
  mg.write("<head>\r\n")
  mg.write("<meta charset=\"UTF-8\"></meta>\r\n")
  mg.write("<title>Server stats</title>\r\n")
  mg.write("</head>\r\n")
  mg.write("<body onload=\"load()\">\r\n")
  mg.write([====[
  <script type="text/javascript">

    var connection;
    var data_field;

    function webSockKeepAlive() {
      if (keepAlive) {
        connection.send('Ok');
        setTimeout("webSockKeepAlive()", 10000);
      }
    }

    function load() {
      var wsproto = (location.protocol === 'https:') ? "wss:" : "ws:";
      connection = new WebSocket(wsproto + "//" + window.location.host + window.location.pathname);
      data_field = document.getElementById('data');

      data_field.innerHTML = "wait for data";

      use_keepAlive = true;

      connection.onopen = function () {
        keepAlive = use_keepAlive;
        webSockKeepAlive();
      };

      // Log errors
      connection.onerror = function (error) {
        keepAlive = false;
        alert("WebSocket error");
        connection.close();
      };

      // Log messages from the server
      connection.onmessage = function (e) {
          data_field.innerHTML = e.data;
      };
    }

</script>
]====])

  mg.write("<div id='data'>Wait for page load</div>\r\n")
  mg.write("</body>\r\n")
  mg.write("</html>\r\n")
  return
end


function table.count(tab)
  local count = 0
  for _ in pairs(tab) do
    count = count + 1
  end
  return count
end


-- table of all active connection
allConnections = {}
connCount = table.count(allConnections)


-- function to get a client identification string
function who(tab)
  local ri = allConnections[tab.client].request_info
  return ri.remote_addr .. ":" .. ri.remote_port
end

-- Callback to accept or reject a connection
function open(tab)
  allConnections[tab.client] = tab
  connCount = table.count(allConnections)
  return true -- return true to accept the connection
end

-- Callback for "Websocket ready"
function ready(tab)
  senddata()
  return true -- return true to keep the connection open
end

-- Callback for "Websocket received data"
function data(tab)
    senddata()
    return true -- return true to keep the connection open
end

-- Callback for "Websocket is closing"
function close(tab)
    allConnections[tab.client] = nil
    connCount = table.count(allConnections)
end

function senddata()
    local date = os.date('*t');

    collectgarbage("collect"); -- Avoid adding uncollected Lua memory from this state

    mg.write(string.format([[
{"Time": "%u:%02u:%02u",
 "Date": "%04u-%02u-%02u",
 "Context": %s,
 "Common": %s,
 "System": \"%s\",
 "ws_status": {"Memory": %u, "Connections": %u}
}]],
date.hour, date.min, date.sec,
date.year, date.month, date.day,
mg.get_info("context"), 
mg.get_info("common"), 
mg.get_info("system"),
collectgarbage("count")*1024,
connCount
));

end

function timer()
    senddata()
    mg.set_timeout("timer()", 1)    
end

mg.set_timeout("timer()", 1)

