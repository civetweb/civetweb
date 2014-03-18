resp = "{";

method = mg.request_info.request_method
uri = mg.request_info.uri
query = mg.request_info.query_string
datalen = nil -- TODO: "CONTENT_LENGTH" !

if method then
  resp = resp .. '"method" : "' .. method .. '", ';
end
if uri then
  resp = resp .. '"uri" : "' .. uri .. '", ';
end
if query then
  resp = resp .. '"query" : "' .. query .. '", ';
end
if datalen then
  resp = resp .. '"datalen" : "' .. datalen .. '", ';
end

resp = resp .. '"time" : "' .. os.date() .. '" ';

resp = resp .. "}";



mg.write("HTTP/1.1 200 OK\r\n")
mg.write("Connection: close\r\n")
mg.write("Content-Type: text/html\r\n")
mg.write("Cache-Control: no-cache\r\n")
--mg.write("Content-Length: " .. resp:len() .. "\n")
mg.write("\r\n")

mg.write(resp)

