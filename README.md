# Murbong - MIDI Player

Using C++ & Windows MIDI Library



##### #MIDI reference

<https://www.csie.ntu.edu.tw/~r92092/ref/midi/>

##### #MIDI File Structure

> Example.mid 파일의 헥스 코드입니다.4D 54 68 64 = "MThd" 로 시작하는 헤더 포맷이 있어야만 Mid 파일이라 볼 수 있습니다.
>
> This Example.mid 파일은 낮은 도부터 높은 도까지 반음씩 올라가는 코드입니다.
>
> 4D 54 68 64 00 00 00 06 00 01 00 03 00 C0 4D 54 72 6B 00 00 00 8D 00 FF 03 0E 50 69 61 6E 6F 20 54 65 6D 70 6C 61 74 65 00 FF 01 09 42 79 20 3C 4E 61 6D 65 3E 00 FF 02 1D 43 6F 70 79 72 69 67 68 74 20 C2 A9 20 3C 59 65 61 72 3E 20 62 79 20 3C 4E 61 6D 65 3E 00 FF 02 13 41 6C 6C 20 52 69 67 68 74 73 20 52 65 73 65 72 76 65 64 00 FF 01 20 47 65 6E 65 72 61 74 65 64 20 62 79 20 4E 6F 74 65 57 6F 72 74 68 79 20 43 6F 6D 70 6F 73 65 72 00 FF 59 02 00 00 00 FF 58 04 04 02 18 08 00 FF 2F 00 4D 54 72 6B 00 00 02 4B 00 FF 21 01 00 00 FF 03 0A 52 69 67 68 74 20 48 61 6E 64 00 C0 00 00 E0 00 40 00 B0 07 7F 00 B0 0A 40 00 90 24 6E 81 20 90 24 00 20 90 25 6E 81 20 90 25 00 20 90 26 6E 81 20 90 26 00 20 90 27 6E 81 20 90 27 00 20 90 28 6E 81 20 90 28 00 20 90 29 6E 81 20 90 29 00 20 90 2A 6E 81 20 90 2A 00 20 90 2B 6E 81 20 90 2B 00 20 90 2C 6E 81 20 90 2C 00 20 90 2D 6E 81 20 90 2D 00 20 90 2E 6E 81 20 90 2E 00 20 90 2F 6E 81 20 90 2F 00 20 90 30 6E 81 20 90 30 00 20 90 31 6E 81 20 90 31 00 20 90 32 6E 81 20 90 32 00 20 90 33 6E 81 20 90 33 00 20 90 34 6E 81 20 90 34 00 20 90 35 6E 81 20 90 35 00 20 90 36 6E 81 20 90 36 00 20 90 37 6E 81 20 90 37 00 20 90 38 6E 81 20 90 38 00 20 90 39 6E 81 20 90 39 00 20 90 3A 6E 81 20 90 3A 00 20 90 3B 6E 81 20 90 3B 00 20 90 3C 6E 81 20 90 3C 00 20 90 3D 6E 81 20 90 3D 00 20 90 3E 6E 81 20 90 3E 00 20 90 3F 6E 81 20 90 3F 00 20 90 40 6E 81 20 90 40 00 20 90 41 6E 81 20 90 41 00 20 90 42 6E 81 20 90 42 00 20 90 43 6E 81 20 90 43 00 20 90 44 6E 81 20 90 44 00 20 90 45 6E 81 20 90 45 00 20 90 46 6E 81 20 90 46 00 20 90 47 6E 81 20 90 47 00 20 90 48 6E 81 20 90 48 00 20 90 49 6E 81 20 90 49 00 20 90 4A 6E 81 20 90 4A 00 20 90 4B 6E 81 20 90 4B 00 20 90 4C 6E 81 20 90 4C 00 20 90 4D 6E 81 20 90 4D 00 20 90 4E 6E 81 20 90 4E 00 20 90 4F 6E 81 20 90 4F 00 20 90 50 6E 81 20 90 50 00 20 90 51 6E 81 20 90 51 00 20 90 52 6E 81 20 90 52 00 20 90 53 6E 81 20 90 53 00 20 90 54 6E 81 20 90 54 00 20 90 55 6E 81 20 90 55 00 20 90 56 6E 81 20 90 56 00 20 90 57 6E 81 20 90 57 00 20 90 58 6E 81 20 90 58 00 20 90 59 6E 81 20 90 59 00 20 90 5A 6E 81 20 90 5A 00 20 90 5B 6E 81 20 90 5B 00 20 90 5C 6E 81 20 90 5C 00 20 90 5D 6E 81 20 90 5D 00 20 90 5E 6E 81 20 90 5E 00 20 90 5F 6E 81 20 90 5F 00 20 90 60 6E 81 20 90 60 00 00 FF 2F 00 4D 54 72 6B 00 00 00 25 00 FF 21 01 00 00 FF 03 09 4C 65 66 74 20 48 61 6E 64 00 C1 00 00 E1 00 40 00 B1 07 7F 00 B1 0A 40 00 FF 2F 00

##### #어떻게 분석하나요?

의사코드로 설명하겠습니다.


`Raw <- File read "Example.mid"`

`Repeat until File Pointer is File End`

`Header	<- Read 4 Byte in Raw`

`Length	<- Read 4 Byte in Raw`

`Data	<- Read Length Byte in Raw`

`if Header == "MThd"`



`Format	<- Read 2 Byte in Data`

`Tracks	<- Read 2 Byte in Data`

`TimeDivision	<- Read 2 Byte in Data`

`TickPerQuotes	<- TimeDivision & 0x7FFF` ; 14~8비트는 필요하지 않습니다.



`else if Header == "MTrk"`



`TrackData[Count].Time	<-VLQ(Data)`

`TrackData[Count].Data	<- Data`

`Count++`



`endif`

`Repeat End`

초기화 끝. 그래서 VLQ는 뭐고 트랙은 어떻게 분석하나요?

**#MIDI reference**에서는, VLQ를 이렇게 정의했습니다.

> Variable Length Quantities
>
> The *variable-length quantity* provides a convenient means of representing arbitrarily large integers, without creating needlessly large fixed-width integers.
>
> A *variable-length quantity* is a represented as a series of 7-bit values, from most-significant to least-significant. where the last byte of the series bit 7 (the most significant bit) set to 0, and the preceding bytes have bit 7 set to 1.

ReadVLQ는 7비트를 읽습니다. 만약 읽은 비트가 1XXX XXXX이 아니라면 결과를 바로 리턴합니다.

`function ReadVLQ(HexCode)`

`Result = 0`

`for(range 1,7) do`

`Temp	<-Read 1 Byte in HexCode`

`Result = Result << 7` ; Bitshiftleft

`Result = Result |(Temp & 0x7F);` ; bitor and bitand

`if (Temp & 0x80) is not 0x80`

`return Result`

`endif`

`end`



`return Result`



`end function`

이제, 트랙을 분석해봅시다.



> FF 03 0E 50 69 61 6E 6F 20 54 65 6D 70 6C 61 74 65 00 FF 01 09 42 79 20 3C 4E 61 6D 65 3E 00 FF 02 1D 43 6F 70 79 72 69 67 68 74 20 C2 A9 20 3C 59 65 61 72 3E 20 62 79 20 3C 4E 61 6D 65 3E 00 FF 02 13 41 6C 6C 20 52 69 67 68 74 73 20 52 65 73 65 72 76 65 64 00 FF 01 20 47 65 6E 65 72 61 74 65 64 20 62 79 20 4E 6F 74 65 57 6F 72 74 68 79 20 43 6F 6D 70 6F 73 65 72 00 FF 59 02 00 00 00 FF 58 04 04 02 18 08 00 FF 2F



첫번째 1 바이트는 'Midi EVENT'. **#MIDI reference**에서는 Midi Event를 3가지로 구분할 수 있다고 했습니다.

Midi Event is 	0xFF 		,META

​				0xF0 or 0xF7 ,SYS

​				Anythingelse	,Channel



`META Type 	<-Read 1 Byte in Trackdata`

`META Length	<-ReadVLQ(Trackdata)`

`META Data	<- Read Length Byte in Trackdata`



## META List

| Type |            Action             |
| :--: | :---------------------------: |
|  00  |        Sequence Number        |
|  01  |          Text Event           |
|  02  |       Copyright Notice        |
|  03  |      Sequence/Track Name      |
|  04  |        Instrument Name        |
|  05  |             Lyric             |
|  06  |            Marker             |
|  07  |           Cue Point           |
|  20  |      MIDI Channel Prefix      |
|  2F  |         End of Track          |
|  51  |           Set Tempo           |
|  54  |         SMTPE Offset          |
|  58  |        Time Signature         |
|  59  |         Key Signature         |
|  7F  | Sequencer-Specific Meta-event |

`Tempo	<-60000000 /META Data`



Sysex 이벤트는 Midi 신디사이저에 사용되는 코드이므로, 스킵하셔도 됩니다.



`Sysex Length	<-ReadVLQ(Trackdata)`

`Sysex Data	<- Read Length Byte in Trackdata`



이제 마지막으로, 채널 이벤트입니다.

미디 이벤트가 0xF0,0xFF가 아니라면 채널 이벤트로 분석이 가능합니다.

**하지만 채널 이벤트는 동일한 이벤트가 뒤에 있을때 생략되므로, 전처리가 필요합니다.**

`if Midi Evnet & 0x80 is 0`

`Parameter 	<- Midi Event`

`Midi Event	<-Tracks Last Event`

`else`

`Parameter	<- Read 1 Byte in Trackdata`

`Tracks Last Event	<- Midi Event`

`endif`

그리고

`Channel	<- Midi Event & 0x0F`

`Event	<- Midi Event >> 4`



## MIDI Event List

| Event |                 Action                  |
| :---: | :-------------------------------------: |
|   8   |                Note off                 |
|   9   |                 Note on                 |
|   A   |         Polyphonic Key Pressure         |
|   B   |  Controller Change(pedal, slider, etc)  |
|   C   | Program Change(Set Instrument or sound) |
|   D   |          Channel Key Pressure           |
|   E   |               Pitch Bend                |



`if Event is 8`

`Note	<- Parameter`

`Midi Stop(Channel,Note)`



`if Event is 9`

`Note	<- Parameter`

`Velocity	<- Read 1 Byte in Trackdata`

`Midi Play(Channel,Note,Velocity)`



`if Event is A,B,E`

`Parameter2 <- Read 1 Byte in Trackdata`

`Midi Event(Channel,Event,Parameter1,Parameter2)`

`if Event is C,D`

`Midi Event(Channel,Event,Parameter1,0)`



`And deltaTime = ReadVLQ(TrackData)`

`TrackData[loop] += 60*deltaTime/Tempo*TickPerQuotes`



