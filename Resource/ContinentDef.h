
#pragma once

#if __VER >= 15 // __USING_CONTINENT_DATA

// 이 파일은 마드리갈의 대륙을 정의합니다
// 이 파일은 대륙이 추가될경우  대륙이름과 Index가 (미리)정의되어야 합니다.
// 대륙이 정의될경우 이름규칙을 꼭 지켜야 합니다. ( 접두사 CONT_ )

const BYTE CONT_NODATA			= 0;

const BYTE CONT_FLARIS			= 1;	// 플라리스 
const BYTE CONT_SAINTMORNING		= 2; // 세인트모닝
const BYTE CONT_RICIS			= 3; // 리시스의 정원
const BYTE CONT_ESTIA			= 4; // 이스티아 캐니언
const BYTE CONT_EAST			= 15; // 동부 : ( 15 > n ) -> 동부 각 대륙

const BYTE CONT_DARKON12		= 241; // 다콘 1, 2
const BYTE CONT_DARKON3			= 242; // 다콘 3
const BYTE CONT_HARMONIN		= 243; // 하르모닌
const BYTE CONT_WEST			= 240; // 서부 : ( n > 240 ) -> 서부 각 대륙

const BYTE CONT_ALL				= 255; // 군주

#endif