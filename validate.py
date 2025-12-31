from datetime import datetime, timedelta
from lunarcalendar import Converter, Solar

# Mảng mapper cho Arduino (Dựa trên mốc 01/12/2025)
LUNAR_MAPPING = [
    -41, -11, 19, 49, 78, 108, 137, 167, 196, 225, 255, 284,
    313, 343, 373, 403, 432, 462, 492, 521, 551, 580, 609, 639,
    668, 697, 727, 757, 786, 816, 846, 876, 905, 935, 964, 993,
    1023, 1052, 1081, 1111, 1141, 1170, 1200, 1230, 1259, 1289, 1318, 1348,
    1377, 1407, 1436, 1465, 1495, 1525, 1554, 1584, 1613, 1643, 1673, 1702,
    1732, 1761, 1791, 1820, 1850, 1879, 1908, 1938, 1968, 1997, 2027, 2056,
    2086, 2116, 2145, 2175, 2204, 2234, 2263, 2293, 2322, 2351, 2381, 2410,
    2440, 2470, 2499, 2529, 2559, 2588, 2618, 2647, 2677, 2706, 2735, 2765,
    2794, 2824, 2853, 2883, 2913, 2943, 2972, 3002, 3031, 3061, 3090, 3119,
    3149, 3178, 3208, 3237, 3267, 3297, 3326, 3356, 3386, 3415, 3445, 3474,
    3503, 3533, 3562, 3591, 3621, 3651, 3680,
]

EPOCH_DATE = datetime(2025, 12, 1)

def get_mapper_lunar_day(target_date):
    days_since_epoch = (target_date - EPOCH_DATE).days
    current_mung_1 = -999
    # Tìm mùng 1 gần nhất trong quá khứ hoặc hiện tại
    for m in LUNAR_MAPPING:
        if m <= days_since_epoch:
            current_mung_1 = m
        else:
            break
    return (days_since_epoch - current_mung_1) + 1

# Một số mốc kiểm tra
test_dates = [
    datetime(2025, 12, 31), # Hôm nay
    datetime(2026, 2, 17),  # Mùng 1 Tết Bính Ngọ
    datetime(2027, 2, 6),   # Mùng 1 Tết Đinh Mùi
    datetime(2035, 1, 1)    # Một ngày xa trong tương lai
]

print("--- Kiểm tra nhanh một số mốc ---")
for d in test_dates:
    mapper_res = get_mapper_lunar_day(d)
    
    # Sửa lỗi AttributeError ở đây: Sử dụng Solar2Lunar thay vì SolarToLunar
    solar = Solar(d.year, d.month, d.day)
    actual_res = Converter.Solar2Lunar(solar).day
    
    status = "OK" if mapper_res == actual_res else "SAI"
    print(f"Ngày Dương {d.strftime('%d/%m/%Y')}: Mapper={mapper_res}, Thư viện={actual_res} -> {status}")