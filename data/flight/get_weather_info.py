import requests
import csv
import os
from concurrent.futures import ThreadPoolExecutor, as_completed

def get_city_code(city_name, api_key):
    url = "https://restapi.amap.com/v3/geocode/geo"
    params = {
        'key': api_key,
        'address': city_name
    }
    response = requests.get(url, params=params)

    if response.status_code == 200:
        geo_data = response.json()
        if geo_data['status'] == '1' and geo_data['infocode'] == '10000':
            return geo_data['geocodes'][0].get('adcode', None)
    return None

def get_weather(city_code, api_key):
    url = "https://restapi.amap.com/v3/weather/weatherInfo"
    params = {
        'city': city_code,
        'key': api_key
    }
    response = requests.get(url, params=params)

    if response.status_code == 200:
        data = response.json()
        return {
            'weather': data['lives'][0]['weather'],
            'temperature': data['lives'][0]['temperature'],
            'wind': data['lives'][0]['windpower']
        }
    return None

def fetch_weather(city_info, api_key):
    city_name = city_info['city'].strip()
    city_code = get_city_code(city_name, api_key)
    if city_code:
        weather_info = get_weather(city_code, api_key)
        if weather_info:
            city_info.update(weather_info)
    return city_info

def process_cities_concurrently(input_file_path, api_key):
    # 读取原始CSV文件内容
    with open(input_file_path, mode='r', encoding='utf-8-sig') as infile:
        reader = csv.DictReader(infile)
        fieldnames = reader.fieldnames  # 使用原始的列名
        rows = list(reader)

    updated_rows = []
    with ThreadPoolExecutor(max_workers=10) as executor:
        future_to_index = {executor.submit(fetch_weather, row, api_key): idx for idx, row in enumerate(rows)}
        for future in as_completed(future_to_index):
            idx = future_to_index[future]
            updated_rows.insert(idx, future.result())

    # 写入更新后的数据到原始文件
    with open(input_file_path, mode='w', encoding='utf-8', newline='') as outfile:
        writer = csv.DictWriter(outfile, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows(updated_rows)

# 给定的文件路径和API Key
input_file_path = './city_info.csv'
api_key = 'f7996b73dfa2df29c469c019db8e3544'

# 处理城市数据
process_cities_concurrently(input_file_path, api_key)