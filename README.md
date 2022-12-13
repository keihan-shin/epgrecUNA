# epgrecUNA from 「適当な何かの別館」

コンテナ化などを想定し、録画システム「epgrecUNA」の関連ソースコードを置いています。

「当ブログで公開した物の取り込みや再利用等は、煮るなり焼くなり好きなようにしてください」の旨を epgrecUNA 開発者 katauna 氏のブログ記事より確認済。

https://katauna.hatenablog.com/

ただ、元々は axfc アップローダにパス付きで置かれていたファイルです。「公開は（できれば）やめてほしい」というご希望があるようにも思えます（裏読みすれば）。

ブログの文言を信じて「合法」判断、忖度なしでアップロードしておりますが、転載は katauna 氏にはご不満かもしれません。万一連絡があれば消す予定。

元本が必要な場合は https://katauna.hatenablog.com/ へ。

## 内容物

「バニラ版がほしい」「パッチもカスタム版も要らん」という人のほうが多いと想定していますのでパッチは別にしております。パッチは参考ないし無視でお願いします。

パッチなしのバニラ版は「PHP 5」でなければ動作しません。epgrecUNA_151114Fix2 相当（epgrecUNAディレクトリ以下）。

### epgrecUNA

epgrecUNA_151114.tar.gz

https://katauna.hatenablog.com/entry/20151114/1447500516

epgrecUNA151114Fix1.tar.gz

https://katauna.hatenablog.com/entry/20151205/1449313773

epgrecUNA151114Fix2.tar.gz

https://katauna.hatenablog.com/entry/20160411/1460383655

をまとめたものです。

### epgdumpUNA

epgdumpUNA160127.tar.gz を展開したもの。

https://katauna.hatenablog.com/entry/20151114/1447500516

### patches

既存の調査等を元に各種パッチを作成しておりますが「自己責任」扱いでお願いします。

簡便化のため do_patch.sh というスクリプトでパッチが適応できるようにしております。patchutils （ディストリにより別名か、とにかく patch コマンド）をインストールのうえ

```
chmod 755 do_patch.sh
./do_patch
```

で適応可能です。

デフォルトでは「patches/php74」のみを当てますが do_patch.sh 内に適応有無のフラグがありますので必要なものを有効化してください。

#### 【PHP 7.4 では要】patches/php74

以下の情報を元にパッチを作成したもの。

https://rasumus.hatenablog.com/entry/2020/10/08/152653

https://qiita.com/akiyuki_cxx/items/b0dd17e866bb8b0032ed

Docker 使用の都合で PHP 7.4 + Debian 11 を選択したため「PHP 7.4 で必要なパッチ」を入れています。

do_patch.sh をテキストエディタで開き `PATCH_PHP74=1` と指定するとパッチを適応します。デフォルトで「1」と指定済み。

##### PHP 8.0 対応はできていません

PHP 8.0.26 にて動作検証を行いましたが、録画一覧が出ないなどの不具合あり。

2022 年 12 月現在、本リポジトリにあるパッチでは PHP 8.0 での動作は NG です。PHP 8.0 は後日対応予定。

#### 【任意】patches/quake_alert 

下記 quake_alert_fix1.tar.gz を解凍したもの。epgrec/recomplete.php を置換するだけです。

https://katauna.hatenablog.com/entry/20160425/1461516298

設定すると、録画中に地震テロップが載るとログが出力されるとのこと。「自己責任で」との記載があり追加パッチ扱いにしております。

do_patch.sh をテキストエディタで開き `PATCH_QUAKE_ALERT=1` と指定するとファイルを置換します。

#### 【任意】patches/recpt1

stz2012 氏版 recpt1 に e-Better 社のデバイスを追加対応させるパッチ。e-Better 社の製品を使っている場合のみ必要です。 

なお stz2012 氏の recpt1 で epgrecUNA は動作します（httpパッチ適応済）。

https://github.com/stz2012/recpt1

do_patch.sh をテキストエディタで開き `PATCH_RECPT1=1` と指定するとパッチを適応します。ディレクトリが無い場合はダウンロードするか確認を取ります。

#### 【任意】patches/trans

トランスコード機能関連のパッチです。

設定ファイルの「%TS%」タグが誤ったファイルパスを出力する問題の対策、トラコン例の追加（QSV / NVEnc / Raspberry pi 4 の「v4lm2m」）、トラコン時のコマンドをログに出力するパッチを持っているため同梱しました。

do_patch.sh をテキストエディタで開き `PATCH_TRANS=1` と指定するとパッチを適応します。

## そのほか epgrecUNA を動かすためには

以下のファイルが必要です。

### ドライバ

PT1/PT2

http://hg.honeyplanet.jp/pt1

PT3

https://github.com/m-tsudo/pt3

px4_drv (Plex, e-Better 製品に対応）

https://github.com/nns779/px4_drv

その他 Digital Devices 社の製品(Max M4)などもありますが、容易に入手できるものは Plex 社の製品です。

### ライブラリ等

libarib25 (stz2012 氏版)

https://github.com/stz2012/libarib25

recpt1 (stz2012 氏版)

https://github.com/stz2012/recpt1

libpcsclite / pcscd (カードリーダー機能）

お使いのディストリに付属、例えば `apt install libpcsclite1 pcscd` など。

## epgrecUNA の設定の仕方が分からない！　助けて！

「epgrecUNA 設定」で検索すれば出ると思われますが、当方が参考にさせていただいたのは下記サイトです。

https://eco.senritu.net/ubuntu20-04%E3%81%ABepgrec%E3%82%92%E3%82%A4%E3%83%B3%E3%82%B9%E3%83%88%E3%83%BC%E3%83%AB%E3%81%99%E3%82%8B/

https://centossrv.com/epgrec_una.shtml

https://qiita.com/AnaKutsu/items/9b9f3886c8b8b05f7ec3

## ライセンス

派生元の epgrec は LGPL ライセンス。このため「LICENSE」ファイルは「LGPL」を指定しました。

http://www.mda.or.jp/epgrec/

epgrec UNA のライセンスは「当ブログで公開した物の取り込みや再利用等は、煮るなり焼くなり好きなようにしてください」の記載あり（再掲）。

https://katauna.hatenablog.com/
