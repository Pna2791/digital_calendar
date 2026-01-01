#ifndef TIMESTAMP_TO_LUNAR_DATE_H
#define TIMESTAMP_TO_LUNAR_DATE_H

#include <Arduino.h>
#include <avr/pgmspace.h>

// Mốc gốc (Day 0): 01/12/2025 (Dương lịch)
// Mảng chứa số ngày kể từ mốc gốc đến ngày Mùng 1 của các tháng Âm lịch tiếp theo
// Sử dụng static để tránh lỗi đa định nghĩa nếu include nhiều nơi
static const int16_t LUNAR_MAPPING[] PROGMEM = {
    -11,   // Mùng 1 tháng 10 năm Ất Tỵ (rơi vào 20/11/2025)
    19,    // Mùng 1 tháng 11 (20/12/2025)
    49,    // Mùng 1 tháng 12 (19/01/2026)
    79,    // Mùng 1 tháng 1 năm Bính Ngọ (Tết 2026 - 17/02/2026)
    109, 138, 168, 197, 227, 256, 286, 315, 345, 374, 404, // 2026
    433, 463, 492, 521, 551, 580, 610, 639, 669, 699, 728, 758, 787, // 2027 (có nhuận)
    817, 846, 875, 905, 934, 963, 993, 1022, 1052, 1082, 1111, 1141, // 2028
    1171, 1200, 1229, 1259, 1288, 1317, 1347, 1376, 1406, 1436, 1466, 1495, // 2029
    1525, 1554, 1583, 1613, 1642, 1671, 1700, 1730, 1759, 1789, 1819, 1849, // 2030
    1878, 1908, 1937, 1966, 1996, 2025, 2054, 2083, 2113, 2142, 2172, 2202, 2232, // 2031 (nhuận)
    2261, 2291, 2320, 2349, 2379, 2408, 2437, 2466, 2496, 2525, 2555, 2585, // 2032
    2615, 2644, 2674, 2703, 2732, 2762, 2791, 2820, 2849, 2879, 2908, 2938, 2968, // 2033 (nhuận)
    2998, 3027, 3057, 3086, 3115, 3145, 3174, 3203, 3232, 3262, 3291, 3321, // 2034
    3351, 3381, 3410, 3440, 3469, 3498, 3528, 3557, 3586, 3615, 3645, 3674  // 2035
};

static const int LUNAR_SIZE = sizeof(LUNAR_MAPPING) / sizeof(LUNAR_MAPPING[0]);
static const uint32_t EPOCH_01_12_2025 = 1764547200; // Timestamp của 01/12/2025 00:00:00 UTC

// Hàm chuyển đổi timestamp sang ngày Âm lịch (1-30)
// Trả về -1 nếu ngoài phạm vi
inline int getLunarDay(uint32_t currentTimestamp) {
    if (currentTimestamp < EPOCH_01_12_2025) return -1; // Ngoài phạm vi hỗ trợ

    // Tính số ngày chênh lệch so với mốc 01/12/2025
    long daysSinceEpoch = (currentTimestamp - EPOCH_01_12_2025) / 86400;

    // Tìm trong mảng để xác định ngày mùng 1 gần nhất
    for (int i = LUNAR_SIZE - 1; i >= 0; i--) {
        int16_t monthStartDay = pgm_read_word(&LUNAR_MAPPING[i]);
        if (daysSinceEpoch >= monthStartDay) {
            // Ngày âm lịch = (Số ngày hiện tại - Ngày mùng 1) + 1
            return (daysSinceEpoch - monthStartDay) + 1;
        }
    }
    return -1;
}

#endif