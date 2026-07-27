# harbour-souniaoime

## Declaration

The frontend code of this program is derived from https://openrepos.net/content/mumudada/pin-yin-shu-ru-fa-shan-zhai-gu-ge and https://openrepos.net/content/gexc/pinyin-prime-pin-yin-jian-pan, with some modifications made by myself.

The backend code is derived from the pinyin section of QtVirtualkeyboard.

OpenCC is used for Simplified/Traditional Chinese conversion, and the plugin was written by Topiasv.

Debugging:

`pkill maliit-server; MALIIT_DEBUG=enabled maliit-server`

Packaging:

```
export RELEASE="4.1.0.24"
export ARCH="armv7hl"
export TARGET=SailfishOS-$RELEASE-$ARCH
mb2 -t $TARGET qmake #required
mb2 -t $TARGET build
mb2 -t $TARGET package
```
