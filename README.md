# 163-lyrics-getter
获取网易云音乐歌词。 | Get lyrics and translated lyrics from NetEase Cloud Music (music.163.com).

----
## 网易云音乐API：

### 歌词

`GET http://music.163.com/api/song/lyric`

| 参数 |  | 
|------|---------|
| id   | 歌曲ID |
| lv   | -1：获取lrc（原歌词LRC），1：不获取 |
| tv   | -1：获取tlyric（翻译后的歌词LRC），1：不获取 |

### 歌曲信息

`GET http://music.163.com/api/song/detail`

| 参数 |  | 
|------|---------|
| ids   | [歌曲ID]，如有多首歌则以逗号分隔 |