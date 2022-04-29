-- reflect the request information as JSON
json = require "json"

response = json.encode(mg.request_info)

mg.response.status = 200
mg.response.http_headers["Content-Type"] = "application/json; charset=utf-8";
mg.response.http_headers["Content-Length"] = #response;
mg.response.send()
mg.write(response)
