import urllib.request, urllib.parse
import sys
from bs4 import BeautifulSoup

def main():   
    url = urllib.request.urlopen("http://th.if.uj.edu.pl/")
    if(url.getcode() == 200):
        if(url.info().get_content_type() == 'text/html'):
            soup = BeautifulSoup(url, 'html.parser')            
            text = soup.get_text()
            if "Institute of Theorethical Physics" in text:
                #print("done")
                sys.exit(0)      
            else:
                sys.exit(1)    
        else:
            sys.exit(1)         
    else:
        sys.exit(1)
        
main()            