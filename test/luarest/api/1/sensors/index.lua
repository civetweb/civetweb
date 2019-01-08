require "json"
-- json library needs to be available 


local http_status = "200 OK";
local response_table = {};


if mg.request_info.request_method == "GET" then

  response_table["result"] = "ok";
  response_table["data"] = {};
  for i=1,10 do
    response_table.data[i] = {name = "sensor" .. i, value = 20+i};
  end
  
elseif mg.request_info.request_method == "PUT" then

  local request_len = mg.request_info.content_length
  local read_len = 0
  local request_body_sep = {}
  local error_reading = false
  
  repeat 
    local chunk = mg.read()
	if chunk then
	   read_len = read_len + #chunk
	   request_body_sep[#request_body_sep+1] = chunk
	else
	   error_reading = true;	   
	end
  until ((read_len == request_len) or error_reading);
  
  
  if (error_reading) then
    response_table["error"] = "ok";
    response_table["status"] = "error reading request body data";
  else
    local request_body = table.concat(request_body_sep)
    local isok, request_table = pcall(json.parse, request_body)
  
    -- TODO: check "isok", then check all arguments in "request_table", 
    
    response_table["result"] = "ok";
    response_table["data"] = {name = request_table};
  end
  
else

  http_status = "403 Forbidden";
  response_table["result"] = "error";
  response_table["status"] = "invalid method " .. mg.request_info.request_method;
  
end


local response_body = json.stringify(response_table)
mg.write("HTTP/1.1 " .. http_status .. "\r\n")
mg.write("Content-Type: application/json; charset=UTF-8\r\n")
mg.write("Content-Length: ".. #response_body .."\r\n")
mg.write("\r\n")
mg.write(response_body)
