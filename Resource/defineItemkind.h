#ifndef __DEFINE_ITEMKIND
#define __DEFINE_ITEMKIND

// IK_2는 탭의 의미적인 구분, IK_3은 아이템 생성에 사용

////////////////////////////////////////////////////////////////////////////
// 1차 구분 
//////////////////////////////////////////////////////////////////////////
#define IK1_GOLD                   0 // 돈<-
#define IK1_WEAPON                 1 // 아이템<-
#define IK1_ARMOR                  2 // 아이템<-
#define IK1_GENERAL                3 // 아이템<-
#define IK1_RIDE                   4 // 아이템<-
#define IK1_SYSTEM                 5 // <-
#define IK1_CHARGED				   6 // 상용화 아이템 여부 판단.
#define IK1_HOUSING					7	// 하우징

////////////////////////////////////////////////////////////////////////////
// 2차 구분 
//////////////////////////////////////////////////////////////////////////
#define IK2_GOLD                   0 // 돈<-
#define IK2_WEAPON_HAND            1 // 단거리<-
#define IK2_WEAPON_DIRECT          2 // 단거리<-
#define IK2_WEAPON_MAGIC           3 // 마법<-
#define IK2_ARMOR                  7 // 방어구(갑옷, 방패)<-
#define IK2_ARMORETC               8 // 주변 방어구(건틀렛 등등)<-
#define IK2_CLOTH                  9 // 의상(일반 의상들)<-
#define IK2_CLOTHETC              10 // 주변 의상(건틀렛 등등)<-
#define IK2_REFRESHER             11 // 향수 형태<-
#define IK2_POTION                12 // 물병 형태<-
#define IK2_JEWELRY               13 // 장신구<-
#define IK2_FOOD                  14 // 음식<-
#define IK2_MAGIC                 15 // 마법물품<-
#define IK2_GEM                   16 // 보석<-
#define IK2_MATERIAL              17 // 재조 재료<-
#define IK2_TOOLS                 18 // 도구<-
#define IK2_SYSTEM                19 // 시스템에 사용<-
#define IK2_RIDING                20 // 소형 비행체<-
#define IK2_MOB                   21 // <-몬스터에게 사용되는 아이템
#define IK2_BLINKWING             22 //<-
#define IK2_AIRFUEL               23 // 비행체 연료들<-
#define IK2_CHARM                 24 // 포스터
#define IK2_BULLET                25 // 발사체
#define IK2_TEXT                  26 // 문서
#define IK2_GMTEXT				  27 // GM명령
#define IK2_GENERAL				  28 // 장군아이템...이 아니고 걍 구분할거 없을때 이거 씀 -_-;;;;
#define	IK2_BUFF					29	// 버프 아이템
#define	IK2_WARP				30
#define IK2_SKILL				31
#define IK2_CLOTHWIG			32	// 머리의상(가발)
#define	IK2_BUFF2		33	// 버프 아이템 - 만료 시간
#define IK2_FURNITURE		34	// 하우징 - 가구
#define IK2_PAPERING		35	// 하우징 - 도배
#define	IK2_TOCASH		36	// 퍼니 코인
#define	IK2_BUFF_TOGIFT		37	// 버프 완료 후 아이템 지급
#define	IK2_GUILDHOUSE_FURNITURE		38 //길드하우스 가구
#define	IK2_GUILDHOUSE_NPC		39 //길드하우스 NPC
#define IK2_GUILDHOUSE_PAPERING 40 //길드하우스 도배
#define	IK2_GUILDHOUES_COMEBACK	41 //길드하우스 귀환 주문서


////////////////////////////////////////////////////////////////////////////
// 3차 구분 
//////////////////////////////////////////////////////////////////////////
#define IK3_GOLD                   0 //<-
// 무기에 관련된 것 (IK_WEAPON 소속)
#define IK3_HAND                   1 //<-
#define IK3_SWD                    2 //<-
#define IK3_AXE                    3 //<-
#define IK3_CHEERSTICK             4 // <-
#define IK3_KNUCKLEHAMMER          5 //<-
#define IK3_WAND                   6 //<-
#define IK3_STAFF                  7 //<-
#define IK3_THSWD                  8 // 양손 소드
#define IK3_THAXE                  9 // 양손 엑스
#define IK3_VIRTUAL               10 //<-몬스터가 사용하는 가상 아이템
#define IK3_YOYO                  11 //요요
#define IK3_BOW                   12 //활
#define IK3_YOBO				  13 //요요와 활을 동시에 사용

// 방어구에 관련된 것 (IK_ARMOR 소속)
#define IK3_SHIELD                16 // 방패<-
#define IK3_HELMET                17 // 머리 보호 방어구<-
#define IK3_SUIT                  18 // 상체(통짜) 보호 방어구<-
#define IK3_GAUNTLET              19 // 손 보호 방어구<-
#define IK3_BOOTS                 20 // 발 보호 방어구<-

// 의상에 관련된 것 (IK2_CLOTH 소속)
#define IK3_HAT                   21 // 모자 의상<-
#define IK3_MASK                  22 // 신발 의상<-
#define IK3_SHOES                 23 // 신발 의상<-
#define IK3_CLOAK                 24 // 망토 의상<-
#define IK3_CLOTH                 57 // 옷
#define IK3_GLOVE                 58 // 장갑

// 일반 아이템들
#define IK3_REFRESHER             25 // 정신력 치료<-
#define IK3_POTION                26 // 각종 물병<-
#define IK3_EARRING               27 // 귀걸이<-
#define IK3_NECKLACE              28 // 목걸이<-
#define IK3_RING                  29 // 반지<-
#define IK3_INSTANT               30 // 즉석으로 먹을수 있게 만들어진 음식<-
#define IK3_COOKING               31 // 요리된 음식(음식)<-
#define IK3_ICECEARM              32 // 만들어진 아이스크림2<-
#define IK3_PILL                  59 // 영양 보조 알약
#define IK3_MAGICTRICK            33 // 각종 마법용품<-
#define IK3_GEM                   34 // 그냥 보석<-
#define IK3_DRINK                 35 // 활력제<-
#define IK3_COLLECTER		      36 // 콜렉터<-
#define IK3_ELECARD		          37 // 속성 카드<-
#define IK3_DICE		          38 // 파워 주사위<-
#define IK3_SUPSTONE		      39 // 보조석<-
 
// 탈것
#define IK3_BOARD                 40 // 보드 형태의 탈 것(손사용가능)<-
#define IK3_STICK                 41 // 올라 타는 것(손사용불능)<-
#define IK3_EVENTMAIN             42 // <-이벤트용 아이템  지급자동, 판매, 트레이드, 타임줍기 불가
#define IK3_QUEST                 43 // 퀘스트용 아이템<-
#define IK3_MAP                   44 // 퀘스트용 아이템<-
#define IK3_BLINKWING             45 // 퀘스트용 아이템<-
#define IK3_EVENTSUB              46 // <-이벤트용 아이템  지급자동, 일반아이템과 동일
#define IK3_TOWNBLINKWING         47 // <-마을로만 돌아가게 되는 블링크윙

//비행체 아이템2
#define IK3_ACCEL		          48 // 가속 연료 아이템<-
#define IK3_DELETE		          49 // <-dwItemkind3가 이것으로 설정되면 캐릭터가 로딩될 때 자동으로 지운다.

//상용화 아이템
#define IK3_SCROLL		        50 //<-
#define IK3_ENCHANTWEAPON		51
#define IK3_CFLIGHT		        52
#define IK3_BFLIGHT		        53
#define IK3_MAGICBOTH			54
#define IK3_BCHARM		        55
#define IK3_RCHARM		        56 
#define IK3_ARROW		        60

//소켓 카드 와 피어싱 주사위
#define IK3_PIERDICE			61
#define IK3_SOCKETCARD			62
#define IK3_SOCKETCARD2			63

//문서 구분
#define IK3_TEXT_BOOK           	70 // 문서
#define IK3_TEXT_SCROLL	        	71 // 문서
#define IK3_TEXT_LETTER         	72 // 문서

//운영자 명령 아이템화
#define	IK3_TEXT_UNDYING		80
#define	IK3_TEXT_DISGUISE		81
#define IK3_TEXT_INVISIBLE		82
#define IK3_TEXT_GM			83

// BINDS
#define	IK3_BINDS			84

// 몬스터 소환
#define IK3_CREATE_MONSTER		85

#define IK3_POTION_BUFF_STR		90		// 버프 물약
#define IK3_POTION_BUFF_DEX		91		// 버프 물약
#define IK3_POTION_BUFF_INT		92		// 버프 물약
#define IK3_POTION_BUFF_STA		93		// 버프 물약
#define IK3_POTION_BUFF_DEFENSE	94		// 버프 물약

#define IK3_ANGEL_BUFF			95		// 엔젤 버프

// PET
#define IK3_PET				100		// 펫소환 아이템
#define IK3_RANDOM_SCROLL		101		// 랜덤 스클롤

#define IK3_ULTIMATE			102		// 얼터멋 웨폰 보석

#define	IK3_LINK	104
// General
#define IK3_GENERAL			118		// 아무 구분이 없는 일반.

#define IK3_ENCHANT			119

#define IK3_EGG		120		// 성장 펫
#define	IK3_FEED	121

#define	IK3_TICKET	122		// 입장권
#define	IK3_POCKET	123		// 주머니

#define IK3_BED			124		// 하우징 - 가구 - 침대
#define IK3_SOFA		125		// 하우징 - 가구 - 소파
#define IK3_WARDROBE	126		// 하우징 - 가구 - 옷장
#define IK3_CLOSET		127		// 하우징 - 가구 - 화장대
#define IK3_TABLE		128		// 하우징 - 가구 - 탁자
#define IK3_CABINET		129		// 하우징 - 가구 - 장식장
#define IK3_PROPS		130		// 하우징 - 가구 - 소품
#define IK3_WALLPAPER	131		// 하우징 - 도배 - 벽지
#define IK3_CARPET		132		// 하우징 - 도배 - 장판
#define	IK3_COUPLE_BUFF	133		// 커플 효과
#define	IK3_FUNNYCOIN	134		// 퍼니 코인
#define	IK3_FLOWER	135		// 겹침 방지를 위한 아이템. 같은 종류 사용 안됨
#define IK3_BALLOON	136		// 풍선
#define IK3_WING	137		// 날개

#define IK3_VIS		138		// 비스
#define	IK3_TS_BUFF	139

#define IK3_TELEPORTER		140 //길드하우스 텔레포터
#define IK3_REST			141	//휴식의 기운
#define	IK3_DESK			142	// 하우징 - 가구 - 책상
#define	IK3_CHAIR			143	// 하우징 - 가구 - 의자
#define	IK3_CASE			144	// 하우징 - 가구 - 책장
#define	IK3_BATH			145	// 하우징 - 가구 - 욕조
#define	IK3_DRAWER			146	// 하우징 - 가구 - 협탁

#define	IK3_CRYSTAL			147

#define IK3_KEY				148

#define MAX_ITEM_KIND3      149	// ItemKind3 총 개수 
#define MAX_UNIQUE_SIZE	    400

// 컨트롤 오브젝트 Kind
#define CK1_CHEST				0    // <-보물상자
#define CK1_DOOR				1    // <-문
#define CK1_TRIGGER				2    // 동작 오브젝트
#define CK1_HOUSING				3    // 하우징 가구 컨트롤
#define	CK1_GUILD_HOUSE				4    // 길드 가구 컨트롤

#define CK2_FADE				1    // 사라지는 컨트롤
#define CK2_KEEP				2    // 없어지지 않는 컨트롤

#define CK3_FULL				1    // 풀에니메이션
#define CK3_HALF				2    // 1/2 에니메이션

#define WEAPON_GENERAL			0	// 일반 무기
#define WEAPON_UNIQUE			1	// 유니크 무기
#define WEAPON_ULTIMATE			2	// 얼터멋 무기

#define ARMOR_SET				3	// 세트 방어구

#define PET_VIS					1	// VisPet
#endif