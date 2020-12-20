[en\_US](README.md) | [ko\_KR](README_ko_KR.md)

# node
패킷을 수집하고 파싱하여 collector로 보냄

## 사용방법

1. ``dependencies.sh``를 실행하여 시스템 환경을 준비하십시오.
1. 무선 LAN 카드를 연결하고 모니터 모드로 설정하십시오.
	1. ``iwconfig``명령으로 연결된 무선 LAN 카드의 인터페이스 이름을 확인하십시오. (예 : wlan0)
	1. 아래의 명령으로 모니터 모드로 변경하십시오.
		```
		sudo ifconfig wlan0 down
		sudo iwconfig wlan0 mode monitor
		sudo ifconfig wlan0 up
		```
1. ``bob0x10_core``를 실행하고 인수를 입력하십시오.
