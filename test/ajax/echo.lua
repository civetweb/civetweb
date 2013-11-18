resp = "{";

method = mg.request_info.request_method
uri = mg.request_info.uri
query = os.getenv("QUERY_STRING");
datalen = os.getenv("CONTENT_LENGTH");

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



mg.write("HTTP/1.0 200 OK\n")
mg.write("Connection: close\n")
mg.write("Content-Type: text/html\n")
mg.write("Cache-Control: no-cache\n")
--mg.write("Content-Length: " .. resp:len() .. "\n")
mg.write("\n")

mg.write(resp)

