mg.write("HTTP/1.1 200 OK\r\n")
mg.write("Connection: close\r\n")
mg.write("Content-Type: text/html; charset=utf-8\r\n")
mg.write("\r\n")

t = os.time()

if not mg.request_info.query_string then
  cnt = 1000
else
  cnt = tonumber(mg.get_var(mg.request_info.query_string, "cnt"))
end

cnt = 100*math.floor(cnt/100)

mg.write([[
<html>
  <head>
    <title>]] .. cnt .. [[ images</title>
    <script type="text/javascript">
      var startLoad = Date.now();
      window.onload = function () {
        var loadTime = (Date.now()-startLoad) + " ms";
        document.getElementById('timing').innerHTML = loadTime;
      }
    </script>
  </head>
  <body>
    <h1>A large gallery of small images:</h1>
    <p>
]])
for s=0,(cnt/100)-1 do
local ts = (tostring(t) .. tostring(s))
mg.write([[
      <h2>page ]]..s..[[</h2>
      <table>
        <tr>
          <td><img src="imagetest/00.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/01.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/02.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/03.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/04.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/05.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/06.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/07.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/08.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/09.png?ts=]]..ts..[["></td>
        </tr>
]])
mg.write([[
        <tr>
          <td><img src="imagetest/10.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/11.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/12.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/13.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/14.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/15.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/16.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/17.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/18.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/19.png?ts=]]..ts..[["></td>
        </tr>
]])
mg.write([[
        <tr>
          <td><img src="imagetest/20.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/21.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/22.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/23.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/24.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/25.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/26.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/27.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/28.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/29.png?ts=]]..ts..[["></td>
        </tr>
]])
mg.write([[
        <tr>
          <td><img src="imagetest/20.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/21.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/22.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/23.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/24.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/25.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/26.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/27.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/28.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/29.png?ts=]]..ts..[["></td>
        </tr>
]])
mg.write([[
        <tr>
          <td><img src="imagetest/30.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/31.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/32.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/33.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/34.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/35.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/36.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/37.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/38.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/39.png?ts=]]..ts..[["></td>
        </tr>
]])
mg.write([[
        <tr>
          <td><img src="imagetest/40.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/41.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/42.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/43.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/44.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/45.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/46.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/47.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/48.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/49.png?ts=]]..ts..[["></td>
        </tr>
]])
mg.write([[
        <tr>
          <td><img src="imagetest/50.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/51.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/52.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/53.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/54.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/55.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/56.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/57.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/58.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/59.png?ts=]]..ts..[["></td>
        </tr>
]])
mg.write([[
        <tr>
          <td><img src="imagetest/60.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/61.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/62.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/63.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/64.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/65.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/66.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/67.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/68.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/69.png?ts=]]..ts..[["></td>
        </tr>
]])
mg.write([[
        <tr>
          <td><img src="imagetest/70.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/71.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/72.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/73.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/74.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/75.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/76.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/77.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/78.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/79.png?ts=]]..ts..[["></td>
        </tr>
]])
mg.write([[
        <tr>
          <td><img src="imagetest/80.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/81.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/82.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/83.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/84.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/85.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/86.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/87.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/88.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/89.png?ts=]]..ts..[["></td>
        </tr>
]])
mg.write([[
        <tr>
          <td><img src="imagetest/90.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/91.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/92.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/93.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/94.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/95.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/96.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/97.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/98.png?ts=]]..ts..[["></td>
          <td><img src="imagetest/99.png?ts=]]..ts..[["></td>
        </tr>
      </table>
]])
end
mg.write([[
    </p>
    <p id="timing">
      Test case: all images are displayed.
    </p>
  </body>
</html>
]])
