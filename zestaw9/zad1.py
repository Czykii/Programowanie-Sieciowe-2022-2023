import urllib.request, urllib.parse
import sys
from bs4 import BeautifulSoup

def error():
   print("Error occured")
   sys.exit(1)

def tag(weather_tag):
    if weather_tag is not None:
        parent(weather_tag)
    else:
        error()    

def parent(weather_tag):
    if weather_tag.parent is not None and weather_tag.parent.name == "div":
        weather = weather_tag.text
        is_Celsius(weather)
    else:
        error()

def is_Celsius(weather):
   if "°C" in weather:
        check_temperature(weather)
   else:
       error()

def check_temperature(weather):
    temperature = weather.strip("°C")
    temperature = int(temperature)
    if temperature > -30 and temperature < 40:
                            
        print(weather)
        sys.exit(0)
    else:
        error()

def main():   
    url = urllib.request.urlopen("https://pogoda.interia.pl/prognoza-dlugoterminowa-krakow,cId,4970")
    
    if(url.getcode() == 200):    
        if(url.info().get_content_type() == 'text/html'):
            soup = BeautifulSoup(url, 'html.parser')            
            weather_tag = soup.find("div",class_="weather-currently-temp-strict")
            tag(weather_tag)                           
        else:
            error()    
    
    else:
        error()
        
main()            