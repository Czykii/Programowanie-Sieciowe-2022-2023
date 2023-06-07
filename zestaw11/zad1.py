import requests
import sys
import json

# Budka Suflera - 359282

def get_artists(band_id):
    url = f'https://api.discogs.com/artists/{band_id}'
    headers = {'User-Agent': 'DiscogsAppMusic/1.0'}
    
    page = requests.get(url, headers)
    content = page.content.decode('utf8')
    data = json.loads(content)
    return data['members']
   
def check(id_arg, name, url):
    headers = {'User-Agent': 'DiscogsAppMusic/1.0'}
    
    page = requests.get(url, headers)
    content = page.content.decode('utf8')
    data = json.loads(content)
    bands = data['groups']

    for band in bands:
        if band["id"] != id_arg and band["name"] in other_bands.keys():
            other_bands[band["name"]].append(name)
        elif band["id"] != id_arg and band["name"] not in other_bands.keys():
            other_bands[band["name"]] = [name]   

def main():
    band_id = int(sys.argv[1])
    global other_bands
    other_bands = {}
    artists = get_artists(band_id)
    for artist in artists:
        check(band_id, artist["name"], artist["resource_url"])

    for i in sorted(other_bands):
        if (len(other_bands[i]) > 1):
            print(f'{i}: {", ".join(other_bands[i])}')    

main()