from datetime import datetime, timedelta
# Sử dụng thư viện lunardate (Cài đặt: pip install lunardate)
from lunardate import LunarDate 

def generate_arduino_mapper():
    # Cấu hình
    start_date = datetime(2025, 12, 1)
    end_date = datetime(2035, 12, 31)
    epoch_date = datetime(2025, 12, 1)
    
    lunar_mung_1_offsets = []
    
    print("// Mapper: So ngay ke tu 01/12/2025 den ngay Mung 1 Am Lich")
    print("const int16_t LUNAR_MAPPING[] PROGMEM = {")
    
    current_date = start_date - timedelta(days=45) # Quay lai 1 chut de tim mung 1 gan nhat
    
    last_found_mung_1 = None
    
    while current_date <= end_date:
        # Lay thong tin am lich cua ngay duong lich hien tai
        lunar = LunarDate.fromSolarDate(current_date.year, current_date.month, current_date.day)
        
        # Neu la ngay mung 1
        if lunar.day == 1:
            offset = (current_date - epoch_date).days
            lunar_mung_1_offsets.append(str(offset))
            
        current_date += timedelta(days=1)

    # In ra dinh dang mang C++
    formatted_output = ", ".join(lunar_mung_1_offsets)
    # Tu dong xuong dong sau moi 12 phan tu cho dep mat
    parts = formatted_output.split(", ")
    for i in range(0, len(parts), 12):
        print("    " + ", ".join(parts[i:i+12]) + ",")
        
    print("};")
    print(f"// Tong so thang: {len(parts)}")

if __name__ == "__main__":
    # Neu chua co lunardate, hay chay: pip install lunardate
    try:
        generate_arduino_mapper()
    except ImportError:
        print("Vui long cai dat thu vien: pip install lunardate")