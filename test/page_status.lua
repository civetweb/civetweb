mg.write("HTTP/1.0 200 OK\r\n")

-- MIME type: https://www.ietf.org/rfc/rfc4627.txt, chapter 6
mg.write("Content-Type: application/json\r\n")

mg.write("\r\n")

num_threads = mg.get_option("num_threads")
num_threads = tonumber(num_threads)


function n(s) 
  if ((type(s) == "string") and (#s > 0)) then 
    return s 
  else 
    return "null" 
  end
end


mg.write("{\r\n\"system\" :\r\n")

mg.write(n(mg.get_info("system")))

mg.write(",\r\n\"summary\" :\r\n")
mg.write(n(mg.get_info("context")))
mg.write(",\r\n\"common\" :\r\n")
mg.write(n(mg.get_info("common")))
mg.write(",\r\n\"connections\" :\r\n[\r\n")

  mg.write(n(mg.get_info("connection", 1)))

for i=2,num_threads do
  mg.write(",\r\n")
  mg.write(n(mg.get_info("connection", i)))
end
mg.write("]\r\n}\r\n")

