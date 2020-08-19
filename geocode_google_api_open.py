import requests
import urllib.request
import json
from urllib.parse   import quote

def json_type(s):
    ans = ""
    for elem in s:
        if (elem == '\n' or elem == ' '):
            continue
        if (elem == "'"):
            ans += '"'
        else:
            ans += elem
    return ans

url = "https://maps.googleapis.com/maps/api/geocode/json?address=" 
apikey = "***"
address = "#"
url = url + quote(address) + apikey
response = urllib.request.urlopen(url)
s = response.read()
content =  s.decode(response.headers.get_content_charset())  
data = json.loads(content)
if (len(data["results"]) == 0):
    print("Incorrect address")
else:
    lat = data["results"][0]["geometry"]["location"]["lat"]
    lng = data["results"][0]["geometry"]["location"]["lng"]
    print(lat, lng)
