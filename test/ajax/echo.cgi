#!/usr/bin/lua5.1

-- Every CGI script that returns any valid JSON object will work in the test.
-- In case you do not have not yet used CGI, you may want to use this script which is written in Lua.
-- You may download an interpreter from http://luabinaries.sourceforge.net/download.html, extract it
-- to some folder in your search path (the path of the webserver or /usr/bin on Linux), and add the
-- following lines to your .conf file.
-- cgi_interpreter c:\somewhere\lua5.1.exe
-- enable_keep_alive yes

resp = "{";

method = os.getenv("REQUEST_METHOD")
uri = os.getenv("REQUEST_URI");
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




print "Status: 200 OK"
print "Connection: close"
--print "Connection: keep-alive"
print "Content-Type: text/html; charset=utf-8"
print "Cache-Control: no-cache"
--print ("Content-Length: " .. resp:len())
print ""

print (resp)


doLogging = false

if (doLogging) then
  -- Store the POST data to a file
  if (method == "POST") then
    myFile = io.open("data" .. query:sub(4) .. ".txt", "wb");
    myFile:write(resp)
    myFile:write("\r\n\r\n")  
    if datalen then
      datalen = tonumber(datalen)
      myFile:write("<<< " .. datalen .. " bytes of data >>>\r\n")
      
      data = io.stdin:read(datalen)
      myFile:write(data)
      
      myFile:write("\r\n<<< end >>>\r\n")
    else
      myFile:write("<<< no data >>>\r\n")
    end  
    myFile:close()
  end
end



