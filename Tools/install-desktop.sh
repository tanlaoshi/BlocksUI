#!/usr/bin/env bash
# 安装图标 + 桌面/应用菜单入口；Dock 靠 com.blocks.blocksui.desktop 对齐
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BIN="$ROOT/Build/meson/Code/blocksui-gui"
ICON_SRC="$ROOT/Assets/icons/hicolor"
ICON_FILE="$ROOT/Assets/icons/hicolor/128x128/apps/com.blocks.blocksui.png"
DESKTOP_NAME="com.blocks.blocksui.desktop"

if [[ ! -x "$BIN" ]]; then
  echo "missing binary: $BIN" >&2
  echo "run: cd \"$ROOT\" && meson setup Build/meson && meson compile -C Build/meson" >&2
  exit 1
fi

for size in 16 24 32 48 64 128 256; do
  src="$ICON_SRC/${size}x${size}/apps/com.blocks.blocksui.png"
  if [[ ! -f "$src" ]]; then
    continue
  fi
  dest_dir="$HOME/.local/share/icons/hicolor/${size}x${size}/apps"
  mkdir -p "$dest_dir"
  cp -f "$src" "$dest_dir/com.blocks.blocksui.png"
  cp -f "$src" "$dest_dir/blocksui.png"
done

mkdir -p "$HOME/.local/share/applications"
rm -f "$HOME/.local/share/applications/blocksui.desktop"

cat >"$HOME/.local/share/applications/${DESKTOP_NAME}" <<ENTRY
[Desktop Entry]
Type=Application
Version=1.0
Name=BlocksUI
Name[zh_CN]=BlocksUI 设计器
Comment=Blocks host UI designer (.uitxt)
Comment[zh_CN]=Blocks 主机侧拖控件设计器
Exec=${BIN}
Path=${ROOT}
Icon=${ICON_FILE}
Terminal=false
Categories=Development;
StartupNotify=true
StartupWMClass=com.blocks.blocksui
X-GNOME-UsesNotifications=false
ENTRY
chmod 644 "$HOME/.local/share/applications/${DESKTOP_NAME}"

DESKTOP_DIR="${XDG_DESKTOP_DIR:-}"
if [[ -z "$DESKTOP_DIR" ]] && command -v xdg-user-dir >/dev/null 2>&1; then
  DESKTOP_DIR="$(xdg-user-dir DESKTOP)"
fi
if [[ -z "$DESKTOP_DIR" || ! -d "$DESKTOP_DIR" ]]; then
  if [[ -d "$HOME/桌面" ]]; then
    DESKTOP_DIR="$HOME/桌面"
  elif [[ -d "$HOME/Desktop" ]]; then
    DESKTOP_DIR="$HOME/Desktop"
  fi
fi
if [[ -n "${DESKTOP_DIR:-}" && -d "$DESKTOP_DIR" ]]; then
  rm -f "$DESKTOP_DIR/blocksui.desktop"
  cp -f "$HOME/.local/share/applications/${DESKTOP_NAME}" "$DESKTOP_DIR/${DESKTOP_NAME}"
  chmod +x "$DESKTOP_DIR/${DESKTOP_NAME}"
  if command -v gio >/dev/null 2>&1; then
    gio set "$DESKTOP_DIR/${DESKTOP_NAME}" metadata::trusted true 2>/dev/null || true
  fi
  echo "desktop shortcut: $DESKTOP_DIR/${DESKTOP_NAME}"
fi

if command -v gtk-update-icon-cache >/dev/null 2>&1; then
  gtk-update-icon-cache -f -t "$HOME/.local/share/icons/hicolor" 2>/dev/null || true
fi
if command -v update-desktop-database >/dev/null 2>&1; then
  update-desktop-database "$HOME/.local/share/applications" 2>/dev/null || true
fi

echo "menu entry: $HOME/.local/share/applications/${DESKTOP_NAME}"
echo "Dock tip: quit BlocksUI, launch from menu/桌面 (not raw binary); pin if needed."
echo "done."
