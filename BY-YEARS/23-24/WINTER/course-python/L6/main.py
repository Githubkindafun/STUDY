# zad.1. Lista 6.
#
from bs4 import BeautifulSoup 
import requests
from urllib.parse import urljoin

def find_sentance(word, text):
    sentances = text.split('.') # dzielimy tekst na zdania (zdania koncza sie kropka)
    found = [] # tu bedziemy przechowywac znalezione zdania zawierajace "word"
 
    for sentance in sentances: # przeszukujemy zdania 
        if(sentance.find(word) != -1): # .find zwraca -1 gdy nie znajdzie szukanej
            found.append(sentance)

    return found # zwracamy zdania z znalezionym slowem


def crawl(start_page, distance, action):
    visited = [] # nasze odwiedzone 

    def helper(url, curr_distance):
        # sprawdzamy czy juz tu bylismy lub czy nie chcemy zejsc za gleboko
        if url in visited or curr_distance < 0: return

        visited.append(url) # dodajemy do odwiedzonych aktualny
        
        try:

            response = requests.get(url) # pobieramy i dostajemy info czy sukces 
            content = response.content # dostaje dostęp do zawartosic HTML
            soup = BeautifulSoup(content, 'html.parser') # formatujemy dane internetowe

            yield (url, action("Python", soup.text)) # wypisujemy to co nas interesuje, .text aby byl w formacie jaki chcemy
            

            all_links = []
            for link in soup.find_all('a', href=True): # szukamy <a> HTML i wyciągamy z nich href (href reprezentuje link)
                #print(link)
                href_value = link.get('href')
                all_links.append(href_value)
            
            
            for link in all_links:
                abs_url = urljoin(url, link) # tutaj uzyskujemy absolute url po to aby uniknac blednych sciezek
                yield from helper(abs_url, curr_distance - 1) # rekursja

        except Exception as e:
            print(f"Error z: {url}: {e}")

    yield from helper(start_page, distance) # inicjalizuje tak naprawde caly crawling


link = "https://www.ii.uni.wroc.pl"

for url, result in crawl(link, 1, find_sentance):
    print(f"{url}: {result}")
