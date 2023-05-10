import urllib.request, urllib.parse
import sys
from bs4 import BeautifulSoup

def main():   
    url = urllib.request.urlopen("https://pogoda.interia.pl/prognoza-dlugoterminowa-krakow,cId,4970")
    
    if(url.getcode() == 200):    
        if(url.info().get_content_type() == 'text/html'):
            soup = BeautifulSoup(url, 'html.parser')            
            weather_tag = soup.find("div",class_="weather-currently-temp-strict")
            #print(weather_tag)
            if weather_tag is not None:
                if weather_tag.parent is not None and weather_tag.parent.name == "div":
                    #print(weather_tag.parent.name) 
                    weather = weather_tag.text 
                    if "°C" in weather:
                        temperature = weather.strip("°C")
                        temperature = int(temperature)
                        if temperature > -30 and temperature < 40:
                            
                            print(weather)
                            sys.exit(0)
                        else:
                            print("Bad temperature")
                            sys.exit(1)    
                    else:
                        print("There isn't a tempereture in °C")
                        sys.exit(1)
                else:
                    sys.exit(1)
            else:
                print("Div not found")
                sys.exit(1)                           
        else:
            sys.exit(1)    
    
    else:
        sys.exit(1)
        
main()            