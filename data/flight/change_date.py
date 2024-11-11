import random
from datetime import datetime, timedelta

# 城市列表
cities = ["北京", "哈尔滨", "呼和浩特", "乌鲁木齐", "广州", "拉萨", "昆明", "成都", "上海", "南宁", "伦敦", "东京",
          "悉尼", "莫斯科", "巴黎", "纽约"]

# 城市间距离（单位：公里），实际应用中这些数据应该基于实际航线距离
city_distances = {
    ("北京", "哈尔滨"): 1100,
    ("北京", "呼和浩特"): 450,
    ("北京", "乌鲁木齐"): 2200,
    ("北京", "广州"): 2100,
    ("北京", "拉萨"): 2200,
    ("北京", "昆明"): 2000,
    ("北京", "成都"): 1600,
    ("北京", "上海"): 1100,
    ("北京", "南宁"): 1900,
    ("北京", "伦敦"): 8200,
    ("北京", "东京"): 2200,
    ("北京", "莫斯科"): 5800,
    ("北京", "巴黎"): 7800,
    ("北京", "纽约"): 11000,
    ("北京", "悉尼"): 6500,
    # 添加其他城市间的距离
}


# 模拟生成航班数据
def generate_flight_data(cities, city_distances):
    # 2024年11月1日
    base_date = datetime(2024, 11, 1)

    # 存储航班数据的列表
    flights = []

    # 随机生成航班数据
    for _ in range(100):  # 假设有30个航班
        dep_city = random.choice(cities)
        arr_city = random.choice(cities)
        while arr_city == dep_city:
            arr_city = random.choice(cities)  # 确保出发地和目的地不同

        # 获取两城市间的距离
        distance = city_distances.get((dep_city, arr_city))
        if not distance:
            distance = city_distances.get((arr_city, dep_city))

        if not distance:  # 如果没有距离数据，则跳过
            continue

        # 根据距离计算票价，假设每公里0.1元
        price = distance * 1.03

        # 假设基准飞行速度为800公里/小时
        speed = 700  # 公里/小时
        flight_duration = int((distance / speed) * 60)  # 转换为分钟

        # 随机生成出发时间和到达时间
        dep_time = base_date + timedelta(minutes=random.randint(0, 1440 - flight_duration))
        arr_time = dep_time + timedelta(minutes=flight_duration)

        # 确保到达时间不早于出发时间
        while arr_time < dep_time:
            arr_time += timedelta(minutes=1)

        # 随机生成余座数
        seats = random.randint(0, 200)  # 余座数在0到200之间

        # 格式化时间
        dep_time_str = dep_time.strftime('%Y-%m-%d %H:%M')
        arr_time_str = arr_time.strftime('%Y-%m-%d %H:%M')

        # 添加到航班数据列表
        flights.append((dep_city, dep_time_str, arr_city, arr_time_str, round(price, 2), seats))

    return flights


# 将航班数据写入TXT文件
def write_flights_to_file(flights, filename):
    with open(filename, 'w', encoding='utf-8') as file:
        for flight in flights:
            # 将每个元素用英文逗号隔开，并写入文件，每个航班数据占一行
            file.write(', '.join(map(str, flight)) + '\n')


# 生成航班数据并写入文件
flights = generate_flight_data(cities, city_distances)
write_flights_to_file(flights, 'flight_data.txt')