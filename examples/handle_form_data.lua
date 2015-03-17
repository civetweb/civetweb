mg.write("HTTP/1.0 200 OK\r\n")
mg.write("Connection: close\r\n")
mg.write("Content-Type: text/plain; charset=utf-8\r\n")
mg.write("Cache-Control: max-age=0, must-revalidate\r\n")
mg.write("\r\n")

-- Read the entire body data (POST content) into "bdata" variable.
bdata = ""
repeat
  local add_data = mg.read()
  if add_data then
    bdata = bdata .. add_data
  end
until (add_data == nil);

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
   part_body_end = bdata:find(bs, 1, true)

   -- Isolate the content, and drop it from "bdata".
   form_field_value = bdata:sub(1,part_body_end-1)
   bdata = bdata:sub(part_body_end+#bs)

   -- Now the data (file content or field value) is isolated: We know form_field_name and form_field_value.
   -- Here the script should do something useful with the data. This example just sends it back to the client.
   mg.write("Field name: " .. form_field_name .. "\r\n")
   local len = #form_field_value
   if len<50 then
     mg.write("Field value: " .. form_field_value .. "\r\n")
   else
     mg.write("Field value: " .. form_field_value:sub(1, 40) .. " .. (" .. len .. " bytes)\r\n")
   end
   mg.write("\r\n")

end
