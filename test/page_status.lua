mg.write("HTTP/1.0 200 OK\r\n")
mg.write("Content-Type: text/html\r\n")
mg.write("\r\n")
mg.write("<html><body><p>\r\n")
mg.write(mg.get_info("system"))
mg.write("</p>\r\n<p>\r\n")
mg.write(mg.get_info("context"))
mg.write("</p>\r\n<p>\r\n")
mg.write(mg.get_info("common"))
for i=1,100 do
  mg.write("</p>\r\n<p>\r\n")
  mg.write(mg.get_info("connection", i))
end
mg.write("</p></body></html>\r\n")
