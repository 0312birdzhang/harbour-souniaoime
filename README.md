# harbour-souniaoime

## 声明

本程序前端代码部分来自 https://openrepos.net/content/mumudada/pin-yin-shu-ru-fa-shan-zhai-gu-ge 跟 https://openrepos.net/content/gexc/pinyin-prime-pin-yin-jian-pan ，本人魔改了一些。

后端代码来自 QtVirtualkeyboard pinyin 部分

调试：

`pkill maliit-server; MALIIT_DEBUG=enabled maliit-server`

打包:

```
export RELEASE="4.1.0.24"
export ARCH="armv7hl"
export TARGET=SailfishOS-$RELEASE-$ARCH
mb2 -t $TARGET qmake #必须
mb2 -t $TARGET build
mb2 -t $TARGET package
```