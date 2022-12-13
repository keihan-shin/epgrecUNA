static STATION bsSta[] = {
	{ "NHK BS1", "4101.epgdata", 16625, 4, 101},
	{ "NHK BSプレミアム", "4103.epgdata", 16626, 4, 103},
	{ "BS日テレ", "4141.epgdata", 16592, 4, 141},
	{ "BS朝日", "4151.epgdata", 16400, 4, 151},
	{ "BS-TBS", "4161.epgdata", 16401, 4, 161},
	{ "BSジャパン", "4171.epgdata", 16433, 4, 171},
	{ "BSフジ", "4181.epgdata", 16593, 4, 181},
	{ "WOWOWプライム", "4191.epgdata", 16432, 4, 191},
	{ "WOWOWライブ", "4192.epgdata", 17488, 4, 192},
	{ "WOWOWシネマ", "4193.epgdata", 17489, 4, 193},
	{ "スター・チャンネル1", "4200.epgdata", 16529, 4, 200},
	{ "スター・チャンネル2", "4201.epgdata", 17520, 4, 201},
	{ "スター・チャンネル3", "4202.epgdata", 17520, 4, 202},
	{ "BS11", "4211.epgdata", 16528, 4, 211},
	{ "TwellV", "4222.epgdata", 16530, 4, 222},
	{ "放送大学1", "4231.epgdata", 18098, 4, 231},
	{ "放送大学2", "4232.epgdata", 18098, 4, 232},
	{ "放送大学3", "4233.epgdata", 18098, 4, 233},
	{ "グリーンチャンネル", "4234.epgdata", 18224, 4, 234},
	{ "BSアニマックス", "4236.epgdata", 18033, 4, 236},
	{ "FOX bs238", "4238.epgdata", 18096, 4, 238},
	{ "BSスカパー！", "4241.epgdata", 18097, 4, 241},
	{ "J SPORTS 1", "4242.epgdata", 18225, 4, 242},
	{ "J SPORTS 2", "4243.epgdata", 18226, 4, 243},
	{ "J SPORTS 3", "4244.epgdata", 18257, 4, 244},
	{ "J SPORTS 4", "4245.epgdata", 18258, 4, 245},
	{ "BS釣りビジョン", "4251.epgdata", 18288, 4, 251},
	{ "IMAGICA BS", "4252.epgdata", 18256, 4, 252},
	{ "日本映画専門チャンネル", "4255.epgdata", 18289, 4, 255},
	{ "ディズニー・チャンネル", "4256.epgdata", 18034, 4, 256},
	{ "D-Life", "4258.epgdata", 18290, 4, 258},
	{ "NHK総合テレビジョン（東京）", "4291.epgdata", 17168, 4, 291},
	{ "NHK教育テレビジョン（東京）", "4292.epgdata", 17168, 4, 292},
	{ "日本テレビ", "4294.epgdata", 17169, 4, 294},
	{ "テレビ朝日", "4295.epgdata", 17169, 4, 295},
	{ "TBSテレビ", "4296.epgdata", 17169, 4, 296},
	{ "テレビ東京", "4297.epgdata", 17169, 4, 297},
	{ "フジテレビ", "4298.epgdata", 17168, 4, 298},
	{ "放送大学ラジオ", "4531.epgdata", 18098, 4, 531},
	{ "WNI", "4910.ontvjapan.com", 16626, 4, 910},
};

static int bsStaCount = sizeof(bsSta) / sizeof (STATION);



static STATION csSta[] = {
	{ "スターｃｈプラス", "1002.ontvjapan.com", 24608, 6, 237},
	{ "日本映画専門ｃｈＨＤ", "1086.ontvjapan.com", 24608, 6, 239},
	{ "フジテレビＮＥＸＴ", "306ch.epgdata.ontvjapan", 24608, 6, 306},
	{ "ショップチャンネル", "1059.ontvjapan.com", 24704, 6, 55},
	{ "キッズステーションＨＤ", "1045.ontvjapan.com", 24704, 6, 335},
	{ "ザ・シネマ", "1217.ontvjapan.com", 24736, 6, 228},
	{ "スカチャンＨＤ８００", "800ch.epgdata.ontvjapan", 24736, 6, 800},
	{ "スカチャン８０１", "801ch.epgdata.ontvjapan", 24736, 6, 801},
	{ "スカチャン８０２", "802ch.epgdata.ontvjapan", 24736, 6, 802},
	{ "ｅ２プロモ", "100ch.epgdata.ontvjapan", 28736, 7, 100},
	{ "ホームドラマチャンネル", "294ch.epgdata.ontvjapan", 28736, 7, 294}, 
	{ "Ｊスポーツ　ＥＳＰＮ", "1025.ontvjapan.com", 28736, 7, 256},
	{ "ＦＯＸ", "1016.ontvjapan.com", 28736, 7, 312},
	{ "スペースシャワーＴＶ", "1018.ontvjapan.com", 28736, 7, 322},
	{ "カートゥーン　ネット", "1046.ontvjapan.com", 28736, 7, 331},
	{ "ディズニーＸＤ", "1213.ontvjapan.com", 28736, 7, 334}, 
	{ "東映チャンネル", "1010.ontvjapan.com", 28768, 7, 221},
	{ "衛星劇場", "1005.ontvjapan.com", 28768, 7, 222},
	{ "チャンネルＮＥＣＯ", "1008.ontvjapan.com", 28768, 7, 223},
	{ "洋画★シネフィル", "1009.ontvjapan.com", 28768, 7, 224},
	{ "スター・クラシック", "1003.ontvjapan.com", 28768, 7, 238},
	{ "時代劇専門チャンネル", "1133.ontvjapan.com", 28768, 7, 292},
	{ "スーパードラマ", "1006.ontvjapan.com", 28768, 7, 310},
	{ "ＡＸＮ", "1014.ontvjapan.com", 28768, 7, 311},
	{ "ナショジオチャンネル", "1204.ontvjapan.com", 28768, 7, 343},
	{ "ワンテンポータル", "110ch.epgdata.ontvjapan", 28864, 7, 110},
	{ "ゴルフチャンネル", "1028.ontvjapan.com", 28864, 7, 260},
	{ "テレ朝チャンネル", "1092.ontvjapan.com", 28864, 7, 303},
	{ "ＭＴＶ", "1019.ontvjapan.com", 28864, 7, 323},
	{ "ミュージック・エア", "1024.ontvjapan.com", 28864, 7, 324},
	{ "朝日ニュースター", "1067.ontvjapan.com", 28864, 7, 352},
	{ "ＢＢＣワールド", "1070.ontvjapan.com", 28864, 7, 353},
	{ "ＣＮＮｊ", "1069.ontvjapan.com", 28864, 7, 354},
	{ "Ｊスポーツ　１", "1041.ontvjapan.com", 28896, 7, 251},
	{ "Ｊスポーツ　２", "1042.ontvjapan.com", 28896, 7, 252},
	{ "ＪスポーツＰｌｕｓＨ", "1043.ontvjapan.com", 28896, 7, 253},
	{ "ＧＡＯＲＡ", "1026.ontvjapan.com", 28896, 7, 254},
	{ "ｓｋｙ・Ａスポーツ＋", "1040.ontvjapan.com", 28896, 7, 255},
	{ "宝塚プロモチャンネル", "101ch.epgdata.ontvjapan", 28928, 7, 101},
	{ "ＳＫＹ・ＳＴＡＧＥ", "1207.ontvjapan.com", 28928, 7, 290},
	{ "チャンネル銀河", "305ch.epgdata.ontvjapan", 28928, 7, 305},
	{ "ＡＴ-Ｘ", "1201.ontvjapan.com", 28928, 7, 333},
	{ "ヒストリーチャンネル", "1050.ontvjapan.com", 28928, 7, 342},
	{ "スカチャン８０３", "803ch.epgdata.ontvjapan", 28928, 7, 803},
	{ "スカチャン８０４", "804ch.epgdata.ontvjapan", 28928, 7, 804},
	{ "ムービープラスＨＤ", "1007.ontvjapan.com", 28960, 7, 240},
	{ "ゴルフネットワーク", "1027.ontvjapan.com", 28960, 7, 262},
	{ "ＬａＬａ　ＨＤ", "1074.ontvjapan.com", 28960, 7, 314},
	{ "フジテレビＯＮＥ", "1073.ontvjapan.com", 28992, 7, 307},
	{ "フジテレビＴＷＯ", "1072.ontvjapan.com", 28992, 7, 308}, 
	{ "アニマックス", "1047.ontvjapan.com", 28992, 7, 332},
	{ "ディスカバリー", "1062.ontvjapan.com", 28992, 7, 340},
	{ "アニマルプラネット", "1193.ontvjapan.com", 28992, 7, 341},
	{ "Ｃ-ＴＢＳウエルカム", "160ch.epgdata.ontvjapan", 29024, 7, 160},
	{ "ＱＶＣ", "1120.ontvjapan.com", 29024, 7, 161},
	{ "プライム３６５．ＴＶ", "185ch.epgdata.ontvjapan", 29024, 7, 185},
	{ "ファミリー劇場", "1015.ontvjapan.com", 29024, 7, 293},
	{ "ＴＢＳチャンネル", "3201.ontvjapan.com", 29024, 7, 301},
	{ "ディズニーチャンネル", "1090.ontvjapan.com", 29024, 7, 304},
	{ "MUSIC ON! TV", "1022.ontvjapan.com", 29024, 7, 325},
	{ "ＴＢＳニュースバード", "1076.ontvjapan.com", 29024, 7, 351},
	{ "日テレＧ＋ＨＤ", "1068.ontvjapan.com", 29056, 7, 257},
	{ "日テレプラス", "300ch.epgdata.ontvjapan", 29056, 7, 300},
	{ "Music Japan TV", "1208.ontvjapan.com", 29056, 7, 321},
	{ "日テレＮＥＷＳ２４", "2002.ontvjapan.com", 29056, 7, 350},
	{ "旅チャンネル", "1052.ontvjapan.com", 29056, 7, 362},
};

static int csStaCount = sizeof(csSta) / sizeof (STATION);