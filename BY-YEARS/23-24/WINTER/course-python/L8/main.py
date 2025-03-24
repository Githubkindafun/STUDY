import aiohttp
import asyncio
import json

from prywatne import JOKES_API_KEY

# pip install aiohttp


MTG_API_URL = "api.magicthegathering.io"
MTG_API_VERSION = "v1"
MTG_API_RESOURCE = "cards"

JOKES_API_URL = "api.humorapi.com"
JOKES_API_NUM = 7 # tu ile takich zartow
JOKES_API_KEYOWRDS = "man,car" # tutaj podajemy jakie słowa kluczowe chcemy
# UWAGA! to API ma limit dzienny i wrazie jakby został osiągniety to 
# trzeba nowe konto załozyc (jak będzie taka potrzeba to to zrobie)
# narazie dziala
# JOKES_API_KEY = ""

# https://api.magicthegathering.io/<version>/<resource>
async def get_cards(api_url, version, resource):
    url = f"https://{api_url}/{version}/{resource}/"
    headers = { # jak chce miec zwrocone dane
        'Content-Type': 'application/json'
    }
    try:
        # robimy to async aby bylo asynchronicznie
        async with aiohttp.ClientSession() as session: # otwieram sesje z dawca api
            async with session.get(url, headers=headers) as response: # pobiermay dane
                data = await response.json() # zwracamy response jako json, await stopuje funkcje do czasu wykonania zadania
                for card in data["cards"]:
                    print(f"name: {card["name"]} rarity: {card["rarity"]}")


    except aiohttp.ClientConnectionError as e:
        print(f"Error while connecting with client: {e}")

    except aiohttp.ClientResponseError as e:
        print(f"Error connected to server response: {e}")

    except Exception as e:
        print(f"Oh we have an error ;c : {e}")
                            



# https://api.humorapi.com/jokes/search
async def get_jokes(api_url, num, keywords, api_key):
    url = f"https://{api_url}/jokes/search?api-key={api_key}&number={num}&keywords={keywords}"
    headers = { # jak chce miec zwrocone dane
        'Content-Type': 'application/json'
    }
    try:
        # robimy to async aby bylo asynchronicznie
        async with aiohttp.ClientSession() as session: # otwieram sesje z dawca api
            async with session.get(url, headers=headers) as response: # pobiermay dane
                data = await response.json() # zwracamy response jako json, await stopuje funkcje do czasu wykonania zadania
                for joke in data["jokes"]:
                    print(f"joke: \n{joke["joke"]}")

    except aiohttp.ClientConnectionError as e:
        print(f"Error while connecting with client: {e}")

    except aiohttp.ClientResponseError as e:
        print(f"Error connected to server response: {e}")

    except Exception as e:
        print(f"Oh we have an error ;c : {e}")




async def main(): # async bo asyncio
    do = await asyncio.gather(
        get_cards(MTG_API_URL, MTG_API_VERSION, MTG_API_RESOURCE),
        get_jokes(JOKES_API_URL, JOKES_API_NUM, JOKES_API_KEYOWRDS, JOKES_API_KEY),
    )




asyncio.run(main())