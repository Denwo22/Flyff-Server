// number 삽입 이미지 개수
// serialize 연속으로 이미지 삽입(width hight StartPosition-X StartPosition-Y, center_x, center_y)
// pos 단일로 이미지 삽입 (width hight StartPosition-X StartPosition-Y)
number 49
texture "icon\icon_PlayerData.dds" 0xffff00ff

serialize 7  32 32  1   1 0 0  // 큰 상태 아이콘
serialize 7  16 16  1  33 0 0  // 작은 상태 아이콘
serialize 5  16 16  1  49 0 0  // 1차직업 아이콘
serialize 8  16 16  1  65 0 0  // 2차직업 아이콘
serialize 7  16 16  1  81 0 0  // 마스터, 영웅 아이콘
serialize 10 16 16  1  97 0 0  // 서버 채널 아이콘
serialize 5  16 16  1 113 0 0  // 길드내 캐릭터 등급