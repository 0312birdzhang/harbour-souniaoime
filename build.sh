#!/bin/bash
# 一键同步 Windows 仓库源码到 WSL 副本并重新打包 Sailfish RPM
# 用法：在 Windows 侧运行（git-bash 或 wsl bash）：  ./build.sh
set -e

WSL_DIST="Ubuntu-22.04"
SRC="/mnt/d/code/harbour-souniaoime"
DEST="harbour-souniaoime"

echo "== [1/3] 同步源码到 WSL (rsync, 排除构建产物) =="
wsl -d "$WSL_DIST" -- bash -lc "cd ~/code && rsync -a --delete \
  --exclude='.git' \
  --exclude='RPMS' \
  --exclude='installroot' \
  --exclude='Makefile' \
  --exclude='*.o' \
  --exclude='*.so' \
  --exclude='moc_*.cpp' \
  --exclude='qrc_*.cpp' \
  --exclude='*.pro.user' \
  '$SRC/' '$DEST/' && echo '  同步完成'"

echo "== [2/3] 进入 SDK 编译打包 (mb2 -t SailfishOS-latest-aarch64 build) =="
wsl -d "$WSL_DIST" -u root -- bash -lc "timeout 900 /srv/sailfishos/sdks/sfossdk/sdk-chroot -u pc -m all -- bash -lc 'source ~/.hadk.env && cd ~/code/harbour-souniaoime && mb2 -t SailfishOS-latest-aarch64 build'"

echo "== [3/3] 完成 =="
wsl -d "$WSL_DIST" -- bash -lc "ls -la ~/code/harbour-souniaoime/RPMS/*.rpm"