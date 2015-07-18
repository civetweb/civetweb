-- Some basic checks
if mg.request_info.request_method ~= "POST" or mg.request_info.content_type:lower():sub(1,19) ~= 'multipart/form-data' then
  mg.write("HTTP/1.0 400 OK\r\n")
  mg.write("Connection: close\r\n")
  mg.write("Content-Type: text/plain; charset=utf-8\r\n")
  mg.write("Cache-Control: max-age=0, must-revalidate\r\n")
  mg.write("\r\n")
  mg.write("Bad request\r\n\r\n")
  return
end

-- HTTP headers
mg.write("HTTP/1.0 200 OK\r\n")
mg.write("Connection: close\r\n")
mg.write("Content-Type: text/plain; charset=utf-8\r\n")
mg.write("Cache-Control: max-age=0, must-revalidate\r\n")
mg.write("\r\n")

-- Which form sent the data?
mg.write("Read POST data from " .. mg.request_info.http_headers.Referer .. ":\r\n\r\n")

-- Count some data fields
local fields = 0
local datasize = 0

-- Read the entire body data (POST content) into "bdata" variable.
-- Use a string builder pattern for performance reasons
stringtab = {}
bdata = ""
repeat
  local add_data = mg.read()
  if add_data then
    stringtab[#stringtab+1] = add_data
  end
until (add_data == nil);
bdata = table.concat(stringtab)
stringtab = nil

-- Get the boundary string.
bs = "--" .. ((mg.request_info.content_type):upper():match("BOUNDARY=(.*)"));

-- The POST data has to start with the boundary string.
-- Check this and remove the starting boundary.
if bdata:sub(1, #bs) ~= bs then
  error "invalid format of POST data"
end
bdata = bdata:sub(#bs)

-- The boundary now starts with CR LF.
bs = "\r\n" .. bs

-- Now loop through all the parts
while #bdata>4 do
   -- Find the header of new part.
   part_header_end = bdata:find("\r\n\r\n", 1, true)

   -- Parse the header.
   local form_field_name, file_name
   h = bdata:sub(1, part_header_end+2)
   for key,val in h:gmatch("([^%:\r\n]*)%s*%:%s*([^\r\n]*)\r\n") do
      if key:upper() == "CONTENT-DISPOSITION" then
          form_field_name = val:match('name=%"([^%"]*)%"')
          file_name = val:match('filename=%"([^%"]*)%"')
      end
   end

   -- Remove the header from "bdata".
   bdata = bdata:sub(part_header_end+4)

   -- Find the end of the body by locating the boundary string.
   local part_body_end = bdata:find(bs, 1, true)

   -- Isolate the content, and drop it from "bdata".
   local form_field_value = bdata:sub(1,part_body_end-1)
   bdata = bdata:sub(part_body_end+#bs)

   -- Now the data (file content or field value) is isolated: We know form_field_name and form_field_value.
   -- Here the script should do something useful with the data. This example just sends it back to the client.
   if form_field_name then
     mg.write("Field name: " .. form_field_name .. "\r\n")
   end

   local len = #form_field_value
   mg.write("Field data length: " .. len .. "\r\n")

   if file_name then
     mg.write("File name: " .. file_name .. "\r\n")
     mg.write("File content:\r\n")
     local maxlen
     if len>320 then maxlen=320 else maxlen=len end

     for l=0,maxlen,16 do
       for m=1,16 do
         local b = form_field_value:byte(l+m)
         if (b) then
           mg.write(string.format("%02x ", b))
         else
           mg.write("   ")
         end
       end
       mg.write(" -  " .. form_field_value:sub(l+1,l+16):gsub("[%c%z%s]", " ") .. "\r\n")
     end
     if maxlen<len then
       mg.write(string.format("... (+ %u bytes)\r\n", len-maxlen))
     end

   else
     -- not a file
     if len<50 then
       mg.write("Field value: " .. form_field_value .. "\r\n")
     else
       mg.write("Field value: " .. form_field_value:sub(1, 40) .. " .. (" .. len .. " bytes)\r\n")
     end
   end


   mg.write("\r\n")
   fields = fields + 1
   datasize = datasize + len

end

mg.write("Got " .. fields .. " input fields with " .. datasize .. " bytes total\r\n");
