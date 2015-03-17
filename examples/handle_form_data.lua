function add_data(data)
  local additional_data = mg.read()
  if additional_data then
    return data .. additional_data, true
  end
  return data, false
end

function parse_multipart_form(got_field)

    local data, ok = add_data("")
    if not ok then
      return "Can not read data"
    end

    --[[
    local b = mg.request_info.content_type:upper():find("BOUNDARY=");
    if b then
      b = mg.request_info.content_type:sub(b+9)
    end
    if not b or #b<16 or #b>1024 then
      return false, "Boundary string not reasonable"
    end
    ]]
    local b = "--" .. mg.request_info.content_type:upper():match("BOUNDARY=(.*)");
    
    --b = "--" .. b
    if data:sub(1, #b) ~= b then
      return false, "Multipart body does not start properly"
    end
    data = data:sub(#b)
    b = "\r\n" .. b

    -- while there are unread parts
    while #data>0 and data~="--\r\n" do

      local name = nil
      local value = nil
      local file_name = nil
      local file_type = nil

      -- isolate the header of the new part
      local part_header_end
      repeat
        part_header_end = data:find("\r\n\r\n", 1, true)
        if not part_header_end then
          data, ok = add_data(data)
          if not ok then
            return false, "protocol violation: header does not end properly"
          end
        end
      until part_header_end

      -- parse the header of the new part
      local header = {}
      for k,v in data:sub(1,part_header_end+2):gmatch("([^%:\r\n]*)%s*%:%s*([^\r\n]*)\r\n") do
        header[k] = v
        local kupper = k:upper()
        if (kupper=="CONTENT-DISPOSITION") then
          name = v:match('name=%"([^%"]*)%"')
          file_name = v:match('filename=%"([^%"]*)%"')
        elseif (kupper=="CONTENT-TYPE") then
          file_type = v
        end
      end

      -- isolate the body of the new part
      local part_body_end
      data = data:sub(part_header_end+4)
      repeat
        part_body_end = data:find(b, 1, true)
        if not part_body_end then
          data, ok = add_data(data)
          if not ok then
            return false, "protocol violation: body does not end properly"
          end
        end
      until part_body_end
      local value = data:sub(1,part_body_end-1)
      data = data:sub(part_body_end+#b)
      data = add_data(data)

      -- send the result to the caller
      got_field(name, value, file_name, file_type)

    end

    return true, ""
  end




mg.write("HTTP/1.0 200 OK\r\n")
mg.write("Connection: close\r\n")
mg.write("Content-Type: text/plain; charset=utf-8\r\n")
mg.write("Cache-Control: max-age=0, must-revalidate\r\n")
mg.write("\r\n")


mg.write("Parse request:\r\n")

function fetch(k, v, fn, ft)
    mg.write(k .. " = " .. v .. "\r\n")
end

ok, errtxt = parse_multipart_form(fetch)

if not ok then
    mg.write("Error: " .. errtxt .. "\r\n")
else
    mg.write("Parsing OK\r\n")
end

