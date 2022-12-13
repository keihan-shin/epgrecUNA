#!/bin/bash

# epgrecUNA に PHP 7.4 対応パッチを適応する(1) / 適応しない(0)
#
# 以下の調査に準拠したもの
# https://rasumus.hatenablog.com/entry/2020/10/08/152653
# https://qiita.com/akiyuki_cxx/items/b0dd17e866bb8b0032ed
PATCH_PHP74=1

# epgrecUNA に PHP 8.0 対応パッチを適応する(1) / 適応しない(0)
#
# 2022/12 現在、パッチは揃っていない。後日揃えるためにディレクトリ等の整備を行ったのみ。
PATCH_PHP80=0

# 地震テロップ混入をログで知らせるパッチを適応する(1) / 適応しない(0)
#
# 作者 katauna 氏より「自己責任」扱いの記載があるもの
PATCH_QUAKE_ALERT=0

# トラコン関連パッチを適応する（1) / 適応しない(0)
#
# 私家版のため適応しなくていいが、デフォルトのトラコン設定は ts ファイルのパスに問題がある
# ( video ディレクトリを見ていない）ため注意
PATCH_TRANS=0

# recpt1 に e-Better 製デバイスを認識させるパッチを適応する(1) / 適応しない(0)
# recpt1 フォルダが無ければ何もしない
#
# パッチ対象は「stz2012 氏版 recpt1」（本家ではない）
# https://github.com/stz2012/recpt1
#
# デバイス名は px4_drv に従う
# https://github.com/nns779/px4_drv
PATCH_RECPT1=0

# 何らかの事情でリバースパッチを当てて元に戻したい場合
REVERSE_PATCH=0

# epgrecUNA に PHP7.4 + 8.0 共通対応パッチを適応する(1) / 適応しない(0)
#
# PHP 7.4 用パッチか 8.0 パッチのフラグを立てたら有効化する、触らなくて良い
PATCH_PHP74_80=0

function do_patch () {
    echo ""
    echo "patching `basename $1`"
    echo ""

    for pat in `ls -1F ${1} | grep -v "/$"`; do
        if [ $REVERSE_PATCH -eq 1 ]; then
            patch -R -p1 < ${1}/$pat
        else
            patch -p1 < ${1}/$pat
        fi
    done
}

if [ $PATCH_PHP74 -eq 1 ]; then
    if [ $PATCH_PHP80 -eq 1 ]; then
        echo "'PATCH_PHP74' フラグと 'PATCH_PHP80' フラグは併用できません。"
        echo "処理を中断します。"
        
        exit 1
    fi
fi

if [ "$1" == "--reverse" ]; then
    REVERSE_PATCH=1
fi

if [ $PATCH_QUAKE_ALERT -eq 1 ]; then
    pushd ./epgrecUNA  > /dev/null 2>&1
    do_patch ../patches/quake_alert
    popd  > /dev/null 2>&1
fi

if [ $PATCH_PHP74 -eq 1 ]; then
    pushd ./epgrecUNA  > /dev/null 2>&1
    do_patch ../patches/php74
    popd  > /dev/null 2>&1

    PATCH_PHP74_80=1
fi

if [ $PATCH_PHP80 -eq 1 ]; then
    pushd ./epgrecUNA  > /dev/null 2>&1
    do_patch ../patches/php80
    popd  > /dev/null 2>&1
    
    PATCH_PHP74_80=1
fi

if [ $PATCH_PHP74_80 -eq 1 ]; then
    pushd ./epgrecUNA  > /dev/null 2>&1
    do_patch ../patches/php74_80
    popd  > /dev/null 2>&1
fi

if [ $PATCH_TRANS -eq 1 ]; then
    pushd ./epgrecUNA > /dev/null 2>&1
    do_patch ../patches/trans
    popd  > /dev/null 2>&1
fi

if [ $PATCH_RECPT1 -eq 1 ]; then
    if [ ! -d recpt1 ]; then
        echo ""
        echo "recpt1 not found. recpt1  (stz2012 version) is here."
        echo "git clone https://github.com/stz2012/recpt1.git"
        
        which git | grep "git" > /dev/null 2>&1
        if [ $? -eq 0 ]; then
            echo -n "download this? (y/N): "
            read REQ_DOWN
            
            if [ "${REQ_DOWN,,}" == "y" ]; then
                git clone https://github.com/stz2012/recpt1.git
            fi
       else
            echo "download from https://github.com/stz2012/recpt1"
        fi
    fi
        
    if [ -d recpt1 ]; then
        pushd ./recpt1  > /dev/null 2>&1
        do_patch ../patches/recpt1
        popd  > /dev/null 2>&1
    fi
fi
