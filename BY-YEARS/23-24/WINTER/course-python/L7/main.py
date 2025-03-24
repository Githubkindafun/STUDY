# Lista 7.
# czyli tak naprawde zad. 1. z Listy 6
# TO DO :
# [1] ERRORY [✓]
# [2] HTML <base> [✓]
# [3] crawl action jednoargumentowy [✓]
# [4] find sentance jako lista skladana [✓]

import threading
from bs4 import BeautifulSoup 
import requests
from urllib.parse import urljoin
import concurrent.futures


def find_sentance(text):
    #word = "Python"
    #sentances = text.split('.') # dzielimy tekst na zdania (zdania koncza sie kropka)
    #found = [] # tu bedziemy przechowywac znalezione zdania zawierajace "word"
    #
    #for sentance in sentances: # przeszukujemy zdania 
    #    if(sentance.find(word) != -1): # .find zwraca -1 gdy nie znajdzie szukanej
    #        found.append(sentance)
    #
    return [sentance for sentance in text.split('.') if sentance.find("Python") != -1] # [4] [3]

    #return found # zwracamy zdania z znalezionym slowem


def crawl(start_page, distance, action):
    visited = [] # nasze odwiedzone 
    lock = threading.Lock()

    def helper(url, curr_distance, executor):
        # sprawdzamy czy juz tu bylismy lub czy nie chcemy zejsc za gleboko
            with lock:
                if url in visited or curr_distance < 0: return
                print(f"{curr_distance} hop")
            
                visited.append(url) # dodajemy do odwiedzonych aktualny

            try:

                response = requests.get(url) # pobieramy i dostajemy info czy sukces 
                content = response.content # dostaje dostęp do zawartosic HTML
                soup = BeautifulSoup(content, 'html.parser') # formatujemy dane internetowe

                yield (url, action(soup.text)) # wypisujemy to co nas interesuje, .text aby byl w formacie jaki chcemy
                

                # tutaj obsluga <base> [2]
                base = soup.find('base', href=True) 
                if base: # sprawdzamy czy base jest
                    b_url = base['href']
                else:
                    b_url = url

                abs_links = []
                for link in soup.find_all('a', href=True): # szukamy <a> HTML i wyciągamy z nich href (href reprezentuje link)
                    #print(link)
                    href_value = link.get('href')
                    abs_link = urljoin(b_url, href_value) # tutaj uzyskujemy absolute url po to aby uniknac blednych sciezek
                    abs_links.append(abs_link)
                
                #abs_links = [urljoin(url, link) for link in all_links] # tutaj uzyskujemy absolute url po to aby uniknac blednych sciezek


                # TUTAJ ZMIANA WZGLEDEM ZADANIA Z LISTY 6

                futures = [executor.submit(helper, link, curr_distance - 1, executor) for link in abs_links] # tutaj submitujemy zadania do wykonania
                
                # wykonujemy pętle dla przyszłosci ktore sa juz wykonane
                for future in concurrent.futures.as_completed(futures): # i tak naprawde ten for dziala w nastepujacy sposob ze jak jakis sie wykona to go yielduje
                    yield from future.result() 



            # [1]
            except requests.exceptions.SSLError as e: # Error z protokolem do zabezpieczania transferu danych
                print(f"SSL Error with: {url}: {e}") # miedzy przegladarka urzytkownika a witryna

            except requests.exceptions.InvalidSchema as e: # errory z invalid inputem (nie rozpoznany / nie poprawny)
                print(f"Invalid Schema Error with: {url}: {e}")

            except requests.exceptions.ConnectionError as e: # errory przy laczeniu
                print(f"Connection Error with: {url}: {e}")

            except requests.exceptions.RequestException as e: # errory z request
                print(f"Request Error with: {url}: {e}")



    # TUTAJ ZMIANA WZGLEDEM ZADANIA Z LISTY 6
    with concurrent.futures.ThreadPoolExecutor() as executor: # executor "zarzadza" calym procesem
        yield from helper(start_page, distance, executor) # inicjalizuje tak naprawde caly crawling



# Co do bycia thread safe:
# uzyje Locka bo z tego co przeczytalem
# pomaga on nam dosc w problemie potencjalnego 
# nie bycia "thread safe"
# https://python.plainenglish.io/thread-safety-in-python-7441f8627d46

link = "https://www.ii.uni.wroc.pl"

for url, result in crawl(link, 1, find_sentance):
    print(f"{url}: {result}")