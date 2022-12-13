・recfsusb2n httpサーバ版を使う方でアースソフトPTシリーズを使用していない場合は、別途recpt1ctlを導入してください。
・recpt1ctlを使用する方は、recpt1ctlに同梱したパッチをあてて下さい。
・www-data(apache)から syslog を読み書きできるようにパーミッション変更などの対処をしてください。

[パッチ一覧]
recpt1ctl.diff    recpt1ctl機能拡張パッチ（--extendオプションで負数を指定できるように改良・--sidオプションを追加）
                  新本家[b14397800eae]以前およびそのフォーク用です。
                  新本家[c8688d7d6382]以降には
                  recpt1 httpサーバ機能追加パッチ 新本家[c8688d7d6382]以降対応版 http://www1.axfc.net/u/3308402
                  を使用してください。
