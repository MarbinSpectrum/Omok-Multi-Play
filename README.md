# 멀티 플레이 오목게임


멀티플레이가 가능한 오목게임입니다.

----------------------------------------------------------
<img src="./오목 게임 설계도.png" width="100%">


|프로토콜이름|파라미터|설명|
|------|---|---|
|CONNECT_CLIENT|int64(소캣번호)|클라이언트와 서버가 연결됨|
|DISCONNECT_CLIENT|int64(소캣번호)|클라이언트와 서버가 연결이 해제됨|
|LOBBY_ENTER_REQUEST|string(아이디)|클라이언트의 로비 입장 요청|
|LOBBY_ENTER_REPLY|bool(승인여부)|로비 입장 승인 여부 반환|
|LOBBY_ROOM_DATA_REQUEST|  |로비의 방 데이터를 요청|
|LOBBY_ROOM_DATA_REPLY|int(방갯수)[방갯수만큼반복](string(방이름), uint(방번호),int64(방Key),int(인원수)]|로비의 방들의 정보 요청의 처리|
|MAKE_ROOM_REQUEST|  |방을 만드는 것을 요청|
|MAKE_ROOM_REPLY|bool(승인여부)|방을 만들수 있는지 처리|
|ENTER_ROOM_REQUEST|unit(방번호),int64(방Key)|방에 입장하는 것을 요청|
|ENTER_ROOM_REPLY|bool(승인여부)|방에 입장 할 수 있는지 처리|
|EXIT_ROOM_REQUEST|  |방에 퇴장하는 것을 요청|
|EXIT_ROOM_REPLY|bool(승인여부)|방 퇴장 처리|
|GAMEROOM_DATA_REQUEST|  |방의 세부 정보를 요청|
|GAMEROOM_DATA_REPLY|bool(성공여부),string(호스트 아이디),bool(당신이 호스트인지 여부),uint(방의 인원수)[방의 인원수만큼 반복][string(아이디), bool(준비여부)]|방 퇴장 처리|
|GAMEROOM_READY_UPDATE|bool(준비상태)|게임 준비상태 갱신 요청|
|GAMEROOM_GAME_START_REQUEST|  |게임 시작 요청|
|GAMEROOM_GAME_START_REPLY|bool(시작 여부)|게임 시작 요청 처리|
|GAMEBOARD_DATA_REQUEST|  |게임판 상태를 요청|
|GAMEBOARD_DATA_REPLY|bool(성공여부),bool(플레이어의 턴인지 여부),int(기물 갯수)[기물 갯수만큼 반복][int(세로위치),int(가로위치),string(기물종류)]|게임판 정보를 받음|
|GAMEBOARD_SET_PIECE|int(세로),int(가로)|게임판에 기물 배치 요청|
|GAME_RESULT_REQUEST|bool(승패여부)|게임 결과를 처리요청|
|GAME_RESULT_REPLY|bool(처리여부)|게임 결과를 처리|
