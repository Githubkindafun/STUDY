#
import matplotlib.pyplot as plt
import requests
from datetime import datetime, timedelta
from pprint import pprint



def get_data_NBP(year): # zbieramy dane
    start_date = datetime(year, 1, 1) # NBP pozwala na okres max 93 dni zatem bedziemy sobie dzielic rok na czesci

    datas = []
    try:
        while start_date.year == year:
            end_date = min(start_date + timedelta(days=91), datetime(year, 12, 31)) # koniec segmentu po 91 dniach albo koncu roku

            url = f"http://api.nbp.pl/api/exchangerates/rates/a/usd/{start_date.strftime('%Y-%m-%d')}/{end_date.strftime('%Y-%m-%d')}/?format=json"

            with requests.get(url) as req:
                data = req.json()
                datas.append(data)

            start_date = end_date + timedelta(days=1)
        
        return datas
    
    except requests.exceptions.RequestException as e: # sprawdzamy czy cos przy requescie sie nie wykrzaczylo
        print(f"Error occured : {e}")


def process_datas_NBP(datas): # przetwarzamy otrzymane dane
    #
    months = {} # dict wynikowy
    for month_num in range(1, 13):
        months[f"{month_num:02d}"] = {
            "sum": 0.0,
            "num": 0 #przyda sie do zaokraglania
        }
    
    for data in datas:
        for frame in data["rates"]:
            month_num = datetime.strptime(frame["effectiveDate"], "%Y-%m-%d").strftime("%m") # wyciagamy miesiac

            months[month_num]["num"] += 1
            months[month_num]["sum"] += frame["mid"] 

    result = {}
    for month, value in months.items():
        result[month] = round(value["sum"] / value["num"], 2) # tu zaookraglamy

    return result

# do wyciagania wartosci z wynikowego dicta   
def scrap_val(dt):
    res = []
    for i in range(1, len(dt) + 1):
        i = f"{i:02d}"
        res.append(dt[i])
    return res


datas_2021_NBP = get_data_NBP(2021)
months_2021_NBP = process_datas_NBP(datas_2021_NBP)


datas_2022_NBP = get_data_NBP(2022)
months_2022_NBP = process_datas_NBP(datas_2022_NBP)


predict2023 = [] # przewidujemy wartosci dolara w 23
for i in range(1,len(months_2022_NBP ) + 1):
    i = f"{i:02d}"
    predict2023.append(round(months_2022_NBP[i] * 1.042 , 2))

#pprint(predict2023)

# budujemy wykres
x = [m for m in months_2022_NBP] # miesiace

plt.plot(x, scrap_val(months_2021_NBP), color='green', marker='*')
plt.plot(x, scrap_val(months_2022_NBP), color='blue', marker='*')
plt.plot(x, predict2023, color='red', marker='*')
plt.legend(["2021", "2022", "2023-predict"])
plt.xlabel("month")
plt.ylabel("price")
plt.title("USD to PLN price")
#pprint(months_2021_NBP)
#pprint(months_2022_NBP)
plt.show()

