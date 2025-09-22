//////////////////////////////////////////////////////////////////////
//																	//
// 임시서버와 클라이언트들과의 전달패킷종류							//
//													by Songjunho	//
//																	//
//////////////////////////////////////////////////////////////////////

#define PH_CONNECT			"00"		// 접속허가 패킷 전달
#define PH_CONNECTOK		"01"		// 접속완료
#define PH_CNAME			"02"		// 이름정보 전달
#define PH_GAMESTART		"03"		// 게임시작메세지
#define PH_GAMESTARTNO		"04"		// 게임시작못했음 메세지
#define PH_LOADMAP			"05"		// 맵과 주사위맵핑 정보 전달
#define PH_MAPOK			"06"		// 맵을 다받음
#define PH_NAME_ERROR		"07"		// 이름이 동일해 접속을 거부함

#define PH_MESSAGE			"10"		// 대화메세지 전달
#define PH_DICE				"11"		// 주사위 굴린 합계보냄
#define PH_GAMENO			"12"		// 게임진행정도를 조정합니다.
#define PH_READY			"13"		// 초반세팅 수행
#define PH_BUILDING			"14"		// 세틀러먼트를 만듬
#define PH_ROAD				"15"		// 로드를 만듬

#define PH_BUY_CARD			"20"		// 카드구입
#define PH_BUY_CARDNO		"21"		// 카드구입실패 (카드떨어짐)

#define PH_USE_CARD			"30"		// 카드사용
#define PH_CARD_ROAD2		"31"		// 길짓기 카드사용
#define PH_CARD_ABUNDANCEA	"32"		// 풍년카드 사용
#define PH_CARD_MONOPOLY	"33"		// 독점카드 사용
#define PH_USE_MONOPOLY		"34"		// 독점카드 적용
#define PH_CARD_SOLDIER		"35"		// 군대카드 사용

#define PH_TRADE_HANGU		"40"		// 트레이드 (항구)
#define PH_TRADE_SOURCE		"41"		// 트레이드 (원본선택)
#define PH_TRADE_TARGET		"42"		// 트레이드 (타겟선택)

#define PH_RESOURCE			"50"		// 리소스 카드를 얻음
#define PH_RESOURCEOK		"51"		// 리소스 카드를 얻음완료
#define PH_RAND_RESOURCE	"52"		// 임의 카드를 다른이에게 뺏김

#define PH_PLAGUE_MOVE		"60"		// 도둑을 움직임 (직접)
#define PH_PLAGUE_WHITCH	"617"		// 도둑을 움직임 (간접)
#define PH_HALF_DROP		"62"		// 자원을 반을 얻음(자원 7개이상)
#define PH_HALFOK			"63"		// 자원을 반을 얻음완료

#define PH_TURN				"70"		// 턴을 줌
#define PH_TURN_END			"71"		// 턴을 끝냄

#define PH_GAMEEND			"80"		// 게임이 끝나버림

#define PH_DROP				"99"		// 어떤 이가 접속이 끊김