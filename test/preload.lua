-- SAPI for CivetWeb
-- Wrapper for the native "mg" interface

SAPI = {}
SAPI.Request = {}
SAPI.Request.getpostdata = function(n)
  local s = ""
  s = mg.read(n)
  return s
end
SAPI.Request.servervariable = function(varname)
  local s
  if varname == "REQUEST_METHOD" then
     s = mg.request_info.request_method
  elseif varname == "QUERY_STRING" then
     s = mg.request_info.query_string
  elseif varname == "SERVER_PORT" then
     s = mg.request_info.server_port
  elseif varname == "SERVER_NAME" then
     s = mg.auth_domain
  elseif varname == "PATH_INFO" then
     s = mg.request_info.path_info
  elseif varname == "AUTH_TYPE" then
     s = mg.request_info.query_string
  elseif varname == "REMOTE_ADDR" then
     s = mg.request_info.remote_addr
  elseif varname == "SERVER_SOFTWARE" then
     s = "CivetWeb/" .. mg.version
  elseif varname == "SERVER_PROTOCOL" then
     s = "HTTP/1.1"
  else
     print(varname)
  end
  return s or ""
end

SAPI._CurrentResponse = {contenttype = "text/plain", header={}, written=false, status=200}

SAPI.Response = {}
SAPI.Response.contenttype = function(header)
  SAPI._CurrentResponse.contenttype = header
  print("Content: " .. header)
end
SAPI.Response.errorlog = function(message)
  print("Error: " .. tostring(message))
end
SAPI.Response.header = function(header, value)
  local t = SAPI._CurrentResponse.header
  t[#t] = string.format("%s: %s", header, value)
end
SAPI.Response.redirect = function(uri)
  print("Redirect: " .. uri)
end
SAPI.Response.write = function(...)
  if not SAPI._CurrentResponse.written then
    if arg[1]:sub(4) ~= "HTTP" then
      mg.write("HTTP/1.1 " .. SAPI._CurrentResponse.status .. " " .. mg.get_response_code_text(SAPI._CurrentResponse.status) .. "\r\n")
      mg.write("Content-Type: " .. SAPI._CurrentResponse.contenttype .. "\r\n")
      for _,v in ipairs(SAPI._CurrentResponse.header) do
        mg.write(v .. "\r\n")
      end
      mg.write("\r\n")
    end
    SAPI._CurrentResponse.written = true;
  end
  for _,v in ipairs(arg) do
    mg.write(v)
  end
end


-- Debug helper function to print a table
function po(t)
  print("Type: " .. type(t) .. ", Value: " .. tostring(t))
  if type(t)=="table" then
    for k,v in pairs(t) do
      print(k,v)
    end
  end
end

-----------------------------------------------------


-- Use cgilua interface
-- require "cgilua"


